/* 09:32 15/03/2023 - change triggering comment */
#include "scales.h"
#include "pindef.h"
#include "remote_scales.h"

#include <NAU7802.h>

bool hwScalesPresent = false;

// TwoWire scales_i2c(SCALES_SDA, SCALES_SCL);
NAU7802& scales = NAU7802::getInstance();

void scalesInit(float scalesOffset1, float scalesOffset2, float scalesFactor1, float scalesFactor2) {
  hwScalesPresent = false;
  // Forced predicitve scales in case someone with actual hardware scales wants to use them.
  if (FORCE_PREDICTIVE_SCALES) {
    return;
  }

#ifndef DISABLE_HW_SCALES
  hwScalesPresent = !scales.init(scalesOffset1, scalesOffset2, scalesFactor1, scalesFactor2, &Wire, SCALES_RDY);
  // hwScalesPresent = true;
#endif

  scalesTare();
}

bool serviceScales(void) {
  if (hwScalesPresent) {
    return scales.service();
  }
  return false;
}

void scalesTare(void) {
  if (hwScalesPresent) {
    scales.tare();
  }
  else if (remoteScalesIsPresent()) {
    remoteScalesTare();
  }
}


void scalesSetFactors(float factor1, float factor2) {
  scales.setFactors(factor1, factor2);
}

void scalesGetReadings(int32_t readings[]) {
  scales.getReadings(readings);
}

void scalesGetUnits(float units[]) {
  scales.getUnits(units);
}

Measurement scalesGetWeight(void) {
  return scales.getWeight();
}

bool scalesIsPresent(void) {
  // Forced predicitve scales in case someone with actual hardware scales wants to use them.
  if (FORCE_PREDICTIVE_SCALES) {
    return false;
  }
  return hwScalesPresent || remoteScalesIsPresent();
}

// float scalesDripTrayWeight() {
//   long value[2] = {};
//   if (hwScalesPresent) {
//     LoadCellSingleton::getInstance().read_average(value, 4);
//   }
//   return ((float)value[0] + (float)value[1]);
// }
