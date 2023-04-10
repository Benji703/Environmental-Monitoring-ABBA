import paho.mqtt.client as mqtt 
#from random import uniform
import time
import json

def on_message(client, userdata, message):
    parsed_json = json.loads(message.payload.decode("utf-8"))
    print(parsed_json)
    #print("The backend received message: ", str(message.payload.decode("utf-8")))


#mqttBroker = "mqtt.eclipseprojects.io"
client = mqtt.Client("backend")
#client.connect(mqttBroker)'
client.connect("127.0.0.1", 1883)

client.loop_start()
client.subscribe("sensor/temperature")
client.on_message = on_message
time.sleep(1000)
client.loop_end()

