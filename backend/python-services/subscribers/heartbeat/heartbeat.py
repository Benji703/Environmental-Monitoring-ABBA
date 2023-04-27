import paho.mqtt.client as mqtt 
from flask import Flask, request, jsonify
import requests
import time
import json

app = Flask(__name__)
headers = {"Content-Type": "application/json; charset=utf-8"}
url = 'http://client:3000/receive-heartbeats'

def on_message(client, userdata, message):
    parsed_json = json.loads(message.payload.decode("utf-8"))
    response = requests.post( url=url, headers=headers, json=parsed_json)
    return response

client = mqtt.Client("heartbeat_consumer")
client.connect("mosquitto", 1883)

client.loop_start()
client.subscribe("heartbeats")
client.on_message = on_message
time.sleep(1000)
client.loop_end()

if __name__ == '__main__':
    app.run(debug=True, host='0.0.0.0', port=6000)
