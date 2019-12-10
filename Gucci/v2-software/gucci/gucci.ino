#include <SD.h>
#include "gucci.h"
#include "ICM20948.h"

State curr_state;
State last_state;
long startTime;
int useSD = 0;
ICM20948 imu(Wire, (uint8_t)0x68);

void setup()
{
      Serial.println("starting setup");
      setup_imu();
      Serial.println("setup 1");
      setup_distance();
      Serial.println("setup 2");
//      setup_current();
      Serial.println("setup 3");
      setup_motors();
      Serial.println("setup 4");
//      setup_encoders();
      Serial.println("setup 5");
      setup_line();
      Serial.println("setup 6");
      setup_remote();
      Serial.println("Setting up IMU...");
      pinMode(9, OUTPUT); // IMU address pin
      digitalWrite(9, LOW);
      Wire.setSDA(4);
      Wire.setSCL(3);
      Wire.begin();
      Serial.println("Setting up SD card...");
      if (SD.begin(BUILTIN_SDCARD)) {
          Serial.println("Data logging initialized.");
          useSD = 1;
          File file = SD.open("guccii.csv", FILE_WRITE);
          if (file) {
              file.println("time,state,x,y,z,rx,ry,rz");
              file.close();
          } else {
              Serial.println("Could not open file.");
          }
      } else {
          Serial.println("SD card is likely not present, so not data logging.");
      }

      Serial.println("finished setup");
      startTime = millis(); // for testing purposes
}

void loop()
{
      if (digitalRead(REMOTE_PIN))
      {
            Serial.println("doing startup action");
            do_startup_action();
            while (digitalRead(REMOTE_PIN))
            {
                  if (useSD) {
                        imu.readSensor();
                        String line = "";
                        line += String(millis());
                        line += ",";
                        line += String(curr_state);
                        line += ",";
                        line += String(imu.getAccelX_mss(), 6);
                        line += ",";
                        line += String(imu.getAccelY_mss(), 6);
                        line += ",";
                        line += String(imu.getAccelZ_mss(), 6);
                        line += ",";
                        line += String(imu.getGyroX_rads(), 6);
                        line += ",";
                        line += String(imu.getGyroY_rads(), 6);
                        line += ",";
                        line += String(imu.getGyroZ_rads(), 6);

                        File file = SD.open("guccii.csv", FILE_WRITE);
                        if (file) {
                              file.println(line);
                              file.close();
                        } else {
                              Serial.println("Could not open file.");
                        }
                  }

                  if (get_line_flag())
                  {
                        drive(-60, -60);
                        Serial.println(digitalRead(LEFT_INT_LINE));
                        Serial.println(digitalRead(RIGHT_INT_LINE));
                        Serial.println(get_line_flag());
                        Serial.println("Line");
                        delay(200);
//                        drive(-60, 60);
//                        delay(500);

                  }
                  else
                  {
                         curr_state = state_machine();
                        curr_state = SLAMMY_WHAMMY;
                        // Serial.println(digitalRead(LEFT_INT_LINE));
                        // Serial.println(digitalRead(RIGHT_INT_LINE));
                        // Serial.println(get_line_flag());
                        switch (curr_state)
                        {
                        case SEARCH:
                              Serial.println("SEARCH");
                              drive(-70, 70);
                              break;
                        case ADJUST_1_LEFT:
                              Serial.println("A1L");
                              drive(70, -70);
                              break;
                        case ADJUST_1_RIGHT:
                              Serial.println("A1R");
                              drive(-70, 70);
                              break;
                        case ADJUST_2_LEFT:
                              Serial.println("A2L");
                              drive(70, -70);
                              break;
                        case ADJUST_2_RIGHT:
                              Serial.println("A2R");
                              drive(-70, 70);
                              break;
                        case ADJUST_3_LEFT:
                              Serial.println("A3L");
                              drive(70, -70);
                              break;
                        case ADJUST_3_RIGHT:
                              Serial.println("A3R");
                              drive(-70, 70);
                              break;
                        case ADJUST_4_LEFT:
                              Serial.println("A4L");
                              drive(70, -70);
                              break;
                        case ADJUST_4_RIGHT:
                              Serial.println("A4R");
                              drive(-70, 70);
                              break;
                        case SLAMMY_WHAMMY:
                              Serial.println("Come on and slam");
                              drive(100, 100);
                              break;
                        case MEGA_SLAMMY_WHAMMY:
                              Serial.println("Gucci used Skull Bash. It was super effective!");
                              drive(127, 127);
                              break;
                        case PANIC_HIT:
                              // drive(40, 40);
                              Serial.println("We've been hit cap");
                              break;
                        case PANIC_FIRE:
                              drive(0, 0);
                              Serial.println("Oh heck");
                              break;
                        }
                  }
            }
            Serial.println("Stop signal received");
            drive(0, 0);
      }
}
