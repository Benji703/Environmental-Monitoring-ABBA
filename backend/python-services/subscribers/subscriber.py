import paho.mqtt.client as mqtt 
from flask import Flask, request, jsonify
import requests
import time
import json

app = Flask(__name__)

def on_message(client, userdata, message):
    parsed_json = json.loads(message.payload.decode("utf-8"))
    headers = {"Content-Type": "application/json; charset=utf-8"}
    #url = 
    response = requests.post( url='http://client:3000/receive-json', headers=headers, json=parsed_json)
    return response
    #print("The backend received message: ", str(message.payload.decode("utf-8")))

client = mqtt.Client("temperature_consumer")
client.connect("mosquitto", 1883)

client.loop_start()
client.subscribe("sensor/temperature")
client.on_message = on_message
time.sleep(1000)
client.loop_end()

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
