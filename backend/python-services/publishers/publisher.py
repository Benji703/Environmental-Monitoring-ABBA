import paho.mqtt.client as mqtt 
#from random import uniform
import time
import json

#mqttBroker = "mqtt.eclipseprojects.io"
#client.connect(mqttBroker)
client = mqtt.Client("configuration_publisher")
client.connect("mosquitto", 1883)

with open('./resources/configuration.json','r') as f:
    config = json.load(f)

json_message = json.dumps(config)

while True:
    #randNumber = uniform(20.0, 21.0)
    client.publish("sensor/config", json_message)
    print("backend just published: " + str(json_message) + " to Topic: sensor/temperature")
    time.sleep(1)
