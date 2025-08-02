/**
 * @mainpage
 * ESP32 mit ADS1115 fuer 4 bodenfeuchte sensoren capsens.
 * 
 * 
 */
#include <Arduino.h>
#include <limits.h>
#include <WiFi.h>

#include "debug.h"
#include "cloud.h"
#include "sensor.h"

const uint32_t CYCLE_TEST = 10000; // [ms] zyklus fuer testfunktionen
const uint32_t INTERVAL_MEAS_AND_SEND_DATA = 360000; // [ms] alle x ms messwerte schicken

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
  Cloud::loop();
  
  static uint32_t msLastMeascycle;
  if (curMillis - msLastMeascycle > INTERVAL_MEAS_AND_SEND_DATA) {
    msLastMeascycle = curMillis;

    Sensor::MeasVal sens[4];

    if ( !Sensor::measure4Capsens(sens) ) {
      DEBUG_ERROR("MESSFEHLER. ADS NICHT GEFUNDEN");
    }
    // DEBUG_PRINTF("AIN0: %ld %% | AIN1: %ld %% | AIN2: %ld %% | AIN3: %ld %%\n",
    //                     uv0, uv1, uv2, uv3);

    Cloud::sendToCloud(sens);
  }

  //=============================================================================
  // testzyklus
  //=============================================================================
  static uint32_t msLastTestcycle;
  if (curMillis - msLastTestcycle > CYCLE_TEST) {
    msLastTestcycle = curMillis;

    //–30 bis –50	  Exzellent	  Sehr nah am Access Point
    // –51 bis –60	Gut	        Schnelle und stabile Verbindung
    // –61 bis –70	OK ✅	      Stabil, ausreichend für MQTT, Web
    // –71 bis –80	Schwach ⚠️	  Möglich, aber mit Aussetzern
    // < –80	      Kritisch ❌	Hohe Fehlerrate, Verbindungsabbruch
    DEBUG_PRINTF("Aktuelle Signalstärke: %ld dBm\r\n", WiFi.RSSI());
  }
}
