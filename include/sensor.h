#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

namespace Sensor
{

    struct Calibration
    {
        int32_t uv_dry; // [uV] definition der 0 % feuchte
        int32_t uv_wet; // [uV] definition der 100 % feuchte
    };

    struct MeasVal
    {
        int32_t raw=LONG_MIN;    // [mV] 
        int32_t scaled=LONG_MIN; // [%*100] 100% => wasser | 120% => kein Sensor angeschlossen
    };

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
    bool measure4Capsens(MeasVal &sens1, MeasVal &sens2, MeasVal &sens3, MeasVal &sens4);


    
    void setCalibration(uint8_t sensorId, int32_t uv_dry, int32_t uv_wet);
    void setCalibrationDry(uint8_t sensorId);
    void setCalibrationWet(uint8_t sensorId);

    
    
}

#endif // SENSOR_H