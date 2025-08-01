#ifndef CLOUD_H
#define CLOUD_H

#include <Arduino.h>

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
    void sendToCloud(int32_t sens1, int32_t sens2, int32_t sens3, int32_t sens4);
} 
#endif // CLOUD_H
