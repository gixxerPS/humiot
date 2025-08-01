#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

namespace Sensor
{
    void setup();

    /**
     * Dauert ca. 4x125 ms = 500 ms.
     * Call-By-Reference. rueckgabewerte sind ueber die parameter abzuholen
     * 
     * @param sens1 - [%*100] 100% => wasser | 120% => kein Sensor angeschlossen
     * @param sens2 - [%*100] 100% => wasser | 120% => kein Sensor angeschlossen
     * @param sens3 - [%*100] 100% => wasser | 120% => kein Sensor angeschlossen
     * @param sens4 - [%*100] 100% => wasser | 120% => kein Sensor angeschlossen
     * 
     * @return - false, wenn analogmodul (ADS1115) nicht erkannt 
     */
    bool measure4Capsens(int32_t &sens1, int32_t &sens2, int32_t &sens3, int32_t &sens4);
}

#endif // SENSOR_H