/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com  
*********/

#include <OneWire.h>
#include <DallasTemperature.h>

// GPIO where the DS18B20 is connected to
const int oneWireBus = 33;  
float sampleRate = 1; //Samples per second
float interval = 1000/sampleRate;

const int initialBatchSize = 5;

int batchSize = initialBatchSize;
int batchNum = 1;

float measurements[initialBatchSize];

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
  
  if (batchNum > batchSize) {
    Serial.println("Send batch");
    batchNum = 1;
  }
  
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  delay(interval); //Amount of miliseconds between each meassurement
  setSampleRate(sampleRate+1);

  batchNum = batchNum + 1;
}

void setSampleRate(float samp) {
  sampleRate = samp;
  interval = 1000/sampleRate;
}
