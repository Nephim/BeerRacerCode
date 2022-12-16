#include "TurnMotor.h"
#include "project.h"

TurnMotor TurnMotor_constructor(){
    //Start and stop PWM to prevent failure when turning first time.
    PWM_Turn_Start();
    PWM_Turn_1_Start();
    
    
    TurnMotor turnMotor;
    turnMotor.dir = 1;      //Draw the least current while still turn the car
    turnMotor.speed = 5;    //Set standart value
    turnMotor.turn = TurnMotor_turn;
    turnMotor.stop = TurnMotor_stop;
    
    return turnMotor;
}
void TurnMotor_turn(TurnMotor *self, uint8_t dir){
    self->dir=(dir>0?1:0);
    if(self->dir == 1){
        //Turn
        PWM_Turn_WriteCompare(10-self->speed);
        PWM_Turn_1_WriteCompare(10);
    } else {
        //Turn
        PWM_Turn_WriteCompare(10);
        PWM_Turn_1_WriteCompare(10-self->speed);
    }
}

void TurnMotor_stop(void){
    //Stop the turn
    Timer_turn_Stop();
    PWM_Turn_WriteCompare(10);
    PWM_Turn_1_WriteCompare(10);
}