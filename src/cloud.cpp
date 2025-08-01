#include "cloud.h"

#include <WiFi.h>
#include <PubSubClient.h>

#include "debug.h"
#include "password.h"

namespace Cloud
{
    String MQTT_TOPIC_BASE      = "capsense"; // Beispiel fuer Station
    String MQTT_TOPIC_SUBSCRIBE = MQTT_TOPIC_BASE+"/cmd"; // e.g. gartenhaus/cmd
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

    void myPublish(const char *topic, const char *payload)
    {
        DEBUG_PRINTF("topic: %s, payload:%s\r\n", topic, payload);
        client.publish(topic, payload);
    }

    void datenSchicken(int32_t sens1, int32_t sens2, int32_t sens3, int32_t sens4) 
    {
        // topic aufbau:
        // capsense/<device_id>/sensor/<sensor_id>/moisture
        //
        // beispiel:
        // capsense/ECE3348E7474/sensor/1/moisture
        String topicSensBase =  MQTT_TOPIC_BASE + "/" + String(myMqttId) + "/sensor/";
        String topic = topicSensBase + "1/moisture";
        myPublish(topic.c_str(), String(sens1).c_str());
        
        topic = topicSensBase + "2/moisture";
        myPublish(topic.c_str(), String(sens2).c_str());

        topic = topicSensBase + "3/moisture";
        myPublish(topic.c_str(), String(sens3).c_str());

        topic = topicSensBase + "4/moisture";
        myPublish(topic.c_str(), String(sens4).c_str());
    }

    void callback(char *topic, byte *payload, unsigned int length)
    {
        DEBUG_INFO("Message received on topic: ");
        DEBUG_INFO(topic);
        DEBUG_INFO("Message: ");
        for (int i = 0; i < length; i++)
        {
            DEBUG_INFO( String(payload[i]));
        }
        if ((char)payload[0] == '1') { // we subscribed to the right topic so '1' => start
            // digitalWrite(LED_BUILTIN, LOW);
            // cmdSend = true;
            DEBUG_INFO("Licht AN");
            //DO::setOutHeater(HIGH); // TEST !!!
        } else {
            // digitalWrite(LED_BUILTIN, HIGH);
            DEBUG_INFO("Licht AUS");
            //DO::setOutHeater(LOW); // TEST !!!
        }
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
        } else {
            DEBUG_ERROR("\n❌ WLAN-Verbindung fehlgeschlagen!");
        }
    }
    bool reconnect() {
        if (client.connect("humiotClient")) {
            // Once connected, publish an announcement...
            client.publish("test","hello world");
            // ... and resubscribe
            client.subscribe(MQTT_TOPIC_SUBSCRIBE.c_str());
        }
        return client.connected();
    }
    void sendToCloud(int32_t sens1, int32_t sens2, int32_t sens3, int32_t sens4)
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

        if (!clientConnected) {
            return;
        }
        datenSchicken(sens1,  sens2,  sens3,  sens4);
    }
} 