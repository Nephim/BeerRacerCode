#include "BeerCounter.h"
#include <stdio.h>

#define beerWeight 302 //Beer weights 302gram
#define ADC_ratio 0.96 //1gram = 1.15624ADC
#define iterations 500  //Number of samples every time the weight is measured

BeerCounter BeerCounter_constructor(){
    //Start PGA's and ADC
    PGA_1_Start();
    PGA_2_Start();
    ADC_DelSig_Start();
    ADC_DelSig_StartConvert();
    
    //Create struct with the standart variables and combine with methods
    BeerCounter beerCounter;
    beerCounter.tareValue = 0;
    beerCounter.amount = 0;
    beerCounter.weight = 0;
    beerCounter.objectWeight = beerWeight;
    beerCounter.tare = BeerCounter_tare;
    beerCounter.getWeight = BeerCounter_getWeight;
    beerCounter.getAmount = BeerCounter_getAmount;
    beerCounter.tare(&beerCounter);
    
    return beerCounter; //Return the object
}

void BeerCounter_tare(BeerCounter* self){
    uint32_t sum = 0;
    //Make "iterations" number of measurements
    for (uint16_t i = 0; i < iterations; i++){
         if(ADC_DelSig_IsEndConversion(ADC_DelSig_WAIT_FOR_RESULT)){
            sum = sum + ADC_DelSig_GetResult16();
        }
    }
    //Find the mean value of all the measurements and use this as tare value
    uint16_t tare = sum/iterations;
    self->tareValue = tare;
}

uint16_t BeerCounter_getWeight(BeerCounter *self){
    uint32_t sum = 0;
    //Make "iterations" number of measurements
    for (uint16_t i = 0; i < iterations; i++){
         if(ADC_DelSig_IsEndConversion(ADC_DelSig_WAIT_FOR_RESULT)){
            sum = sum + ADC_DelSig_GetResult16()-self->tareValue;
        }
    }
    //Find the mean value of all the measurements. Add some validation
    sum=(sum/iterations);
    sum=(sum>65000?0:sum);
    
    self->weight = sum/ADC_ratio;   //Calculate the actial weight
    return self->weight;
}

uint8_t BeerCounter_getAmount(BeerCounter *self){
    self->getWeight(self);
    uint8_t antal = 0;
    uint16_t start = self->objectWeight/2;
    for(uint8_t i = 0; i < 9; i++){
        if(self->weight > (self->objectWeight*i+start)){
            antal++;
        } else {
            break;
        }
    }
    self->amount = (antal>9?9:antal);
    return self->amount;
}