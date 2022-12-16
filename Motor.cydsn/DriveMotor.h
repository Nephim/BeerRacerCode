#pragma once
#include "project.h"
#include <stdbool.h>

typedef struct DriveMotor DriveMotor;

struct DriveMotor{
    uint8_t dir;
    uint8_t speed;
    bool slowMode;
    void (*drive)(DriveMotor*, int speed);
    void (*exercute)(DriveMotor*);
    void (*setSlowMode)(DriveMotor*, bool mode);
    void (*stop)(DriveMotor*);
};

//DriveMotor constructor
DriveMotor DriveMotor_constructor(void);

void DriveMotor_drive(DriveMotor *self, int speed);
void DriveMotor_exercute(DriveMotor *self);
void DriveMotor_setSlowMode(DriveMotor *self, bool mode);
void DriveMotor_stop(DriveMotor *self);


