/* 09:32 15/03/2023 - change triggering comment */
#ifndef FLOW_SENSOR_H
#define FLOW_SENSOR_H

#include <Arduino.h>

void interruptInit(void);
float getSensorFlow(const float elapsedTimeSec);

#endif
