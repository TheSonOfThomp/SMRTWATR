// Sweep
// by BARRAGAN <http://barraganstudio.com> 

#include <Servo.h> 

const int BUFFER_LEN = 6;
char instr_buff[BUFFER_LEN];
int DISCRETE_JET_HEIGHTS[4] = {600, 1100, 2500, 4095}; // discrete heights to be used during quiz mode

// jet mapping is
// center, bottom-left, top-left, top-right, bottom-right (ccw)
int jet_height[5] = {0, 0, 0, 0, 0};
int jet_pins[5] = {23, 22, 20, 17, 16};

// servo mapping is
// bottom-left, top-left, top-right, bottom-right (ccw)
Servo servo[4];
int servo_pins[4] = {3, 4, 6, 9};

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

void set_discrete_pump_heights() {
  for (int ii = 0; ii < 4; ii++) {
    jet_height[ii] = DISCRETE_JET_HEIGHTS[instr_buff[ii + 2] - '0'];
    Serial.println(jet_height[ii]);
    analogWrite(jet_pins[ii], jet_height[ii]);
  }
}

