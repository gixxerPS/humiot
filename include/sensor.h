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
     * @param sens1 - [%*100] 100% => wasser
     * @param sens2 - [%*100] 100% => wasser
     * @param sens3 - [%*100] 100% => wasser
     * @param sens4 - [%*100] 100% => wasser
     */
    void measure4Capsens(int32_t &sens1, int32_t &sens2, int32_t &sens3, int32_t &sens4);
}

#endif // SENSOR_H