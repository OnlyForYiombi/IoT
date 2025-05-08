#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiClientSecure.h> 
#include <PubSubClient.h>
// WiFi Configuration 
const char* ssid = "NTH414";
const char* password = "cseenth414";

// MQTT Broker 정보
const char* mqtt_server = "6a862c626ea84addad97efb3a08f02de.s1.eu.hivemq.cloud"; 
const int mqtt_port = 8883; // TLS 포트

WiFiClientSecure espClient;   //  Secure로 변경
PubSubClient client(espClient);

// OLED Configuration
#define SCREEN_WIDTH    128
#define SCREEN_HEIGHT   64
#define OLED_RESET      -1
#define OLED_ADDR       0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Valve configuration 
#define VALVE_PIN1    25  // LEFT
#define VALVE_PIN2    21  // RIGHT


void mqttCallback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (unsigned int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  Serial.println("[MQTT] Message arrived [" + String(topic) + "] : " + message);
  
  if(String(topic) == "valve/1/right") {
    if(message == "ON"){
      digitalWrite(VALVE_PIN2, HIGH);
      Serial.println("[VALVE1-RIGHT] ON");
    } else if(message == "OFF"){
      digitalWrite(VALVE_PIN2, LOW);
      Serial.println("[VALVE1-RIGHT] OFF");
    }
  } else if(String(topic) == "valve/1/left") {
    if(message == "ON"){
      digitalWrite(VALVE_PIN1, HIGH);
      Serial.println("[VALVE1-LEFT] ON");
    } else if(message == "OFF"){
      digitalWrite(VALVE_PIN1, LOW);
      Serial.println("[VALVE1-LEFT] OFF");
    }
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("Lora32_Client","Yiombi","Aa12345678")) {
      Serial.println("connected");
      client.subscribe("valve/1/right");
      client.subscribe("valve/1/left");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

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

  espClient.setInsecure(); 
}


void setup() {
  Serial.begin(115200);
  pinMode(VALVE_PIN1, OUTPUT);
  pinMode(VALVE_PIN2, OUTPUT);
  digitalWrite(VALVE_PIN1, LOW);
  digitalWrite(VALVE_PIN2, LOW);

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

