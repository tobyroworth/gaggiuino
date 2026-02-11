/* 09:32 15/03/2023 - change triggering comment */
#include "NAU7802.h"

uint32_t _SCALES_RDY;
volatile bool _ready = false;

void handleScalesReady(void) {
  if (digitalRead(_SCALES_RDY) == HIGH) {
    _ready = true;
  }
}

bool NAU7802::init(float scalesF1, float scalesF2, TwoWire* i2c, uint32_t drdyPin) {

  bool error = false;

  // pinMode(drdyPin, INPUT);
  // attachInterrupt(digitalPinToInterrupt(drdyPin), handleScalesReady, RISING);

  error = this->nau.begin(i2c);
  // if (error) { return error; };
  error = this->nau.setLDO(NAU7802_4V2);
  error = this->nau.setGain(NAU7802_GAIN_128);
  error = this->nau.setRate(NAU7802_RATE_20SPS);
  // if (error) { return error; };
  // this->setFactors(scalesF1, scalesF2);
  this->setFactors(1996.0f, 2605.0f);

  error = this->nau.calibrate(NAU7802_CALMOD_INTERNAL);
  // if (error) { return error; };

  // this->tare();
  return false;
}

bool NAU7802::service(void) {
  if (this->nau.available()) {
    const int32_t reading = this->nau.read();
    this->_readings[this->_channel] = reading;

    _ready = false;
    this->_channel = 1 - this->_channel;
    this->nau.setChannel(this->_channel);
    this->nau.calibrate(NAU7802_CALMOD_INTERNAL);
    return true;
  }
  return false;
}

void NAU7802::tare(void) {
  this->_tareWeight = 0.f;
  float newTareWeight = 0.f;

  while (this->_channel != 0) {
    this->service();
  }

  for (uint8_t readings = 0; readings < 4; readings++) {
    // get first channel
    while (!this->service());
    // get second channel
    while (!this->service());
    newTareWeight += this->getWeight().value;
  }
  this->_tareWeight = newTareWeight / 4;
}


void NAU7802::setFactors(float factor1, float factor2) {
  this->_factors[0] = factor1;
  this->_factors[1] = factor2;
}

void NAU7802::getReadings(int32_t* readings) {
  readings[0] = this->_readings[0];
  readings[1] = this->_readings[1];
}

void NAU7802::getUnits(float* units) {
  units[0] = (float)this->_readings[0] / this->_factors[0];
  units[1] = (float)this->_readings[1] / this->_factors[1];
}

Measurement NAU7802::getWeight(void) {
  Measurement currentWeight = Measurement{ .value = 0.f, .millis = 0 };
  float units[2] = {};
  this->getUnits(units);
  currentWeight = Measurement{
    .value = units[0] + units[1] - this->_tareWeight,
    .millis = millis()
  };
  return currentWeight;
}
