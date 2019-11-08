#include "kirbi.h"
#include "Arduino.h"

#define LEFT_LIDAR_SERIAL Serial1
#define RIGHT_LIDAR_SERIAL Serial4

#define ESC_SERIAL Serial5
#define ESC_ADDRESS 128
#define ESC_CHECKSUM 0b01111111

#define MAX_DIST //Whatever the lidar returns when it doesn't see anything

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
    int[] curr_distances = distances.getFront();
    if (curr_distances[2] != MAX_DIST) {
        if (curr_distances[3] != MAX_DIST) {
            if (curr_distances[1] == 0 && curr_distances[4] == 0) {
                return SLAMMY_WHAMMY; //temp but kinda what we want
            } else if (curr_distances[1] == 1 && curr_distances[4] == 0) {
                return ADJUST_LEFT;
            } else if (curr_distances[1] == 0 && curr_distances[4] == 1) {
                return ADJUST_RIGHT;
            } else {
                return SLAMMY_WHAMMY; //either we're seeing them on all 4 middle sensors or something weird is happening
            }
        }

    } else if (curr_distances[3] != MAX_DIST) {

    } else if (lastState == WAIT_FOR_START) {
        return STARTUP;
    }

}

void drive(int left, int right, bool left_reverse, bool right_reverse) {
    left = left*left_multi;
    right = right*right_multi;
    ESC_SERIAL.write(ESC_ADDRESS);
    ESC_SERIAL.write(left_reverse);
    ESC_SERIAL.write(left);
    ESC_SERIAL.write((ESC_ADDRESS+left_reverse+left)&ESC_CHECKSUM);
    ESC_SERIAL.write(ESC_ADDRESS);
    ESC_SERIAL.write(right_reverse);
    ESC_SERIAL.write(right);
    ESC_SERIAL.write((ESC_ADDRESS+right_reverse+right)&ESC_CHECKSUM);
}

/**
 * INTERRUPT METHODS
**/
void do_line_action_left() {
    //TODO: implement
    drive(x, y, true, true);
}
void do_line_action_right() {
    //TODO: implement
    drive(y, x, true, true);
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
     LEFT_LIDAR_SERIAL.begin(115200);
     LEFT_LIDAR_SERIAL.write(configUART, 5);
     LEFT_LIDAR_SERIAL.write(configOutput, 5);
     RIGHT_LIDAR_SERIAL.begin(115200);
     RIGHT_LIDAR_SERIAL.write(configUART, 5);
     RIGHT_LIDAR_SERIAL.write(configOutput, 5);
     // TODO: the rest of the distance sensors
 }

 void setup_current() {
    // ignore for now
 }

 void setup_motors(){
     // TODO: implement
    left_multi = 1;
    right_multi = 1;
    ESC_SERIAL.begin(115200);
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
    //TODO: implement?
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
    return dist;
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