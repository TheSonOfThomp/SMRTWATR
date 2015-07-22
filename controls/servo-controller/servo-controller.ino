
#include "Delay.h"
#include <Servo.h>

const bool CW = true;
const bool CCW = false;

const int BUFFER_LEN = 6;
const int SERVO_DELAY = 25; // ms per degree
const int SERVO_LL = 25;
const int SERVO_RL = 65;
const int SERVO_MID = (SERVO_RL - SERVO_LL)/2 + SERVO_LL;
const int CJET_QHEIGHT = 3; //height of the center jet during the quiz (see discrete_jet_heights array below --> 4095)
int timecount = 0; //counts the number of 25ms increments that have passed. 
char instr_buff[BUFFER_LEN];
char prev_state;
int DISCRETE_JET_HEIGHTS[4] = {2500, 3000, 3500, 4095}; // discrete heights to be used during quiz mode
NonBlockDelay d;

// jet mapping is
// bottom-left, top-left, top-right, bottom-right (ccw), center
int jet_height[5] = {0, 0, 0, 0, 3};
int jet_pins[5] = {23, 22, 20, 17, 16};

// servo mapping is
// bottom-left, top-left, top-right, bottom-right (ccw)
Servo servo[4];
int servo_pins[4] = {3, 4, 6, 9};
int servo_pos[4] = {SERVO_MID, SERVO_MID, SERVO_MID, SERVO_MID}; 
bool servo_sweep[4] = {CW, CW, CW, CW};
int prev_scores[4] = {0, 0, 0, 0};
int prev_jet_height[5] = {10, 10, 10, 10, 10};
bool score_changed[4] = {false, false, false, false};

void setup() {
    Serial.begin(9600);
    analogWriteResolution(12);
    // PWM pins on the left side of the board
    for(int ii=0; ii<4; ii++) {
        servo[ii].attach(servo_pins[ii]);
    }
}

// fountain has two modes:
// 1. Quiz gameplay mode, in which it simply responds to the scores of the users
// 2. Routine mode, in which it executes a full display

// data format
// q [1-x] [0-3] [0-3] [0-3] [0-3] 
// q10234 question 1, player 1: 0 points player 2: 2 points ect.
// q is the start bit
// [0-x] is the routine. Routines 1, 2, 3 are quiz questions
// [0-3] [0-3] [0-3] [0-3] [0-3] are the jet heights representing player's scores

void loop() {
    if (Serial.peek() == 'q') {
        if (Serial.available()>=BUFFER_LEN) {
            Serial.readBytes(instr_buff, BUFFER_LEN);
            Serial.println("r");
            if (instr_buff[1] != prev_state) {
              routine_setup();
              prev_state = instr_buff[1];
              score_changed[0] = false;
              score_changed[1] = false;
              score_changed[2] = false;
              score_changed[3] = false;
            }
            else {
                quiz_score_routine();
            }
        }
    } else if (Serial.available() && Serial.peek() != 'q') {
        Serial.read();
    }
    routine();
}

void routine_setup() {
    int p0 = instr_buff[2] - '0';
    int p1 = instr_buff[3] - '0';
    int p2 = instr_buff[4] - '0';
    int p3 = instr_buff[5] - '0';
    int p4 = CJET_QHEIGHT;
    
    switch (instr_buff[1]) {
    case '0':
        set_discrete_pump_heights(2,2,2,2,3);
        break;
    case '1':
        set_discrete_pump_heights(p0, p1, p2, p3, 3);
        servo_sweep[0] = servo_sweep[1] = servo_sweep[2] = servo_sweep[3] = CW;
        zero_servos();
        break;
    case '2':
        servo_sweep[0] = servo_sweep[2] = CW;
        servo_sweep[1] = servo_sweep[3] = CCW;
        set_discrete_pump_heights(p0, p1, p2, p3, 3);
        zero_servos();
        break;
    case '3':
        set_discrete_pump_heights(p0, p1, p2, p3, 3);
        servo_sweep[0] = servo_sweep[1] = servo_sweep[2] = servo_sweep[3] = CW;
        zero_servos();
        break;
     case '4':
        //Serial.printf("final routine 1 setup \n");
        servo_sweep[0] = servo_sweep[1] = servo_sweep[2] = servo_sweep[3] = CW;
        //serial.print(servo_sweep[0]);
        set_discrete_pump_heights(0, 0, 0, 0, 3);
        zero_servos();
        break;
     case '5':
        //serial.printf("final routine 2 setup \n");
        servo_sweep[0] = servo_sweep[1] = CCW;
        servo_sweep[2] = servo_sweep[3] = CW;
        set_discrete_pump_heights(0, 0, 0, 0, 3);
        zero_servos();
        break;
     case '6':
        //serial.printf("final routine 3 setup \n");
        servo_sweep[0] = servo_sweep[1] = CW;
        servo_sweep[2] = servo_sweep[3] = CCW;
        set_discrete_pump_heights(0, 0, 0, 0, 0);
        zero_servos();
        break;
     case '7':
        //serial.printf("final routine 4 setup \n");
        servo_sweep[0] = servo_sweep[1] = CCW;
        servo_sweep[2] = servo_sweep[3] = CW;
        set_discrete_pump_heights(0, 0, 0, 0, 3);
        zero_servos();
        break;
    case 'x':
        servo_sweep[0] = servo_sweep[1] = servo_sweep[2] = servo_sweep[3] = CW;
        set_discrete_pump_heights(3, 3, 3, 3, 3);
        zero_servos();
        break;
     case '8':
        rest_seq();
        set_discrete_pump_heights(2,2,2,2,3);
        break;
      case '9':
        set_discrete_pump_heights(3,3,3,3,3);
        break;
    }
}



void routine() {
    switch (instr_buff[1]) {
    case '0':
        break;
    case '1':
    case '2':
    case '3':
        if (d.Timeout()) {
            sweep_servos();
            d.Delay(SERVO_DELAY);
        }
        break;
    case '4':
        if (d.Timeout()) {
            win_seq1();
            d.Delay(SERVO_DELAY);
        }
        break;    
    case '5':
        if (d.Timeout()) {
            win_seq2();
            d.Delay(SERVO_DELAY);
        }
        break; 
    case '6':
        if (d.Timeout()) {
            win_seq3();
            d.Delay(SERVO_DELAY);
        }
        break;
    case '7':
        if (d.Timeout()) {
            win_seq4();
            d.Delay(SERVO_DELAY);
        }
        break;
    case '8':
        break;    
    }
}

void quiz_score_routine() {
    int p0 = instr_buff[2] - '0';
    int p1 = instr_buff[3] - '0';
    int p2 = instr_buff[4] - '0';
    int p3 = instr_buff[5] - '0';

    if (prev_scores[0] != p0) {
        score_changed[0] = true;
    }
    if (prev_scores[1] != p1) {
        score_changed[1] = true;
    }
    if (prev_scores[2] != p2) {
        score_changed[2] = true;
    }
    if (prev_scores[3] != p3) {
        score_changed[3] = true;
    }
    
    int p4 = CJET_QHEIGHT;
    // called when the score changes
    set_discrete_pump_heights(p0, p1, p2, p3, p4);
}

void set_discrete_pump_heights(int p0, int p1, int p2, int p3, int p4) {
   ////serial.printf("discrete jet heights: %d, %d, %d, %d, %d", p0, p1, p2, p3); 
    if (p0 != prev_jet_height[0]){
        analogWrite(jet_pins[0], DISCRETE_JET_HEIGHTS[p0]);
    } 
    if (p1 != prev_jet_height[1]){
        analogWrite(jet_pins[1], DISCRETE_JET_HEIGHTS[p1]);
    }
    if (p2 != prev_jet_height[2]){
        analogWrite(jet_pins[2], DISCRETE_JET_HEIGHTS[p2]);
    } 
    if (p3 != prev_jet_height[3]){
        analogWrite(jet_pins[3], DISCRETE_JET_HEIGHTS[p3]);
    } 
    if (p4 != prev_jet_height[4]){
        analogWrite(jet_pins[4], DISCRETE_JET_HEIGHTS[p4]);
    } 
    prev_jet_height[0] = p0;
    prev_jet_height[1] = p1;
    prev_jet_height[2] = p2;
    prev_jet_height[3] = p3;
    prev_jet_height[4] = p4;
}

void zero_servos() {
    // reset servos to either the left or right edge before the routine starts
    // do this smoothely (as opposed to in a single sweep) to avoid whipping the jet around
    int zeroed_servos = 0;
    while (zeroed_servos < 4 && d.Timeout()) {
        for (int ii=0; ii<4; ii++)  {
            if (servo_sweep[ii]==CW && servo_pos[ii] > SERVO_LL) {
                servo_pos[ii] -= 1;
                servo[ii].write(servo_pos[ii]);
            } else if (servo_sweep[ii]==CCW && servo_pos[ii] < SERVO_RL) {
                servo_pos[ii] += 1;
                servo[ii].write(servo_pos[ii]);
            } else {
                servo_sweep[ii] = !servo_sweep[ii];
                zeroed_servos++;
            }
        }
        d.Delay(SERVO_DELAY);
    }
}

void sweep_servos() {
    // sweep the servos back and forth based on the initial condition given in servo_sweep
    for (int ii=0; ii<4; ii++) {
        if (score_changed[ii]) {
            // reverse direction if necessary
            if (servo_sweep[ii]==CW && servo_pos[ii] >= SERVO_RL) {
                servo_sweep[ii] = CCW;
            }
            if (servo_sweep[ii]==CCW && servo_pos[ii] <= SERVO_LL) {
                servo_sweep[ii] = CW;
            }
            // advance servos if necessary
            if (servo_sweep[ii] == CW) {
                servo_pos[ii] += 1;
                servo[ii].write(servo_pos[ii]);
            } else {
                servo_pos[ii] -= 1;
                servo[ii].write(servo_pos[ii]);
            }
        } else {
              servo[ii].write(SERVO_MID);
        }
    }
}

void rest_seq(){
   for (int ii=0; ii<4; ii++) {
            servo_pos[ii] = SERVO_MID;
            servo[ii].write(servo_pos[ii]);
    }
}
void win_seq1(){
  // first option 
  // water goes from one pump to another (hopefully)
  
  if (servo_pos[0] < SERVO_RL && servo_sweep[0] ){
    //serial.printf("\n m1 moving right");
    servo_pos[0] += 1;
    servo[0].write(servo_pos[0]); 
    if (servo_pos[3] > SERVO_LL)
    {
      servo_sweep[3] = CCW;
      servo_pos[3] -= 1;
      //serial.printf("\n m4 moving back left");
      servo[3].write(servo_pos[3]);
    }
    set_discrete_pump_heights(3, 0, 0, 0, 3);
  }   
  else if (servo_pos[1] < SERVO_RL && servo_sweep[1] == CW){
    servo_pos[1] += 1;
    servo[1].write(servo_pos[1]); 
    //serial.printf("\n m2 moving right");
    servo_sweep[0] = CCW;
    if (servo_pos[0] > SERVO_LL){
      servo_pos[0] -= 1;
      servo[0].write(servo_pos[0]);
      //serial.printf("\n m1 moving back left");
    }
    set_discrete_pump_heights(0, 3, 0, 0, 3);
  }
  else if (servo_pos[2] < SERVO_RL && servo_sweep[2] == CW){
    servo_pos[2] += 1;
    servo[2].write(servo_pos[2]);
    //serial.printf("\n m3 moving right");
    servo_sweep[1] = CCW;
    if (servo_pos[1] > SERVO_LL){
      servo_pos[1] -= 1;
      servo[1].write(servo_pos[1]); 
      //serial.printf("\n m2 moving back left");
    }
    set_discrete_pump_heights(0, 0, 3, 0, 3);
  }
  
  else if (servo_pos[3] < SERVO_RL && servo_sweep[3] == CW){
    servo_pos[3] += 1;
    servo[3].write(servo_pos[3]); 
    //serial.printf("\n m4 moving right");
    servo_sweep[2] = CCW;
    if (servo_pos[2] > SERVO_LL){
      servo_pos[2] -= 1;
      servo[2].write(servo_pos[2]);
      //serial.printf("\n m3 moving back left");
    }
    set_discrete_pump_heights(0, 0, 0, 3, 3);
  }
  else{
    servo_sweep[0] = CW;
    servo_sweep[1] = CW;
    servo_sweep[2] = CW;
    servo_sweep[3] = CW;
    set_discrete_pump_heights(0, 0, 0, 0, 3);
  }
}

void win_seq2(){
  //serial.print("timecount:"+ timecount);
  if (timecount == 200){ //first 5 seconds
    set_discrete_pump_heights(3, 3, 3, 3, 3);
  }
  else if (timecount == 400){
    set_discrete_pump_heights(0, 0, 0, 0, 3);
  }
  else if (timecount == 600){
    set_discrete_pump_heights(3, 0, 3, 0, 3);
  }
  else if (timecount == 800) {
    set_discrete_pump_heights(0, 3, 0, 3, 3);
  }
  else if (timecount == 1000) {
    set_discrete_pump_heights(3, 3, 3, 3, 3);
  }
  else if (timecount == 1200) {
    timecount = 0;
  }
  timecount += 1;
  for (int ii=0; ii<4; ii++) {
        //serial.printf("\n in for loop");
        // reverse direction if necessary
        if (servo_sweep[ii]==CW && servo_pos[ii] >= SERVO_RL) {
            servo_sweep[ii] = CCW;
        }
        if (servo_sweep[ii]==CCW && servo_pos[ii] <= SERVO_LL) {
            servo_sweep[ii] = CW;
        }
        // advance servos if necessary
        if (servo_sweep[ii] == CW) {
            servo_pos[ii] += 1;
            servo[ii].write(servo_pos[ii]);
        } 
        else {
            servo_pos[ii] -= 1;
            servo[ii].write(servo_pos[ii]);
        }
    }
  
}

void win_seq3(){
  //serial.print("timecount:"+ timecount);
  if (timecount == 200){ //first 5 seconds
    set_discrete_pump_heights(3, 3, 3, 3, 3);
  }
  else if (timecount == 400){
    set_discrete_pump_heights(0, 0, 0, 0, 3);
  }
  else if (timecount == 600){
    set_discrete_pump_heights(3, 0, 3, 0, 3);
  }
  else if (timecount == 800) {
    set_discrete_pump_heights(0, 3, 0, 3, 3);
  }
  else if (timecount == 1000) {
    set_discrete_pump_heights(3, 3, 3, 3, 3);
  }
  else if (timecount == 1200) {
    timecount = 0;
  }
  timecount += 1;
  
  
  for (int ii=1; ii<=3; ii+=2) {
        //serial.printf("\n in for loop");
        // reverse direction if necessary
        if (servo_sweep[1]==CW && servo_pos[ii] >= SERVO_RL) {
            servo_sweep[ii] = CCW;
        }
        if (servo_sweep[ii]==CCW && servo_pos[ii] <= SERVO_LL) {
            servo_sweep[ii] = CW;
        }
        // advance servos if necessary
        if (servo_sweep[ii] == CW) {
            servo_pos[ii] += 1;
            servo[ii].write(servo_pos[ii]);
        } 
        else {
            servo_pos[ii] -= 1;
            servo[ii].write(servo_pos[ii]);
        }
    }
  for (int ii=0; ii<=2; ii+=2) {
        //serial.printf("\n in for loop");
        // reverse direction if necessary
        if (servo_sweep[ii]==CW && servo_pos[ii] >= SERVO_RL) {
            servo_sweep[ii] = CCW;
        }
        if (servo_sweep[ii]==CCW && servo_pos[ii] <= SERVO_LL) {
            servo_sweep[ii] = CW;
        }
        // advance servos if necessary
        if (servo_sweep[ii] == CW) {
            servo_pos[ii] += 1;
            servo[ii].write(servo_pos[ii]);
        } 
        else {
            servo_pos[ii] -= 1;
            servo[ii].write(servo_pos[ii]);
        }
    }   
} 

void win_seq4(){
  //serial.print("timecount:"+ timecount);
  if (timecount == 200){ //first 5 seconds
    set_discrete_pump_heights(3, 3, 3, 3, 0);
  }
  else if (timecount == 400){
    set_discrete_pump_heights(0, 0, 0, 0, 3);
  }
  else if (timecount == 600){
    set_discrete_pump_heights(3, 0, 3, 0, 3);
  }
  else if (timecount == 800) {
    set_discrete_pump_heights(0, 3, 0, 3, 3);
  }
  else if (timecount == 1000) {
    set_discrete_pump_heights(3, 3, 3, 3, 3);
  }
  else if (timecount == 1200) {
    timecount = 0;
  }
  timecount += 1;
  
  
  for (int ii=1; ii<=3; ii+=2) {
        //Serial.printf("\n in for loop");
        // reverse direction if necessary
        if (servo_sweep[1]==CW && servo_pos[ii] >= SERVO_RL) {
            servo_sweep[ii] = CCW;
        }
        if (servo_sweep[ii]==CCW && servo_pos[ii] <= SERVO_LL) {
            servo_sweep[ii] = CW;
        }
        // advance servos if necessary
        if (servo_sweep[ii] == CW) {
            servo_pos[ii] += 1;
            servo[ii].write(servo_pos[ii]);
        } 
        else {
            servo_pos[ii] -= 5;
            servo[ii].write(servo_pos[ii]);
        }
    }
  for (int ii=0; ii<=2; ii+=2) {
        //Serial.printf("\n in for loop");
        // reverse direction if necessary
        if (servo_sweep[ii]==CW && servo_pos[ii] >= SERVO_RL) {
            servo_sweep[ii] = CCW;
        }
        if (servo_sweep[ii]==CCW && servo_pos[ii] <= SERVO_LL) {
            servo_sweep[ii] = CW;
        }
        // advance servos if necessary
        if (servo_sweep[ii] == CW) {
            servo_pos[ii] += 5;
            servo[ii].write(servo_pos[ii]);
        } 
        else {
            servo_pos[ii] -= 1;
            servo[ii].write(servo_pos[ii]);
        }
    }   
} 

