#include <WiFi.h>
#include <WiFiClientSecure.h> 
#include <PubSubClient.h>

const char* ssid = "NTH414"; // 포항 도시숲 WiFi 대체
const char* password = "cseenth414"; 

// MQTT Broker 정보
const char* mqtt_server = "6a862c626ea84addad97efb3a08f02de.s1.eu.hivemq.cloud"; 
const int mqtt_port = 8883; // TLS 포트

WiFiClientSecure espClient;   //  Secure로 변경
PubSubClient client(espClient);

#define VALVE_PIN  13   // D13 핀 사용

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int retry = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    retry++;
    if (retry > 30) {
      ESP.restart();
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // ✅ SSL 인증서 검증 OFF
  espClient.setInsecure(); 
}

void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.println("[MQTT] Message arrived [" + String(topic) + "] : " + message);

  if (String(topic) == "valve/1/control/right") {
    if (message == "ON") {
      digitalWrite(VALVE_PIN, HIGH);
      client.publish("valve/1/status", "ON");
    } else if (message == "OFF") {
      digitalWrite(VALVE_PIN, LOW);
      client.publish("valve/2/status", "OFF");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("Lora32_Client","Yiombi","Aa12345678")) {
      Serial.println("connected");
      client.subscribe("valve/1/control");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(VALVE_PIN, OUTPUT); 
  digitalWrite(VALVE_PIN, LOW);

  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(mqttCallback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
