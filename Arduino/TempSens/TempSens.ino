/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>

String deviceName = "M1";


//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
const char* mqtt_server = "192.168.204.26";

// GPIO where the DS18B20 is connected to
const int oneWireBus = 33;  
float sampleRate = 1; //Samples per second
float interval = 1000/sampleRate;

const int initialBatchSize = 10;

int batchSize = initialBatchSize;
int batchNum = 0;

float measurements[1000];

char ssid[] = "BimseNet";
char password[] = "vffj8352";

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (batchNum > batchSize-1) {
    Serial.println("Send batch");
    for (int i = 0; i < batchNum; i++) {
      Serial.println(measurements[i]); //TODO: Send data over HTTP
    }
    setBatchSize(batchSize);
  }
  
  sensors.requestTemperatures(); 
  measurements[batchNum] = sensors.getTempCByIndex(0);
  batchNum = batchNum + 1;
  
  delay(interval); //Amount of miliseconds between each meassurement
  
  
}

void setSampleRate(float samp) {
  sampleRate = samp;
  interval = 1000/sampleRate;

  Serial.print("New samplingrate: ");
  Serial.println(samp);
}

void setBatchSize(int newBatchSize) {
  batchSize = newBatchSize;
  //TODO: Send current measurements
  Serial.print("New batch size: ");
  Serial.println(newBatchSize);
  
  batchNum = 0;
}

void sendBatch() {
  
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("sensor/config");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}


void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  
  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "sensor/config") {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, messageTemp);

    int newBatchSize;
    int newSampleRate;

    for (int i = 0; i < sizeof(doc); i++) {
      if (doc[i] == deviceName) {
        newBatchSize = doc[i]["sensors"][0]["settings"]["batchSize"];
        newSampleRate = doc[i]["sensors"][0]["settings"]["samplingRate"];
      }
    }
    
    setSampleRate(newSampleRate);
    setBatchSize(newBatchSize);
  }
}

void json() {
  String jsonString = "[     {         \"name\": \"m1\",         \"sensors\": [             {                 \"name\": \"s1\",                 \"settings\": {                     \"samplingRate\": 100.0,                     \"batchSize\": 3000                 }             },             {                 \"name\": \"s2\",                 \"settings\": {                     \"samplingRate\": 3.3333333,                     \"batchSize\": 1000                 }             }         ]     },     {         \"name\": \"m2\",         \"sensors\": [             {                 \"name\": \"s1\",                 \"settings\": {                     \"samplingRate\": 1666.6666,                     \"batchSize\": 300                 }             }         ]     } ]";
  
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, jsonString);

  String nameM2 = doc[1]["name"];
  float sampleM2S1 = doc[1]["sensors"][0]["settings"]["samplingRate"];
  int batchM2S1 = doc[1]["sensors"][0]["settings"]["batchSize"];

  Serial.println(nameM2);
  Serial.println(sampleM2S1);
  Serial.println(batchM2S1);
}
