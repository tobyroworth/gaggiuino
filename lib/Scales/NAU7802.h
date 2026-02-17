/* 09:32 15/03/2023 - change triggering comment */
#ifndef NAU7802_H
#define NAU7802_H

#include <Wire.h>
#include <Adafruit_NAU7802.h>
#include "measurements.h"

class NAU7802 {

private:
  Adafruit_NAU7802 nau;

  NAU7802() {};

  uint8_t _channel = 0;
  int32_t _readings[2] = { 0, 0 };
  float _offsets[2] = { 0.f, 0.f };
  float _factors[2] = { 0.f, 0.f };
  float _tareWeight = 0.f;

public:
  static NAU7802& getInstance() {
    static NAU7802 instance;
    return instance;
  }

  bool init (
    float scalesOffset1,
    float scalesOffset2,
    float scalesFactor1,
    float scalesFactor2,
    TwoWire* i2c,
    uint32_t drdyPin
  );
  bool service(void);
  void tare(void);
  void setOffset(float offset1, float offset2);
  void setFactors(float factor1, float factor2);
  void getReadings(int32_t* readings);
  void getUnits(float* units);
  Measurement getWeight(void);
};

#endif
