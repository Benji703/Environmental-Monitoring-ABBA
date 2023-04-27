#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <PubSubClient.h>
#include <NTPClient.h>
#include <WiFiUdp.h>



//For getting timestamp over internet
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

//MQTT
WiFiClient espClient;
PubSubClient client(espClient);
const char* mqtt_server = "192.168.242.26";
const char* client_id = "m1";

boolean isConfigured = false;

// GPIO where the DS18B20 is connected to
const int oneWireBus = 33;  
float sampleRate = 0; //Samples per second
float interval = 0;

int batchSize = 0;
int batchNum = 0;

unsigned long lastMeasureTime;
unsigned long lastHeartBeatTime;
int heartBeatInterval = 10000;

const int arraySize = 1000;
float measurements[arraySize];
unsigned long timeArr[arraySize];

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

  //allocateArray();
}



void loop() {
  reconnectIfNotConnected();
  
  if (isConfigured) {
    lastMeasureTime = 0;
    lastHeartBeatTime = 0;
  }
  while (isConfigured) {
    checkMQTT();
    if (millis() > lastMeasureTime + interval ) {
      measureTemp();
      lastMeasureTime = millis();
    }
    if (millis() > lastHeartBeatTime + heartBeatInterval) {
      sendHeartBeat();
      lastHeartBeatTime = millis();
    }
    if ( batchNum > batchSize-1) {
      Serial.println("Sending batch");
      sendBatch();
      Serial.println("Batch sent!");
    } 
      
  }
  checkMQTT();
}

void reconnectIfNotConnected() {
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Reconnecting to WiFi...");
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("Reconnected to WiFi");
    }
  }
}

void measureTemp() {
  timeClient.update();

  unsigned long currentTime = timeClient.getEpochTime();
  
  sensors.requestTemperatures(); 
  measurements[batchNum] = sensors.getTempCByIndex(0);
  timeArr[batchNum] = currentTime;
  batchNum = batchNum + 1;
}

void checkMQTT() {
  if (!client.connected()) {
     reconnect();
  }
  client.loop();
}

void customDelay(int delayMilli) {
  int startTime = millis();
  while (millis() > startTime + delayMilli) {
     if (!client.connected()) {
      reconnect();
    }
    client.loop();
  }
  //sendHeartBeat();
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
  reconnectIfNotConnected();
  
  String myOutput = "{ \"machine_id\": \"" + (String)client_id + "\", \"temperatures\": [";
  
  for (int i = 0; i < batchNum; i++) {
    if (i == batchNum-1) {
      myOutput += (String)measurements[i];
      break;
    }
    myOutput += (String)measurements[i] + ",";
  }
  
  myOutput += "], \"time_stamps\": [";

  for (int i = 0; i < batchNum; i++) {
    if (i == batchNum-1) {
      myOutput += (String)timeArr[i];
      break;
    }
    myOutput += (String)timeArr[i] + ",";
  }
  
  myOutput += "] }";

  //DynamicJsonDocument doc(1024);
  //doc["temperature"] = sendMes;
  //serializeJson(doc, myOutput);
  const char* payload = myOutput.c_str();
  client.publish("sensor/temperature", payload);
  
  //allocateArray();
  batchNum = 0;
}


void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(client_id)) {
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

    if (batchNum > 0) {
      sendBatch(); 
    }

    for (int i = 0; i < sizeof(doc); i++) {
      if (doc[i]["name"] == client_id) {
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

    checkConfigValidity();
    
  }

  if (String(topic) == "sensor/temperature") {
    Serial.println(messageTemp);
  }
}

void sendHeartBeat() {
  String heartbeat = "{ \"machine_id\": \"" + (String)client_id + "\" }";
  
  const char* payload = heartbeat.c_str();
  client.publish("heartbeats", payload);
}

//Checks if the config is valid to run on the esp32. If not, the isConfigured boolean is set to false
void checkConfigValidity() {
  if (batchSize > arraySize) {
    isConfigured = false; 
  } else {
    isConfigured = true; 
  }
}
/*
void allocateArray() {
   measurements = (float *) malloc(batchSize * sizeof(float));
    if (measurements == NULL) {
      Serial.println("Failed to allocate memory for measurements array");
      return;
    }
}
*/
