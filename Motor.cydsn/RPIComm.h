#pragma once
#include "project.h"

typedef struct RPIComm RPIComm;

struct RPIComm{
    void (*sendByte)(uint8_t byte);
};

//DriveMotor constructor
RPIComm RPIComm_constructor();

void RPIComm_sendByte(uint8_t byte);

