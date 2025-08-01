#include "cloud.h"

#include <WiFi.h>
#include <PubSubClient.h>

#include "debug.h"
#include "password.h"

namespace Cloud
{
    String MQTT_TOPIC_BASE      = "humiot"; // Beispiel fuer Station
    String MQTT_TOPIC_SUBSCRIBE = MQTT_TOPIC_BASE+"/cmd"; // e.g. gartenhaus/cmd
    String MQTT_TOPIC_PUBLISH   = MQTT_TOPIC_BASE+"/sensorData"; // e.g. gartenhaus/sensorData

    uint32_t INTERVAL_SEND_DATA = 60000; // [ms] alle x ms messwerte schicken

    bool noWifiActive;

    WiFiClient espClient;
    PubSubClient client(espClient);

    // BEISPIEL
    void datenSchicken() {
        String topic = MQTT_TOPIC_PUBLISH + "/temp";
        DEBUG_INFO(topic);
        client.publish(topic.c_str(), String(21.2f).c_str());
        topic = MQTT_TOPIC_PUBLISH + "/humidity";
        client.publish(topic.c_str(), String(67.5f).c_str());
        topic = MQTT_TOPIC_PUBLISH + "/pressure";
        client.publish(topic.c_str(), String(1013).c_str());
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
        DEBUG_INFO("WIFI MAC:m" + WiFi.macAddress());
    
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

        static uint32_t msLastTestcycle;
        if (curMillis - msLastTestcycle > INTERVAL_SEND_DATA) {
            msLastTestcycle = curMillis;
            if (!clientConnected) {
                return;
            }
            DEBUG_INFO("daten schicken");
            datenSchicken();
        }
    }
} 