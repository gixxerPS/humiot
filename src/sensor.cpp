#include "sensor.h"

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

#include "debug.h"

namespace Sensor
{
    constexpr int32_t LSB_UV = 125; // µV pro LSB bei GAIN_ONE (±4.096 V)
    bool adsError;
    Adafruit_ADS1115 ads;

    Calibration calib[4] = {
        {2414375, 900250},
        {2414375, 900250},
        {2414375, 900250},
        {2414375, 900250}
    };

    void setCalibration(uint8_t sensorId, int32_t uv_dry, int32_t uv_wet)
    {
        if (sensorId >= 0 && sensorId <= 3) {
            calib[sensorId].uv_dry = uv_dry;
            calib[sensorId].uv_wet = uv_wet;
        }
    }

    void setCalibrationDry(uint8_t sensorId)
    {
        if (sensorId >= 0 && sensorId <= 3) {
            calib[sensorId].uv_dry = (int32_t) ads.readADC_SingleEnded(sensorId)* LSB_UV;
            DEBUG_PRINTF("setCalibrationDry: id=%d, uv=%ld", sensorId, calib[sensorId].uv_dry);
        }
    }

    void setCalibrationWet(uint8_t sensorId)
    {
        if (sensorId >= 0 && sensorId <= 3) {
            calib[sensorId].uv_wet = (int32_t) ads.readADC_SingleEnded(sensorId)* LSB_UV;
            DEBUG_PRINTF("setCalibrationWet: id=%d, uv=%ld", sensorId, calib[sensorId].uv_wet);
        }
    }

    
    void setup()
    {
        Wire.begin(21, 22);  // SDA, SCL — ggf. anpassen
        if (!ads.begin()) {
            DEBUG_ERROR("ADS1115 nicht gefunden!");
            adsError = true;
        }
        ads.setGain(GAIN_ONE);  // +/- 4.096V Messbereich (ideal für 0–3.3 V)
        ads.setDataRate(RATE_ADS1115_8SPS); // je messung ca 125ms
    }

    bool measure4Capsens(MeasVal &sens1, MeasVal &sens2, MeasVal &sens3, MeasVal &sens4)
    {
        if (adsError) {
            return false;
        }
        int32_t raw1 = (int32_t)ads.readADC_SingleEnded(0)* LSB_UV;
        int32_t raw2 = (int32_t)ads.readADC_SingleEnded(1)* LSB_UV;
        int32_t raw3 = (int32_t)ads.readADC_SingleEnded(2)* LSB_UV;
        int32_t raw4 = (int32_t)ads.readADC_SingleEnded(3)* LSB_UV;

        // Umrechnung in mV
        sens1.raw = raw1 / 1000;
        sens2.raw = raw2 / 1000;
        sens3.raw = raw3 / 1000;
        sens4.raw = raw4 / 1000;

        // DEBUG_PRINTF("Rohwerte - AIN0: %ld µV | AIN1: %ld µV | AIN2: %ld µV | AIN3: %ld µV\n",
        //                 sens1, sens2, sens3, sens4);

        sens1.scaled = map(raw1, calib[0].uv_dry, calib[0].uv_wet, 0, 100);
        sens2.scaled = map(raw2, calib[1].uv_dry, calib[1].uv_wet, 0, 100);
        sens3.scaled = map(raw3, calib[2].uv_dry, calib[2].uv_wet, 0, 100);
        sens4.scaled = map(raw4, calib[3].uv_dry, calib[3].uv_wet, 0, 100);

        return true;
    }
}