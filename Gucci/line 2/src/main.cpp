// A test of the eFLL fuzzy library
// Utilizes the VL53L0X sensor and Car ESCs

#include <Particle.h>

#include <FuzzyRule.h>
#include <FuzzyComposition.h>
#include <Fuzzy.h>
#include <FuzzyRuleConsequent.h>
#include <FuzzyOutput.h>
#include <FuzzyInput.h>
#include <FuzzyIO.h>
#include <FuzzySet.h>
#include <FuzzyRuleAntecedent.h>

#include <thresholds.h>

#include <movement.h>

#include <logic.h>

#include <sensors.h>

#include <init.h>

#include <VL53L0X.h>

#if !defined(PARTICLE)
#  include <Wire.h>
#endif

//n#include <sensors.h>

// #include <init.h>

// Particle Photon settings. Should only be in 1 place in all the files
SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);

void setup() {
    Serial.begin(9600);
    tof_init();			// ToF and I2C

    // accel_init();		// accelerometer NOT USED

    others_init();		// line, remote, esc, ***printing***
    interrupt_init();	// interrupts for lines, remote, and eventually encoders/current
    ESC_init();			// Car ESCs
    fuzzy_init();		// Fuzzy library************
    // Serial.begin(9600); // *** need to modify before comp ***
    // pinMode(RS, INPUT);

    RGB.control(true); 	// take control of the on-board LED for debugging

    robot_init();   // makes the bot wait 5s before starting as per the rules
    line_init();		// initialize line variables
}

// -----------------------------------------------------------
// ------------------Runtime functions------------------------
// -----------------------------------------------------------
// The functions that robot loops through during the match
void loop(){
  cur = millis();           // update timer
  // moveState(0);
  //RSflag = true;
  //start = false;

  if (start) {
    startUp(); //If the game has just begun move the robot forward(?) a bit
  }

  if (!start) {
    checkLine(); //Make sure we're not too close to a line
    //checkEncoders();


    // Serial.print(decision);
    // Serial.print(" | ");
    // Serial.print("RS: ");
    // Serial.print(RS_distance);
    // Serial.print(" | ");
    // Serial.print("RR: ");
    // Serial.print(RR_distance);
    // Serial.print(" | ");
    // Serial.print("RM: ");
    // Serial.print(RM_distance);
    // Serial.print(" | ");
    // Serial.print("LM: ");
    // Serial.print(LM_distance);
    // Serial.print(" | ");
    // Serial.print("LL: ");
    // Serial.print(LL_distance);
    // Serial.print(" | ");
    // Serial.print("LS: ");
    // Serial.print(LS_distance);
    // Serial.println();
    // delay(500);
    // getToF();


  	if (!prevFlagSet) {	// If we didn't see a line do Fuzzy logic to determine move inputs
      // while(1) {
      //   stop();
      // }
  		getToF();
      // Serial.println("Cool");
  		// doFuzzy();
    //   Serial.print(" | ");
       // Serial.print(decision);
  	}
  }


  // Serial.print("cur: ");
  // Serial.println(cur);
  // Serial.print("prevFlag: ");
  // Serial.println(prevFlag);
  // Serial.println(start);

  // Serial.println("Cool");
	checkSwitch(); //See if we hit the off switch
  Serial.print(count1 + " " + count2);
  move(1, R_command, R_dir); //Move based off what info we got from doFuzzy()
  move(2, L_command, L_dir);
}
