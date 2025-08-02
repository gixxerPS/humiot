#ifndef CLOUD_H
#define CLOUD_H

#include <Arduino.h>

#include "sensor.h"

/**
 * Modul fuer Cloud-Funktionen.
 * 
 * @brief Kapselt anbindung an cloud etc
 * 
 * "Datenmodul" in dem die Zustaende gehalten werden und abgefragt werden koennen
 */

namespace Cloud
{
    void loop();
    void setup();
    void sendToCloud(Sensor::MeasVal sens1, Sensor::MeasVal sens2, Sensor::MeasVal sens3, Sensor::MeasVal sens4);
} 
#endif // CLOUD_H
