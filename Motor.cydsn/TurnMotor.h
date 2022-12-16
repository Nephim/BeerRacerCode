#pragma once
#include "project.h"

typedef struct TurnMotor TurnMotor;

struct TurnMotor{
    uint8_t dir;
    uint8_t speed;
    void (*turn)(TurnMotor*, uint8_t);
    void (*stop)(void);
};

//DriveMotor constructor
TurnMotor TurnMotor_constructor();

//Methods for TurnMotor
void TurnMotor_turn(TurnMotor *self, uint8_t dir);
void TurnMotor_stop();
