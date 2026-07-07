#include <Arduino.h>
// #include <HX711_2.h>
// #include <EasyNextionLibrary.h>
// #include <ADS1X15.h>

#include "NAU7802.h"

#define relayPin PA15  // PB0
#define UART_LCD Serial2
#define USART_DEBUG Serial  // USB-CDC (Takes PA8,PA9,PA10,PA11)

#define SCALES_SDA    PB9
#define SCALES_SCL    PB8
#define SCALES_RDY    PB0

float calibration_factor_lc1 = 4000; //-7050 worked for my 440lb max scale setup
float calibration_factor_lc2 = 4000; //-7050 worked for my 440lb max scale setup

//Nextion object init
// EasyNex myNex(UART_LCD);

// TwoWire scales_i2c = TwoWire(SCALES_SDA, SCALES_SCL);
NAU7802& scales = NAU7802::getInstance();

void setup() {
  USART_DEBUG.begin(115200);
  // myNex.begin(115200);
  pinMode(relayPin, OUTPUT);
  digitalWrite(relayPin, LOW);

  // while (myNex.readNumber("initCheck") != 100) {
  //   delay(600);
  // }

  // scales.init(calibration_factor_lc1, calibration_factor_lc2, &scales_i2c, SCALES_RDY);
  scales.init(
    -47200.f,
    -10700.f,
    0.0003775f,
    -0.0003811f,
    &Wire,
    SCALES_RDY);

  USART_DEBUG.println("Initialized");
}

void loop() {
  scales.service();
  static unsigned long timer = millis();
  float values[2];
  static float previousFactor1, previousFactor2;

  if (millis() > timer) {
    long rawValues[2];
    scales.getReadings(rawValues);
    // USART_DEBUG.printf("%8d %8d\n", rawValues[0], rawValues[1]);
    float units[2];
    scales.getUnits(units);
    USART_DEBUG.printf("%8.1f %8.1f\n", units[0], units[1]);
  }

  // myNex.NextionListen();

  // if (calibration_factor_lc1 != previousFactor1 || calibration_factor_lc2 != previousFactor2) {
  //   scales.setFactors(calibration_factor_lc1, calibration_factor_lc2);
  //   previousFactor1 = calibration_factor_lc1;
  //   previousFactor2 = calibration_factor_lc2;
  // }

  // if (millis() > timer) {
  //   if (myNex.currentPageId == 0) {
  //     scales.getUnits(values);
  //     myNex.writeStr("t0.txt",String(values[0],2));
  //     myNex.writeStr("t1.txt",String(values[1],2));

  //       myNex.writeStr("t2.txt", String(calibration_factor_lc1, 2));
  //       myNex.writeStr("t3.txt", String(calibration_factor_lc2, 2));
  //     }
  //     timer = millis() + 100ul;
  //   }
  // }
}

void trigger0() {
  // calibration_factor_lc1 -= myNex.readNumber("n1.val");
}

void trigger1() {
  // calibration_factor_lc1 += myNex.readNumber("n1.val");
}

void trigger2() {
  // calibration_factor_lc2 -= myNex.readNumber("n2.val");
}

void trigger3() {
  // calibration_factor_lc2 += myNex.readNumber("n2.val");
}

void trigger4() {
}

void trigger5() {
  // calibration_factor_lc1 = 4000;
  // calibration_factor_lc2 = 4000;
}
