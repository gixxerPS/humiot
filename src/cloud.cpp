#include "cloud.h"

#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#include "debug.h"
#include "password.h"
#include "sensor.h"

namespace Cloud
{
    String MQTT_TOPIC_BASE      = "capsense"; // Beispiel fuer Station
    String MQTT_SUBTOPIC_SUBSCRIBE = "/cmd"; // e.g. gartenhaus/cmd
    String MQTT_TOPIC_PUBLISH   = "/sensorData"; // e.g. gartenhaus/sensorData

    char myMqttId[18]; // wird zur laufzeit ausgelesen und ist eindeutig
    
    bool noWifiActive;
    bool clientConnected;

    WiFiClient espClient;
    PubSubClient client(espClient);

    // mac adresse als geraete id verwenden
    void getDeviceId() {
        uint8_t mac[6];
        esp_read_mac(mac, ESP_MAC_WIFI_STA);  // oder WiFi.macAddress() für String-Form
        snprintf(myMqttId, sizeof(myMqttId), "%02X%02X%02X%02X%02X%02X",
                mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

        DEBUG_PRINTF("MQTT ID: %s", myMqttId);
    }

    void myPublish(const char *topic, const uint8_t * payload, unsigned int plength)
    {
        DEBUG_PRINTF("topic: %s, payload:%s\r\n", topic, payload);
        client.publish(topic, payload, plength);
    }

    void datenSchicken(Sensor::MeasVal sens[4]) 
    {
        JsonDocument doc;
        char buffer[256];
        size_t n;

        String topicSensBase =  MQTT_TOPIC_BASE + "/" + String(myMqttId) + "/sensor/";
        String topic;
        for (uint8_t i=0; i<4; i++) {
            doc["raw"] = sens[i].raw;
            doc["scaled"] = sens[i].scaled;

            n = serializeJson(doc, buffer);

            // topic aufbau:
            // capsense/<device_id>/sensor/<sensor_id>/moisture
            //
            // beispiel:
            // capsense/ECE3348E7474/sensor/1/moisture
            topic = topicSensBase + String(i+1) + "/moisture";
            DEBUG_INFO("topic:" + topic);
            DEBUG_PRINTF("payload:%s\r\n", buffer);
            client.publish(topic.c_str(), buffer, n);
        }
    }

    void callback(char *topic, byte *payload, unsigned int length)
    {
        // DEBUG_INFO("Message received on topic: ");
        // DEBUG_INFO(topic);
        // DEBUG_INFO("Message: ");
        // for (int i = 0; i < length; i++)
        // {
        //     DEBUG_INFO( String(payload[i]));
        // }

        // payload in durchsuchbaren string umwandeln
        char msg[length + 1];
        memcpy(msg, payload, length);
        msg[length] = '\0';  // Null-Terminierung anhängen
        String message = String(msg);  // in Arduino String umwandeln
        String kommando;
        int32_t sensor_id;

        // string durchsuchen nach ';'
        int sepIndex = message.indexOf(';');
        if (sepIndex != -1) {
            sensor_id = message.substring(0, sepIndex).toInt();
            kommando = message.substring(sepIndex + 1);
            if (kommando == "dry") { 
                Sensor::setCalibrationDry(sensor_id);
            } else if (kommando == "wet"){
                Sensor::setCalibrationWet(sensor_id);
            } else {
                DEBUG_ERROR("Ungültiges Kommando: " + kommando);
            }
        } else { // kein ';' gefunden
            // befehl = jetzt messen ?
            if (message == "meas") {
                Sensor::MeasVal sens[4];
                if ( !Sensor::measure4Capsens(sens) ) {
                    DEBUG_ERROR("MESSFEHLER. ADS NICHT GEFUNDEN");
                }
                sendToCloud(sens);
            } else { // auch kein messbefehl, dann ungueltiger cmd
                DEBUG_ERROR("Ungültiges Kommando-Format: " + message);
            }
        }
    }

    bool reconnect() {
        if (client.connect("humiotClient")) {
            // ... and resubscribe

            // capsense/ECE3348E7474/sensor/1/moisture
            String topicSubscribe =  MQTT_TOPIC_BASE + "/" + String(myMqttId) + MQTT_SUBTOPIC_SUBSCRIBE;
            DEBUG_INFO("subcribe to topic: " + topicSubscribe);
            client.subscribe(topicSubscribe.c_str());
        }
        return client.connected();
    }

    void setup()
    {
        DEBUG_INFO("WIFI MAC= " + WiFi.macAddress());
        getDeviceId();
    
        DEBUG_INFO("📶 Verbinde mit: " + String(WIFI_SSID));
        noWifiActive = true;
        WiFi.begin(WIFI_SSID, WIFI_PASS);

        int timeout = 15; // 15 Sekunden Timeout
        while (WiFi.status() != WL_CONNECTED && timeout > 0) {
            delay(1000);
            Serial.print(".");
            timeout--;
        }

        if (WiFi.status() == WL_CONNECTED) {
            DEBUG_INFO("\n✅ WLAN verbunden!");
            DEBUG_INFO("📡 IP-Adresse: " + String(WiFi.localIP()));
            noWifiActive = false;
            client.setServer(MQTT_SERVER, 1883);
            client.setCallback(callback);
            reconnect();

        } else {
            DEBUG_ERROR("\n❌ WLAN-Verbindung fehlgeschlagen!");
        }
    }
   
    void sendToCloud(Sensor::MeasVal sens[4])
    {
        if (!clientConnected) {
            DEBUG_WARN("Attempt to reconnect");
            if (reconnect()) {
                DEBUG_INFO("successfully reconnected");
            } else {
                DEBUG_ERROR("VERBINDUNGSVERSUCH FEHLGESCHLAGEN.");
            }
        }
        datenSchicken(sens);
    }

    void loop()
    {
        uint32_t curMillis = millis();
        static uint32_t lastReconnectAttempt = 0;
        bool clientConnected = client.connected();
        // not connected and wifi data present?
        if (!clientConnected && !noWifiActive) {
            if (curMillis - lastReconnectAttempt > 5000) {
                lastReconnectAttempt = curMillis;
                DEBUG_WARN("Attempt to reconnect");
                if (reconnect()) {
                    DEBUG_INFO("successfully reconnected");
                    lastReconnectAttempt = 0;
                }
            }
        } else {
            // Client connected
            client.loop();
        }
    }
} 