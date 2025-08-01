#include "sensor.h"

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

#include "debug.h"

namespace Sensor
{
    Adafruit_ADS1115 ads;

    int32_t UV_DRY_1 = 2414375; // [uV] definition der 0 % feuchte fuer sensor 1
    int32_t UV_WET_1 = 900250; // [uV] definition der 100 % feuchte fuer sensor 1
    int32_t UV_DRY_2 = 2414375; // [uV] definition der 0 % feuchte fuer sensor 1
    int32_t UV_WET_2 = 900250; // [uV] definition der 100 % feuchte fuer sensor 1
    int32_t UV_DRY_3 = 2414375; // [uV] definition der 0 % feuchte fuer sensor 1
    int32_t UV_WET_3 = 900250; // [uV] definition der 100 % feuchte fuer sensor 1
    int32_t UV_DRY_4 = 2414375; // [uV] definition der 0 % feuchte fuer sensor 1
    int32_t UV_WET_4 = 900250; // [uV] definition der 100 % feuchte fuer sensor 1
    
    constexpr int32_t LSB_UV = 125; // µV pro LSB bei GAIN_ONE (±4.096 V)

    bool adsError;

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

    bool measure4Capsens(int32_t &sens1, int32_t &sens2, int32_t &sens3, int32_t &sens4)
    {
        if (adsError) {
            return false;
        }
        int16_t raw0 = ads.readADC_SingleEnded(0);
        int16_t raw1 = ads.readADC_SingleEnded(1);
        int16_t raw2 = ads.readADC_SingleEnded(2);
        int16_t raw3 = ads.readADC_SingleEnded(3);

        // Umrechnung in Mikrovolt (µV)
        sens1 = (int32_t)raw0 * LSB_UV;
        sens2 = (int32_t)raw1 * LSB_UV;
        sens3 = (int32_t)raw2 * LSB_UV;
        sens4 = (int32_t)raw3 * LSB_UV;

        // DEBUG_PRINTF("Rohwerte - AIN0: %ld µV | AIN1: %ld µV | AIN2: %ld µV | AIN3: %ld µV\n",
        //                 sens1, sens2, sens3, sens4);

        sens1 = map(sens1, UV_DRY_1, UV_WET_1, 0, 100);
        sens2 = map(sens2, UV_DRY_2, UV_WET_2, 0, 100);
        sens3 = map(sens3, UV_DRY_3, UV_WET_3, 0, 100);
        sens4 = map(sens4, UV_DRY_4, UV_WET_4, 0, 100);

        return true;
    }
}