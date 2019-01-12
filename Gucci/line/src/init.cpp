//
// Created by Logan Schick on 9/6/2018.
//

#include <Particle.h>

#include <VL53L0X.h>

#if !defined(PARTICLE)
#  include <Wire.h>
#endif

#include <sensors.h>

#include <movement.h>

#include <init.h>

//Defining variables
VL53L0X sensor0;
VL53L0X sensor1;
VL53L0X sensor2;
VL53L0X sensor3;
VL53L0X sensor4;
VL53L0X sensor5;

// Line sensors: Front Left, Front Right, Back Left, Back Right
int FL = A2;    // AUX BOARD SWITCHED FL and FR up
int FR = A3;    // A5 does not support attachInterrupt, so jump A5 to D7 on the board, also cut INT line
int BL = A4;
int BR = A6;
// Line sensor flags that are flipped at an interrupt
boolean FLflag = true;
boolean FRflag = true;
boolean BLflag = true;
boolean BRflag = true;

// timer for current time in milis()
int cur = 0;

// timer for time elapsed after cur for timed movements
int prevFlag = 0;

// flag that helps prioritize timed movements, which are most likely for line detection
boolean prevFlagSet = false;

// flag to denote whether or not we want to run startup sequence
boolean start = true;

// remote start pin
int RS = D5;
// remote start flag that is flipped once we press the button on the remote
boolean RSflag = false;

// setting pins for the motors
int Lmotor = D3;
int Rmotor = A5;

// set up encoders pins and counters
int Lencoder = D4;
int Rencoder = RX;
int LHit = 0;
int RHit = 0;
boolean LStall = false;
boolean RStall = false;

//Setting flags to line sensors pins during an interrupt handler
// void FLISR() {
//     FLflag = digitalRead(FL);
//     // moveState(10);
// }

// void FRISR() {
//     FRflag = digitalRead(FR);
//     // moveState(10);
// }

// void BLISR() {
//     BLflag = digitalRead(BL);
// }

// void BRISR() {
//     BRflag = digitalRead(BR);
// }

// interrupt handler for remote start
void RSISR() {
    RSflag = digitalRead(RS);
}


void LencoderISR() {
    LHit = cur;
}

void RencoderISR() {
    RHit = cur;
}

//Setting up TOF sensors
//Add 2 more ToF sensors All the pins need to be fixed bc they've changed
void tof_init() {
    // GND to GND
    // VIN to VIN
    // D0 to SDA
    // D1 to SCL
    // A0 to SHDN0
    // A1 to SHDN1
    // A7 to SHDN2
    // D2 to SHDN3
    // D6 to SHDN4
    // D7 to SHDN5
    // procedure to set up multiple sensors to one I2C port
    pinMode(A0, OUTPUT);
    pinMode(A1, OUTPUT);
    pinMode(A7, OUTPUT);
    pinMode(D2, OUTPUT);
    pinMode(D6, OUTPUT);
    pinMode(D7, OUTPUT);
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A7, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
    delay(500);

    Wire.setSpeed(CLOCK_SPEED_100KHZ);  // need to initialize i2c protocol after driving shdn pins low
    Wire.begin();
    delay(10);

    digitalWrite(A0, HIGH);
    digitalWrite(A1, HIGH);
    digitalWrite(A7, HIGH);
    digitalWrite(D2, HIGH);
    digitalWrite(D6, HIGH);
    digitalWrite(D7, HIGH);
    delay(10);
    digitalWrite(A0, LOW);
    digitalWrite(A1, LOW);
    digitalWrite(A7, LOW);
    digitalWrite(D2, LOW);
    digitalWrite(D7, LOW);

    sensor0.init();
    sensor0.setAddress(0x30);
    sensor0.setTimeout(1000);
    delay(10);
    digitalWrite(D7, HIGH);
    sensor1.init();
    sensor1.setAddress(0x31);
    sensor1.setTimeout(1000);
    delay(10);
    digitalWrite(A7, HIGH);
    sensor2.init();
    sensor2.setAddress(0x32);
    sensor2.setTimeout(1000);
    delay(10);
    digitalWrite(D2, HIGH);
    sensor3.init();
    sensor3.setAddress(0x33);
    sensor3.setTimeout(1000);
    delay(10);
    digitalWrite(A1, HIGH);
    sensor4.init();
    sensor4.setAddress(0x34);
    sensor4.setTimeout(1000);
    delay(10);
    digitalWrite(A0, HIGH);
    sensor5.init();
    sensor5.setAddress(0x35);
    sensor5.setTimeout(1000);
    delay(10);

    sensor0.startContinuous(10);
    sensor1.startContinuous(10);
    sensor2.startContinuous(10);
    sensor3.startContinuous(10);
    sensor4.startContinuous(10);
    sensor5.startContinuous(10);
}

//Setting up pins for line sensor, remote start, and ESCs to right modes
// INITIALIZE THE ENCODERS HERE
void others_init() {
    // Line sensors
    pinMode(FL, INPUT);
    pinMode(FR, INPUT);
    pinMode(BL, INPUT);
    pinMode(BR, INPUT);
    // Remote switch
    pinMode(RS, INPUT);
    // Car ESCs
    pinMode(Lmotor, OUTPUT);
    pinMode(Rmotor, OUTPUT);

    // encoders
    pinMode(Lencoder, INPUT);
    pinMode(Rencoder, INPUT);

    // ***Remove for competition***
    // ***Serial monitor***
    //Serial.begin(9600);	// *** need to modify before comp ***
}

//Setting up line sensor interrupts so they can be triggered
// ADD THE INTERRUPT FOR THE ENCODERS HERE
void interrupt_init() {
    // Line sensors
    // attachInterrupt(FL, FLISR, CHANGE);
    // attachInterrupt(FR, FRISR, CHANGE);
    // attachInterrupt(BL, BLISR, CHANGE);
    // attachInterrupt(BR, BRISR, CHANGE);
    // Remote switch
    attachInterrupt(RS, RSISR, CHANGE);

    // Encoders
    attachInterrupt(Lencoder, LencoderISR, RISING);
    attachInterrupt(Rencoder, RencoderISR, RISING);

    // NOT USED
    // Accel software timer
    // Timer accelTimer(PRINT_SAMPLE_PERIOD, getAccel);
    // accelTimer.start();
}

// initialize prevFlag to be in unison with cur
void line_init() {
    // moving = true;
    prevFlag = millis();
}

//Wait 5 seconds before moving as per the rules
void robot_init() {
    while(RSflag == false) {    // initial LOW
        Serial.println("Waiting for Start");
    }
    // Serial.println("Starting in 5 seconds...");
    // delay(5000);
    Serial.println("GO!");
}

// setting the startup sequence
void startUp() {
    if (!prevFlagSet) {
        prevFlag = cur;
        prevFlagSet = true;
    } else {
        moveState(2);
    }
}
