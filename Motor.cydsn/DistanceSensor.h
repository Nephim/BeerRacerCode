#pragma once
#include "project.h"
#include <stdbool.h>

typedef struct DistanceSensor DistanceSensor;

struct DistanceSensor{
    uint8_t distance;
    bool isSend;
    uint32_t sendTime;
    void (*startDistanceCalc)(DistanceSensor*);
    void (*calculateDistance)(DistanceSensor*, uint16_t counterValue);
    uint8_t (*getDistance)(DistanceSensor*);
};

DistanceSensor DistanceSensor_constructor();
void DistanceSensor_startDistanceCalc(DistanceSensor*);
void DistanceSensor_calculateDistance(DistanceSensor*, uint16_t counterValue);
uint8_t DistanceSensor_getDistance(DistanceSensor*);