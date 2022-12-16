#include "project.h"
#include "BeerRacer.h"
#include "protocol.h"
#include <stdio.h>

BeerRacer BeerRacer_constructor(DriveMotor* driveMotorPTR, TurnMotor* turnMotorPTR, DistanceSensor* distanceSensorPTR, RPIComm* rpiCommPTR, BeerCounter* beerCounterPTR){
    //Create and return object
    BeerRacer beerRacer;
    beerRacer.driveMotor = driveMotorPTR;
    beerRacer.turnMotor = turnMotorPTR;
    beerRacer.distanceSensor = distanceSensorPTR;
    beerRacer.rpiComm = rpiCommPTR;
    beerRacer.beerCounter = beerCounterPTR;
    beerRacer.byteRecieved = BeerRacer_byteRecieved;
    beerRacer.setSlowMode = BeerRacer_setSlowMode;
    beerRacer.slowDown = BeerRacer_slowDown;
    beerRacer.sendDataToUI = BeerRacer_sendDataToUI;
    beerRacer.slowMode = false;
    beerRacer.reverseOnly = false;
    beerRacer.lastDistance = 0;
    beerRacer.lastAmountOfBeers = 0;
    return beerRacer;
}

void BeerRacer_byteRecieved(BeerRacer *self, uint8_t recievedByte){
    if((recievedByte & commandBits) == motorToDrive){ //Motor drive command
        int speed = (recievedByte & speedBits); //Get speed
        speed = speed*10;   //Seed times 10 (maks-speed = 100)
        if((recievedByte&driveDirectionBit) != 0){
            speed = -speed; //If direction is backwards, set speed negative
        }
        if(speed > 0 && self->reverseOnly == true){ 
            self->driveMotor->stop(self->driveMotor);
        } else {
            self->driveMotor->drive(self->driveMotor, speed);
        }
    } else if((recievedByte & commandBits) == motorToStop){
        self->driveMotor->stop(self->driveMotor); //Stop the car
    } else if((recievedByte & commandBits) == motorToTurn){
        uint8_t direction = (recievedByte&turnDirectionBit);
        self->turnMotor->turn(self->turnMotor, direction);//Turn the front wheels
    } else if((recievedByte & commandBits) == stopTurn){
        self->turnMotor->stop();    //Stop the turn
    } else if((recievedByte & commandBits) == changeMode){
        self->setSlowMode(self, (recievedByte & modebit));  //Change mode
    } else {
    }
}

void BeerRacer_setSlowMode(BeerRacer *self, bool mode){
    if(mode != self->slowMode){
        //If slowmode is difrent, change mode
        self->driveMotor->setSlowMode(self->driveMotor, mode);
        self->slowMode = mode;
    }
}     

void BeerRacer_slowDown(BeerRacer *self){
    volatile uint8_t distance = self->distanceSensor->getDistance(self->distanceSensor);
    if(distance < 10 && distance != 0){
        if(self->reverseOnly == true){
        } else {
            //Set reverse only if car is too close to object
            self->driveMotor->stop(self->driveMotor);
            self->reverseOnly = true;
        }
    } else if( distance < 30 && distance != 0){
        //Slow down the car if distance is < 30cm
        self->driveMotor->setSlowMode(self->driveMotor, true);
        self->reverseOnly = false;
    } else {
        //Normal mode/racemode
        self->driveMotor->setSlowMode(self->driveMotor, false);
        self->reverseOnly = false;
    }
}

void BeerRacer_sendDataToUI(BeerRacer *self){
    uint8_t distance = self->distanceSensor->getDistance(self->distanceSensor);
    if(distance != self->lastDistance){
        //If distance is diffrent from last distance. Send to UI
        self->rpiComm->sendByte(distanceByte | distance);
        self->lastDistance = distance;
    }
    
    uint8_t amountOfBeers = self->beerCounter->getAmount(self->beerCounter);
    if(amountOfBeers != self->lastAmountOfBeers){
        //If amount of beers is diffrent from last distance. Send to UI
        self->rpiComm->sendByte(beerByte | amountOfBeers);
        self->lastAmountOfBeers = amountOfBeers;
    }
}