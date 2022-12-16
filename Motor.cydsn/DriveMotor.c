#include "DriveMotor.h"
#include "project.h"
#include <stdio.h>

#define BACKWARDS 0
#define FORWARDS 1

DriveMotor DriveMotor_constructor(void){
    //Start and stop PWM. This correct an error that accured 
    //when using the drive-method the first time
    PWM_Drive_Start();
    PWM_Drive_1_Start();
    
    DriveMotor driveMotor;
    driveMotor.dir = 1;
    driveMotor.speed = 0;
    driveMotor.slowMode = false;
    driveMotor.drive = DriveMotor_drive;
    driveMotor.exercute = DriveMotor_exercute;
    driveMotor.setSlowMode = DriveMotor_setSlowMode;
    driveMotor.stop = DriveMotor_stop;
    
    return driveMotor;
}

void DriveMotor_drive(DriveMotor *self, int speed){
    Timer_drive_WriteCounter(6000000);
    Timer_drive_Start();
    uint8_t tempDir = (speed<0?BACKWARDS:FORWARDS);
    speed=(speed<0?-speed:speed);
    if(speed == self->speed && tempDir == self->dir){
        return; //Same as current speed and dir - Do nothing
    } else{
        self->dir = tempDir;
        if(tempDir == BACKWARDS){
            //Never reverse with more than 60% speed
            speed=(speed>60?60:speed);
            self->speed = speed;
        } else{
            //Secure speed is never > 100%
            speed=(speed>100 ? 100:speed);
            self->speed=speed;
        }
        DriveMotor_exercute(self);  //Execute
    }
    
}

void DriveMotor_exercute(DriveMotor *self){
    if(self->dir == 1){
        //Drive forward
        PWM_Drive_1_WriteCompare(100);
        if(self->slowMode == true && self->speed != 0){
            PWM_Drive_WriteCompare(100-50);
        } else{
            PWM_Drive_WriteCompare(100-self->speed);
        }
    } else {
        //Reverse
        PWM_Drive_WriteCompare(100);
        if(self->slowMode == true && self->speed != 0){
            PWM_Drive_1_WriteCompare(100-35);
        } else {
            PWM_Drive_1_WriteCompare(100-self->speed);
        }
    }
}

void DriveMotor_setSlowMode(DriveMotor *self, bool mode){
    self->slowMode = mode;
    DriveMotor_exercute(self);
}

void DriveMotor_stop(DriveMotor *self){
    self->speed=0;
    PWM_Drive_WriteCompare(100);
    PWM_Drive_1_WriteCompare(100);
}