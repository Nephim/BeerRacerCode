#pragma once
#include "project.h"

uint8_t commandBits = 0b11100000;

/*MotorDriveProtokol*/
uint8_t motorToDrive = 0b00100000;  //Drive command = 0b001xxxxx
uint8_t speedBits = 0b00001111;     //Bits to define the speed of the car 
uint8_t driveDirectionBit = 0b00010000;  //Bit to define the direction og the car
uint8_t motorToStop = 0b01000000;   //Stop command = 0b010xxxxx;

/*TurnMotorProtokol*/
uint8_t motorToTurn = 0b01100000;
uint8_t turnDirectionBit = 0b00000001;
uint8_t stopTurn = 0b10000000;

/*Mode Protocol*/
uint8_t changeMode = 0b11100000;
uint8_t modebit = 0b00000001;

/*Send Protocol*/
uint8_t distanceByte = 0b10000000;
uint8_t beerByte = 0b00000000;