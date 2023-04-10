/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


String deviceName = "m1";

//For getting timestamp over internet
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

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

float* measurements;

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
  client.setBufferSize(512);

  timeClient.begin();

  allocateArray();
}

void loop() {
  
  if (!client.connected()) {
     reconnect();
  }
  client.loop();

  timeClient.update();

  int currentTime = timeClient.getEpochTime();
  Serial.println(currentTime);
  
  
  if (batchNum > batchSize-1) {
    Serial.println("Send batch");
    sendBatch();
    Serial.println("Batch sent!");
    setBatchSize(batchSize);
  }
  
  sensors.requestTemperatures(); 
  measurements[batchNum] = sensors.getTempCByIndex(0);
  batchNum = batchNum + 1;

  customDelay(interval); //Amount of miliseconds between each meassurement
  
  
}

void customDelay(int delayMilli) {
  int startTime = millis();
  while (millis() > startTime + delayMilli) {
     if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }
}

void setSampleRate(float samp) {
  sampleRate = samp;
  interval = 1000/sampleRate;
}

void setBatchSize(int newBatchSize) {
  batchSize = newBatchSize;
  //TODO: Send current measurements
  batchNum = 0;
}

void sendBatch() {

  String myOutput = "{ \"name\": \"" + deviceName + "\", \"temperatures\": [";
  
  for (int i = 0; i < batchNum; i++) {
    if (i == batchNum-1) {
      myOutput += (String)measurements[i];
      break;
    }
    myOutput += (String)measurements[i] + ",";
  }

  myOutput += "] }";

  //DynamicJsonDocument doc(1024);
  //doc["temperature"] = sendMes;
  //serializeJson(doc, myOutput);
  const char* payload = myOutput.c_str();
  client.publish("sensor/temperature", payload);

  allocateArray();
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
  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "sensor/config") {
    DynamicJsonDocument doc(1024);
    deserializeJson(doc, messageTemp);

    int newBatchSize;
    float newSampleRate;

    sendBatch();

    for (int i = 0; i < sizeof(doc); i++) {
      if (doc[i]["name"] == deviceName) {
        newBatchSize = (int) doc[i]["sensors"][0]["settings"]["batchSize"];
        newSampleRate = (float) doc[i]["sensors"][0]["settings"]["samplingRate"];
      }
    }

    if (newSampleRate != NULL) {
      setSampleRate(newSampleRate);
    }
    if (newBatchSize != NULL) {
      setBatchSize(newBatchSize);
    }

    
  }

  if (String(topic) == "sensor/temperature") {
    Serial.println(messageTemp);
  }
}

void allocateArray() {
  measurements = (float*) realloc(measurements, batchSize * sizeof(float));
    if (measurements == NULL) {
      Serial.println("Failed to allocate memory for measurements array");
      return;
    }
}
