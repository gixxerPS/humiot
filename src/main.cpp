/**
 * @mainpage
 * Logik controller der fuegevorrichtung fur schuheinlagen.
 *
 */
#include <Arduino.h>

#include "debug.h"
#include "cloud.h"
#include "sensor.h"

const uint32_t CYCLE_TEST = 2000; // [ms] zyklus fuer testfunktionen
const uint32_t CYCLE_SETOUT_SLOW = 50; // [ms] zyklus langsame (i2c) ausgaenge setzen

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
  static uint32_t lastMillisSetOut = 0;

  //=============================================================================
  // zyklus handling
  //=============================================================================
  // MAINUTIL::loop();

  // //=============================================================================
  // // applikation / display logik
  // //=============================================================================
  // SEQ::loop(); // Automatik Sequenz
  // ctrlAppInst.loop(); //Regelung/Steuerung der Heizung
  // Display.loop(); //Abfragen ob ein "printh" Befehl vom Display gesendet wurde und auslösen der Callback Funktion
  // RecipeNS::loop(); //HAndling der Rezepte
  // OpMode::loop();  //Auswahl der Betriebsart manuell/automatik und steuern des manuellen Modus
  // ACTORS::loop();  //Handling der Zylindersteuerung

  // //=============================================================================
  // // cloud / connectivity aufgaben
  // //=============================================================================
  Cloud::loop();

  //=============================================================================
  // testzyklus
  //=============================================================================
  static uint32_t msLastTestcycle;
  if (curMillis - msLastTestcycle > CYCLE_TEST) {
    msLastTestcycle = curMillis;

    int32_t uv0, uv1, uv2, uv3;

    Sensor::measure4Capsens(uv0, uv1, uv2, uv3);

    Serial.printf("AIN0: %ld µV | AIN1: %ld µV | AIN2: %ld µV | AIN3: %ld µV\n",
                        uv0, uv1, uv2, uv3);
  }
}
