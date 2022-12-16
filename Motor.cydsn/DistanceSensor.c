#include "project.h"
#include "DistanceSensor.h"
#include <stdio.h>

DistanceSensor DistanceSensor_constructor(){
    PWM_distance_1_Init();
    PWM_distance_2_Init();
    CyDelayFreq(10000000);
    
    DistanceSensor tempDistanceSensor;
    tempDistanceSensor.distance = 0;
    tempDistanceSensor.startDistanceCalc = DistanceSensor_startDistanceCalc;
    tempDistanceSensor.calculateDistance = DistanceSensor_calculateDistance;
    tempDistanceSensor.getDistance = DistanceSensor_getDistance;
    tempDistanceSensor.isSend = false;
    tempDistanceSensor.sendTime = 0;
    return tempDistanceSensor;
}

void DistanceSensor_startDistanceCalc(DistanceSensor* self){
    Counter_Stop();         //Secures the counter to start over, even when it never gets a return signal
    Counter_WriteCounter(0);//To start counter from 0
    Counter_Start();        //Start the counter
    self->isSend = true;
    self->sendTime = Timer_ReadCounter();
    PWM_distance_1_Start(); //Start first PWM Signal
    CyDelayCycles(130);      //x cycles delay with 10MHz freq
    PWM_distance_2_Start(); //Start second PWM signal
    CyDelayCycles(5950);    //Delay to send 10 full PWM cycles
    PWM_distance_1_Stop();  //Stop fist PWM
    PWM_distance_2_Stop();  //Stop second PWM
    PWM_distance_1_initVar = 0; //Next time PWM starts it will init again. This secures the excact same PWM signal each time
    PWM_distance_2_initVar = 0; // -||-
}

void DistanceSensor_calculateDistance(DistanceSensor *self, uint16_t counterValue){
    Counter_Stop(); //Stop the counter
    uint16_t value = 65535 - counterValue; //Counter is a downcounter from 65535
    uint32_t time = value * (0.167/2);  //0.167 = 1/6MHz in us. divided by two to get one way time
    uint8_t distance = 0.0343*time-3;  //0.0343 = sound of speed [cm/us]. -3 to corret for the speaker itself   
    self->distance = distance;  //Store the distance
}

uint8_t DistanceSensor_getDistance(DistanceSensor* self){
    return self->distance;  //Return distance
}

