/* 09:32 15/03/2023 - change triggering comment */#ifndef SCALES_H
#define SCALES_H

// Needs to be here so it pleases so cppcheck is pleased.
#ifndef FORCE_PREDICTIVE_SCALES
#define FORCE_PREDICTIVE_SCALES 0
#endif

#include "measurements.h"

void scalesInit(float scalesF1, float scalesF2);
bool serviceScales(void);
void scalesTare(void);
void scalesSetFactors(float factor1, float factor2);
void scalesGetReadings(int32_t readings[]);
void scalesGetUnits(float units[]);
Measurement scalesGetWeight(void);
bool scalesIsPresent(void);
// float scalesDripTrayWeight();

#endif
