#include "project.h"
#include "RPIComm.h"

RPIComm RPIComm_constructor(){
    UART_Start();
    RPIComm rpiComm;
    rpiComm.sendByte = RPIComm_sendByte;
    return rpiComm;
}

void RPIComm_sendByte(uint8_t byte){
    //Send byte
    UART_PutChar(byte);
}