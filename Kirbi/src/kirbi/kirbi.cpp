#include "kirbi.h"
#include "Arduino.h"

#define LEFT_LIDAR_SERIAL Serial1
#define RIGHT_LIDAR_SERIAL Serial2

x_accel = new CircularArray<>(8);
y_accel = new CircularArray<>(8);
distances = new CircularArray<>(8);
current = new CircularArray<>(8);

enum State {
    SEARCH_LEFT,
    SEARCH_RIGHT,
    ADJUST_LEFT,
    ADJUST_RIGHT,
    SLAMMY_WHAMMY,
    PANIC_HIT,
    PANIC_FIRE,

    WAIT_FOR_START,
    STARTUP
};


State state_machine(State lastState) {
    curr_time = millis();
    get_distances();
    get_current();
    if (curr_time - prev_time > check_accel) {
        prev_time = millis();
        get_accel();
    }
    if() {

    } else if () {

    } else if (lastState == WAIT_FOR_START) {
        return STARTUP;
    }

}

void drive(int left, int right) {
    int true_left = left*left_multi;
    int true_right = right*right_multi;
    // TODO: actually set ECSs
}

/**
 * INTERRUPT METHODS
**/
void do_line_action_left() {
    //TODO: implement
    drive(-x, -y);
}
void do_line_action_right() {
    //TODO: implement
    drive(-y, -x);
}

void increment_encoder_right() {
    right_encoder++;
}

void increment_encoder_left() {
    left_encoder++;
}

/**
 * Method for startup action
**/
void do_startup_action() {
    //TODO: implement
}

/**
 * SETUP METHODS
**/
 void setup_imu() {
    pinMode(9, OUTPUT);
    digitalWrite(9, LOW);
    Wire2.begin();
    Wire2.setSDA(8);
    Wire2.setSCL(7);
    icm.begin()
    icm.disableDataReadyInterrupt();
    icm.configAccel(ICM20948::ACCEL_RANGE_2G, ICM20948::ACCEL_DLPF_BANDWIDTH_6HZ);
    icm.configGyro(ICM20948::GYRO_RANGE_250DPS, ICM209480::GYRO_DLPF_BANDWIDTH_6HZ);
 }

 void setup_distance() {
     /* lidar setup */
     LIDAR_SERIAL.begin(115200);
     LIDAR_SERIAL.write(configUART, 5);
     LIDAR_SERIAL.write(configOutput, 5);
     // TODO: the rest of the distance sensors
 }

 void setup_current() {
    // ignore for now
 }

 void setup_motors(){
     // TODO: implement
    left_multi = 1;
    right_multi = 1;
 }

/**
 * SENSOR READ METHODS
**/
void get_accel() {
    //TODO: implement
    x_accel.add(icm.getAccelX_mss());
    y_accel.add(icm.getAccelY_mss());
}
void get_gyro() {
    //TODO: implement
}
void get_distances() {
    //TODO: implement
    int[] dist = int[6];
    dist[0] = digitalRead(omron0);
    dist[1] = digitalRead(omron1);
    dist[2] = read_lidar(LEFT_LIDAR_SERIAL);
    dist[3] = read_lidar(RIGHT_LIDAR_SERIAL);
    dist[4] = digitalRead(omron4);
    dist[5] = digitalRead(omron5);
    distances.add(dist);
}
int read_lidar(Serial s) {
    byte[] bytes = byte[9];
    s.readBytes(bytes, 9);
    int dist_low = 0;
    while (dist_low < 9) {
        if (bytes[dist_low] == 0x89) {
            if (dist_low == 0) {
                if (bytes[8] == 0x89) {
                    return bytes[1] + (bytes[2] << 8);
                }
            } else if (dist_low == 7) {
                if (bytes[8] == 0x89) {
                    return bytes[0] + (bytes[1] << 8);
                }
            } else {
                if (bytes[++dist_low] == 0x89) {
                    return bytes[++dist_low] + (bytes[++dist_low] << 8);
                }
            }
        }
        dist_low++;
    }
    return read_lidar(s);
}
void get_current() {
    // ignore for now
}

/**
 * OTHER
**/
void balance_motors() {
    if (left_encoder > right_encoder) {
        left_multi = right_encoder/left_encoder;
        right_multi = 1;
    } else {
        right_multi = left_encoder/right_encoder;
        left_multi = 1;
    }
}