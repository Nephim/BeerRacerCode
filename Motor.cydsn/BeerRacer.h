#pragma once
#include "project.h"
#include "DriveMotor.h"
#include "TurnMotor.h"
#include "RPIComm.h"
#include "DistanceSensor.h"
#include "BeerCounter.h"
#include <stdbool.h>

typedef struct BeerRacer BeerRacer;

struct BeerRacer{
    DriveMotor *driveMotor;
    TurnMotor *turnMotor;
    DistanceSensor *distanceSensor;
    RPIComm *rpiComm;
    BeerCounter *beerCounter;
    uint8_t lastDistance;
    uint8_t lastAmountOfBeers;
    bool slowMode;
    bool reverseOnly;
    void (*byteRecieved)(BeerRacer*, uint8_t byte);
    void (*setSlowMode)(BeerRacer*, bool mode);
    void (*slowDown)(BeerRacer*);
    void (*sendDataToUI)(BeerRacer*);
};

//DriveMotor constructor
BeerRacer BeerRacer_constructor(DriveMotor*, TurnMotor*, DistanceSensor*, RPIComm*, BeerCounter*);

void BeerRacer_byteRecieved(BeerRacer*, uint8_t byte);
void BeerRacer_setSlowMode(BeerRacer*, bool mode);
void BeerRacer_slowDown(BeerRacer*);
void BeerRacer_sendDataToUI(BeerRacer*);


