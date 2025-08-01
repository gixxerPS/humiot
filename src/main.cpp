/**
 * @mainpage
 * ESP32 mit ADS1115 fuer 4 bodenfeuchte sensoren capsens.
 */
#include <Arduino.h>
#include <limits.h>

#include "debug.h"
#include "cloud.h"
#include "sensor.h"

const uint32_t CYCLE_TEST = 2000; // [ms] zyklus fuer testfunktionen
const uint32_t INTERVAL_MEAS_AND_SEND_DATA = 3000; // [ms] alle x ms messwerte schicken

void setup(void)
{
  DEBUG_BEGIN(115200);
  delay(100); // ohne delay friert esp ein
 
  Sensor::setup(); // i2c initialisieren
  Cloud::setup(); // wlan verbinden etc

  delay(2000); // kleine verzoegerung bevor wir zyklisch werden
}

void loop(void)
{
  uint32_t curMillis = millis();

  // //=============================================================================
  // // applikation / display logik
  // //=============================================================================
  static uint32_t msLastMeascycle;
  if (curMillis - msLastMeascycle > INTERVAL_MEAS_AND_SEND_DATA) {
    msLastMeascycle = curMillis;

    int32_t uv0=LONG_MIN, uv1=LONG_MIN, uv2=LONG_MIN, uv3=LONG_MIN;

    if ( !Sensor::measure4Capsens(uv0, uv1, uv2, uv3) ) {
      DEBUG_ERROR("MESSFEHLER. ADS NICHT GEFUNDEN");
    }
    DEBUG_PRINTF("AIN0: %ld %% | AIN1: %ld %% | AIN2: %ld %% | AIN3: %ld %%\n",
                        uv0, uv1, uv2, uv3);

    Cloud::sendToCloud(uv0, uv1, uv2, uv3);
  }

  //=============================================================================
  // testzyklus
  //=============================================================================
  static uint32_t msLastTestcycle;
  if (curMillis - msLastTestcycle > CYCLE_TEST) {
    msLastTestcycle = curMillis;

    
  }
}
