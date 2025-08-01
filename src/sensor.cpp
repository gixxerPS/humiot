#include "sensor.h"

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

#include "debug.h"

namespace Sensor
{
    Adafruit_ADS1115 ads;

    const int16_t UV_100PERCENT = 1; // [uV] definition der 100 % feuchte
    constexpr int32_t LSB_UV = 125; // µV pro LSB bei GAIN_ONE (±4.096 V)

    void setup()
    {
        Wire.begin(21, 22);  // SDA, SCL — ggf. anpassen
        if (!ads.begin()) {
            DEBUG_ERROR("ADS1115 nicht gefunden!");
            while (1);
        }
        ads.setGain(GAIN_ONE);  // +/- 4.096V Messbereich (ideal für 0–3.3 V)
        ads.setDataRate(RATE_ADS1115_8SPS); // je messung ca 125ms
    }

    void measure4Capsens(int32_t &sens1, int32_t &sens2, int32_t &sens3, int32_t &sens4)
    {
       
        int16_t raw0 = ads.readADC_SingleEnded(0);
        int16_t raw1 = ads.readADC_SingleEnded(1);
        int16_t raw2 = ads.readADC_SingleEnded(2);
        int16_t raw3 = ads.readADC_SingleEnded(3);

        // Umrechnung in Mikrovolt (µV)
        sens1 = (int32_t)raw0 * LSB_UV;
        sens2 = (int32_t)raw1 * LSB_UV;
        sens3 = (int32_t)raw2 * LSB_UV;
        sens4 = (int32_t)raw3 * LSB_UV;
    }
}