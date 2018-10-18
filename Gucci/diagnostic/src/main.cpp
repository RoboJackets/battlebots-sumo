//Attempt to test ToF Sensors

#include <Particle.h>
#include <VL53L0X.h>

#if !defined(PARTICLE)
#  include <Wire.h>
#endif

SYSTEM_THREAD(ENABLED);
SYSTEM_MODE(MANUAL);

VL53L0X sensor0;

void setup() {
    Serial.begin(9600);
    // Wire.begin();

    // sensor.init();
    // sensor.setTimeout(500);

    // Start continuous back-to-back mode (take readings as
    // fast as possible).  To use continuous timed mode
    // instead, provide a desired inter-measurement period in
    // ms (e.g. sensor.startContinuous(100)).
    // sensor.startContinuous(10);


    // Serial.begin(9600);
    // pinMode(D0, OUTPUT);

    // digitalWrite(D0, LOW);

    // delay(500);

    // Wire.setSpeed(CLOCK_SPEED_100KHZ);  // need to initialize i2c protocol after driving shdn pins low
    // Wire.begin();

    // delay(10);
    // digitalWrite(D0, HIGH);

    // sensor0.init();
    // sensor0.setAddress(0x30);
    // sensor0.setTimeout(1000);
    // delay(10);

    // sensor0.startContinuous(10);
}

void loop() {
    // distance = sensor0.readRangeContinuousMillimeters();
    // Serial.print(distance);
    Serial.println("abc");
    delay(1000);

    // Serial.print(sensor.readRangeContinuousMillimeters());
    // if (sensor.timeoutOccurred()) { Serial.print(" TIMEOUT"); }

    // Serial.println();
}