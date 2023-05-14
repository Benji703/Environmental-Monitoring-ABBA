# ESP32
The Arduino IDE is used... introduction **something** **something** **something**

There are two sections: 
- [Setup](#setup)
- [Ready to run](#ready-to-run)

## Setup

### Install Libraries
Navigate to **Manage Libraries**: 
Tools -> Manage Libraries

Thereafter find the following Libraries: 
- *PubSubClient*
- *NTPClient*
- *ArduinoJson*
- *DallasTemperature* 
  - Note: *DallasTemperature* might need other dependencies, if so, "**Install All**". 

*OneWire* and *WiFi* should be ready by default, otherwise download it. 

### Install Board Manager 
If this is the first time running ESP32 on the machine, we need to add the board to the Arduino Manager, otherwise skip this point.

Navigate to **Additional Boards Manager URLs**
File -> Preference -> Settings -> Additional Board Manager URLs (field).


Insert the following into the field. 
```
https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
```

Navigate to **Boards Manager**
Tools -> Board: "\<current board>" -> Boards Manager
- Note: might be *Arduino Uno*. 

Now search for **ESP32** in the *Boards Manager* and install the search result.

### Select Board Manager 

Navigate to **Boards Manager**
Tools -> Board: "\<current board>" 
- Note: might be *Arduino Uno*.

Now select: **DOIT ESP32 DEVKIT V1** 

### Select Port 

Insert the ESP32 via USB into the machine. 

Navigate to **Port**
Tools -> Port "\<current port>" 

Now select the USB port in which the ESP32 is connected to the machine. 

## Ready to Run

Now the program is runnable. 

To test the *temperature measurement* and the *config loading* head to [Test Backend](../Backend/readme.md).
