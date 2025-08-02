# humiot
humidity sensor box ready for iot

## Uebersicht

- Node-RED
- Raspberry Pi 3B+ (32-Bit)
- influxdb V1.8 (wg. 32-Bit)
  Datenbank = mydb
- MQTT (mosquitto) V2.0.11
- grafana V10.0.3



### influx db abfrage

´´´
SELECT * FROM moisture WHERE  time > now() - 7d
´´´