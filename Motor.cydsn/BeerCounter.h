#pragma once
#include "project.h"

typedef struct BeerCounter BeerCounter;

struct BeerCounter{
    uint16_t tareValue;
    uint8_t amount;
    uint16_t weight;
    uint16_t objectWeight;
    void (*tare)(BeerCounter*);
    uint16_t (*getWeight)(BeerCounter*);
    uint8_t (*getAmount)(BeerCounter*);
};

BeerCounter BeerCounter_constructor();
void BeerCounter_tare(BeerCounter*);
uint16_t BeerCounter_getWeight(BeerCounter*);
uint8_t BeerCounter_getAmount(BeerCounter*);