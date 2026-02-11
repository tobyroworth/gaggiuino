/* 09:32 15/03/2023 - change triggering comment */
#include "flow_sensor.h"
#include "pindef.h"
#include "../log.h"

volatile long pulseCount = 0;

const float mlPerPulse = 0.02f; // 49162 pulse/l or 0.02 ml/pulse

void handleFlowPulse(void) {
  pulseCount++;
}

void flowInterruptInit(void) {
#ifdef flowPin
  attachInterrupt(digitalPinToInterrupt(flowPin), handleFlowPulse, RISING);
#endif
}

float getSensorFlow(const float elapsedTimeSec) {
  const float currentPulseCount = (float)pulseCount;
  pulseCount = 0;
  return currentPulseCount * mlPerPulse / elapsedTimeSec;
}
