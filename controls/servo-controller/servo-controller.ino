#include "Delay.h"
#include <Servo.h> 

const int BUFFER_LEN = 6;
const int SERVO_DELAY = 25; // ms per degree
const int SERVO_LL = 10;
const int SERVO_RL = 170;
char instr_buff[BUFFER_LEN];
int DISCRETE_JET_HEIGHTS[4] = {600, 1100, 2500, 4095}; // discrete heights to be used during quiz mode
NonBlockDelay d;

// jet mapping is
// center, bottom-left, top-left, top-right, bottom-right (ccw)
int jet_height[5] = {0, 0, 0, 0, 0};
int jet_pins[5] = {23, 22, 20, 17, 16};

// servo mapping is
// bottom-left, top-left, top-right, bottom-right (ccw)
Servo servo[4];
int servo_pins[4] = {3, 4, 6, 9};
int servo_pos[4] = {0, 0, 0, 0};
bool servo_sweep[4] = {0, 0 , 0, 0}; // 0 = cw, 1 = ccw

void setup() { 
  Serial.begin(9600);
  analogWriteResolution(12);
  // PWM pins on the left side of the board
  for(int ii=0; ii<4; ii++){
    servo[ii].attach(servo_pins[ii]);
  }
} 

// fountain has two modes:
// 1. Quiz gameplay mode, in which it simply responds to the scores of the users
// 2. Routine mode, in which it executes a full display

// data format
// q [0-x] [0-3] [0-3] [0-3] [0-3]
// q is the start bit
// [0-x] is the routine. Routines 1, 2, 3, 4 are quiz questions
// [0-3] [0-3] [0-3] [0-3] [0-3] are the jet heights representing player's scores
 
void loop() {   
  if (Serial.peek() == 'q') {
    if (Serial.available()>=BUFFER_LEN) {
      Serial.readBytes(instr_buff, BUFFER_LEN);
      Serial.println(instr_buff);
      routine_setup(); // instructions have just been recieved, set any initial state here
      } 
    } else {
      Serial.read();
    }
  }

void routine_setup() {
  switch (instr_buff[1]) {
    case '1':
       set_discrete_pump_heights();
       // place servos at leftmost edge
       break;
    case '2':
       set_discrete_pump_heights();
       break;
    case '3':
      set_discrete_pump_heights();
      break;
    case '4':
      set_discrete_pump_heights();
      break;         
  }
}

void routine() {
  switch (instr_buff[1]) {
    case '1':
    break;
    case '2':
    break;
    case '3':
    break;
    case '4':
    break;
    case '5':
    break;
  }
}

void set_discrete_pump_heights() {
  // set discrete jet heights (1, 2, 3, 4) during the quiz
  for (int ii = 0; ii < 4; ii++) {
    jet_height[ii] = DISCRETE_JET_HEIGHTS[instr_buff[ii + 2] - '0'];
    Serial.println(jet_height[ii]);
    analogWrite(jet_pins[ii], jet_height[ii]);
  }
}

void zero_servos() {
  // reset servos to either the left or right edge before the routine starts
  int zeroed_servos = 0;
  while (zeroed_servos < 4 && d.Timeout()) {
    for (int ii=0; ii<4; ii++)  {
      if (servo_sweep[ii] && servo_pos[ii] > SERVO_LL) {
        servo_pos[ii] -= 1;
        servo[ii].write(servo_pos[ii]);
      } else if (!servo_sweep[ii] && servo_pos[ii] < SERVO_RL) {
        servo_pos[ii] += 1;
        servo[ii].write(servo_pos[ii]);
      } else {
        zeroed_servos++;
      }
    }
    d.Delay(SERVO_DELAY);
  }
}

void sweep_servos() {
  for (int ii=0; ii<4; ii++) {
    if (servo_sweep[ii])
  }
}

