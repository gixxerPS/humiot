
#ifndef DEBUG_H
#define DEBUG_H

#include <string>
#include "Arduino.h"

#define DEBUG_ACTIVE 1
#define DEBUG_LEVEL 3  // 0 = aus, 1 = Fehler, 2 = Warnungen, 3 = Infos

#if DEBUG_ACTIVE
  #define DEBUG_ERROR(x) if (DEBUG_LEVEL >= 1) { Serial.print("[ERROR] "); Serial.print(millis()); Serial.print(" - "); Serial.println(x); }
  #define DEBUG_WARN(x) if (DEBUG_LEVEL >= 2) { Serial.print("[WARN] "); Serial.print(millis()); Serial.print(" - "); Serial.println(x); }
  #define DEBUG_INFO(x) if (DEBUG_LEVEL >= 3) { Serial.print("[INFO] "); Serial.print(millis()); Serial.print(" - "); Serial.println(x); }
  #define DEBUG_PRINTF(fmt, ...) if (DEBUG_LEVEL >= 3) {Serial.print("[INFO] "); Serial.print(millis()); Serial.print(" - "); Serial.printf((fmt), ##__VA_ARGS__);}
  #define DEBUG_BEGIN(baud) Serial.begin(baud)
#else
  #define DEBUG_ERROR(x)
  #define DEBUG_WARN(x)
  #define DEBUG_INFO(x)
  #define DEBUG_PRINTF(fmt, ...)
  #define DEBUG_BEGIN(baud)
#endif

// beispiele:

// void setup() {
//     DEBUG_BEGIN(115200);
//     DEBUG_INFO("System gestartet.");
//     DEBUG_WARN("Sensorwert ungewöhnlich hoch!");
//     DEBUG_ERROR("Verbindung fehlgeschlagen.");
// }

// void loop() {
//     // Beispiel: Sensorwert ausgeben, aber nur, wenn Debug-Level hoch genug ist
//     DEBUG_INFO("Loop läuft...");
//     delay(1000);
// }

#endif