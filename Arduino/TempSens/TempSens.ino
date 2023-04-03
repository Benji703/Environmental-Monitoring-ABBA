/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFi.h>
#include <HTTPClient.h>
  #include <ArduinoJson.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 33;  
float sampleRate = 1; //Samples per second
float interval = 1000/sampleRate;

const int initialBatchSize = 50;

int batchSize = initialBatchSize;
int batchNum = 0;

float measurements[1000];

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

void setup() {
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
}

void loop() {
  
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
  setSampleRate(sampleRate+1);

  
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
  
}
