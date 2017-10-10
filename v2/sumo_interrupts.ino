// bare-bones setup of the interrupts for the sumo robot

//***PCB MODIFICATION***
// for blls, jump D13 to A6 (can pick whatever unused digital pin)
// for brls, jump D12 to A7 (can pick whatever unused digital pin)

//***Additional notes***
// check if these interrupts interfere with writeMicrseconds()
// in the ISRs, you can branch to other subroutines and perform basic calculations,
// but in general try not to spend too much time inside the ISR itself.

//part of PCINT2_vect
const int leftHall = 2;   // PCINT 18
const int rightHall = 3;  // PCINT 19

//part of PCINT2_vect
const int startModule = 4; // PCINT 20

//part of PCINT1_vect
short flls = A2; //front-left line sensor, PCINT 10
short frls = A3; //front-right line sensor, PCINT 11

//part of PCINT0_vect
short blls = 13; //back-left line sensor, PCINT 5
short brls = 12; //back-right line sensor, PCINT 4

unsigned int flag; //used to see which sensor was triggered

void setup() {
  Serial.begin(9600);
  
  // Hardware Interrupts for the left and right hall sensors. Upon a change in pin state, enter respective ISRs.
  attachInterrupt(digitalPinToInterrupt(leftHall), leftHallRead, CHANGE);
  attachInterrupt(digitalPinToInterrupt(rightHall), rightHallRead, CHANGE);

  // Pin change interrupts 
  PCICR |= B00000111;   // Enable PCINT0, PCINT1, PCINT2 (all interrupt groups)
  PCMSK2 |= B00010000;  // For each PCINT_vect, enable the PCINTs mentioned above with PCMSK
  PCMSK1 |= B00001100;
  PCMSK0 |= B00110000;
}

void loop() {
  switch(flag) {
    case 1:
      Serial.println("Left Hall");
      break;
    case 2:
      Serial.println("Right Hall");
      break;
    case 3:
      Serial.println("Start Module");
      break;
    case 4:
      Serial.println("Front Left Line Sensor");
      break;
    case 5:
      Serial.println("front-right line sensor");
      break;
    case 6:
      Serial.println("back-left line sensor"); 
      break;
    case 7:
      Serial.println("back right line sensor"); 
      break;
    case 8:
      Serial.println("Turn off");
      break;
    default:
      Serial.println("No/false interrupt detected");
      break;
  }
}

void leftHallRead() {
  flag = 1;
}

void rightHallRead() {
  flag = 2;
}

// For the pin-change interrupts with more than one trigger source, the conditionals help
// the program determine which pin changed state.

ISR (PCINT0_vect) {
  // line sensors go low on a white line
  if(!digitalRead(blls)) flag = 6;
  if(!digitalRead(brls)) flag = 7;
}

ISR (PCINT1_vect) {
  // line sensors go low on a white line
  if(!digitalRead(frls)) flag = 4;
  if(!digitalRead(flls)) flag = 5;
}

ISR (PCINT2_vect) {
  if(digitalRead(startModule)) {
    // turn on
    flag = 3;
  } else if(!digitalRead(startModule)) {
    // turn off
    flag = 8;
  } else {
    flag = 9;
  }
}
