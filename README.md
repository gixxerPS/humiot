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


### csv aus grafana daten erzeugen

1. Öffne dein Dashboard.
2. Fahre mit der Maus über das gewünschte Panel.
3. Klicke auf das Panel-Menü (drei Punkte ⋮) oben rechts.
4. Wähle "Inspect" → "Data".
5. Dort siehst du eine Tabelle mit den Abfragedaten.
6. Klicke auf "Download CSV".