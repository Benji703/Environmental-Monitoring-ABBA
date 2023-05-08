import paho.mqtt.client as mqtt 
from flask import Flask, request, jsonify
import requests
import time
import json

app = Flask(__name__)
headers = {"Content-Type": "application/json; charset=utf-8"}
url = 'http://client:3000/receive-temperature'

def on_connect(client, userdata, flags, rc):
    client.subscribe("sensor/temperature")
    

def on_message(client, userdata, message):
    print("The backend received message: ", str(message.payload.decode("utf-8")))
    parsed_json = json.loads(message.payload.decode("utf-8"))
    response = requests.post( url=url, headers=headers, json=parsed_json)
    return response

client = mqtt.Client("temperaturee_consumer")
client.on_connect = on_connect
client.on_message = on_message
client.connect("mosquitto", 1883)

client.loop_forever()

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=5000)
