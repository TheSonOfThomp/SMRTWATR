
#include "Delay.h"
#include <Servo.h>

const bool CW = true;
const bool CCW = false;

const int BUFFER_LEN = 6;
const int SERVO_DELAY = 25; // ms per degree
const int SERVO_LL = 10;
const int SERVO_RL = 170;
char instr_buff[BUFFER_LEN];
char prev_state;
int DISCRETE_JET_HEIGHTS[4] = {600, 1100, 2500, 4095}; // discrete heights to be used during quiz mode
NonBlockDelay d;

// jet mapping is
// bottom-left, top-left, top-right, bottom-right (ccw), center
int jet_height[5] = {0, 0, 0, 0, 0};
int jet_pins[5] = {23, 22, 20, 17, 16};

// servo mapping is
// bottom-left, top-left, top-right, bottom-right (ccw)
Servo servo[4];
int servo_pins[4] = {3, 4, 6, 9};
int servo_pos[4] = {0, 0, 0, 0};
bool servo_sweep[4] = {CW, CW, CW, CW};

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
            Serial.println(instr_buff);
            if (instr_buff[1] != prev_state) {
              routine_setup();
              prev_state = instr_buff[1];
            }
            else {
              quiz_score_routine();
            }
        }
    } else {
        Serial.read();
    }
    routine();
}

void routine_setup() {
    switch (instr_buff[1]) {
    case '1':
        set_discrete_pump_heights();
        servo_sweep[0] = servo_sweep[1] = servo_sweep[2] = servo_sweep[3] = CW;
        zero_servos();
        break;
    case '2':
        servo_sweep[0] = servo_sweep[2] = CW;
        servo_sweep[1] = servo_sweep[3] = CCW;
        set_discrete_pump_heights();
        zero_servos();
        break;
    case '3':
        set_discrete_pump_heights();
        servo_sweep[0] = servo_sweep[1] = servo_sweep[2] = servo_sweep[3] = CW;
        zero_servos();
        break;
    case '4':
        break;
    }
}

void routine() {
    switch (instr_buff[1]) {
    case '1':
    case '2':
    case '3':
        if (d.Timeout()) {
            sweep_servos();
            d.Delay(SERVO_DELAY);
        }
        break;
    case '4':
        break;
    }
}

void quiz_score_routine() {
    // called when the score changes
    set_discrete_pump_heights();
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
        // reverse direction if necessary
        if (servo_sweep[ii]==CW && servo_pos[ii] > SERVO_RL) {
            servo_sweep[ii] = CCW;
        }
        if (servo_sweep[ii]==CCW && servo_pos[ii] < SERVO_LL) {
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
    }
}
