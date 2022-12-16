#include "project.h"
#include "DriveMotor.h"
#include "TurnMotor.h"
#include "DistanceSensor.h"
#include "RPIComm.h"
#include "BeerRacer.h"
#include "BeerCounter.h"
#include <stdio.h>


CY_ISR_PROTO(ISR_UART_rx_handler);
CY_ISR_PROTO(ISR_distance_handler);
CY_ISR_PROTO(timer_interrupt_handler);
void handleByteReceived(uint8_t byteReceived);

//Make global structs.
BeerRacer beerRacer;
DriveMotor driveMotor;
TurnMotor turnMotor;
DistanceSensor distanceSensor;
RPIComm rpiComm;
BeerCounter beerCounter;
volatile uint8_t timer_int_count = 0;

int main(void)
{
    //Create objects from the global structs
    driveMotor = DriveMotor_constructor();
    turnMotor = TurnMotor_constructor();
    distanceSensor = DistanceSensor_constructor();
    rpiComm = RPIComm_constructor();
    beerCounter = BeerCounter_constructor();
    beerRacer = BeerRacer_constructor(&driveMotor, &turnMotor, &distanceSensor, &rpiComm, &beerCounter);

    Timer_Start();  //Start timer for calculating distance and sending data to UI
    CyGlobalIntEnable; /* Enable global interrupts. */
    isr_UART_rx_StartEx(ISR_UART_rx_handler);   //Combine the ISR-pin with the ISR-function
    distance_isr_StartEx(ISR_distance_handler); //Combine the ISR-pin with the ISR-function
    timer_interrupt_StartEx(timer_interrupt_handler);   //Combine the ISR with the ISR-function

    for(;;)
    {
    }
}

//This function is taken direcly from the GFV course.
CY_ISR(ISR_UART_rx_handler){
    uint8_t bytesToRead = UART_GetRxBufferSize();
    while (bytesToRead > 0) {
        uint8_t byteReceived = UART_ReadRxData();
        //handleByteReceived(byteReceived); //Testin
        beerRacer.byteRecieved(&beerRacer, byteReceived);   //Tis function when not testing
        bytesToRead--;
    }
    UART_ClearRxBuffer();
}

//This function is taken direclty from the GFV Course. This function is for testing purpose
void handleByteReceived(uint8_t byteReceived) {
    uint8_t testByte = 0b00000000;
    switch(byteReceived){
        case 's':
            testByte = 0b00111111;
            break;
        case 'w':
            testByte = 0b00101111;
            break;    
        case 'x':
            testByte = 0b01000000;
            break;
        case 'a':
            testByte = 0b01100001;
            break;
        case 'd':
            testByte = 0b01100000;
            break;
        case 'q':
            testByte = 0b10000000;
            break;
        case 'l':
            testByte = 0b11100001;
            break;
        case 'o':
            testByte = 0b11100000;
            break;
        case 'p':
            distanceSensor.startDistanceCalc(&distanceSensor);
            break;
    }
    beerRacer.byteRecieved(&beerRacer, testByte);
}

CY_ISR(ISR_distance_handler){
    //Interrupt from distance sensor reciever
    DistanceSensor_calculateDistance(&distanceSensor, Counter_ReadCounter()); //Calculate the distance
    beerRacer.slowDown(&beerRacer); //This functions slows down the car if distance < 15cm
    distance_pin_in_ClearInterrupt(); //Clear interrupt
}

CY_ISR(timer_interrupt_handler){
    //If statement is there to only send dato to ui every 5 time the distance is calculated
    if(timer_int_count < 5){
        distanceSensor.startDistanceCalc(&distanceSensor);  //Start the distance sensor calc (Send ultra sonic signal)
        timer_int_count++; //Increase counter
    } else {
        distanceSensor.startDistanceCalc(&distanceSensor); //Send sound signal from transmitter
        beerRacer.sendDataToUI(&beerRacer); //Send sensor data to UI
        timer_int_count = 0;    //Reset counter
    }
    Timer_ReadStatusRegister(); //Read status register to clear interrupt
}
