//
// Created by Logan Schick on 9/6/2018.
//

#ifndef SUMO_INIT_H
#define SUMO_INIT_H

void FLISR();

void FRISR();

void BLISR();

void BRISR();

void RSISR();

void tof_init();

void others_init();

//void ESC_init();

void interrupt_init();

void line_init();

void robot_init();

void startUp();

extern int count1;
extern int count2;

#endif //SUMO_INIT_H
