from flask import Flask, request, jsonify
import paho.mqtt.client as mqtt
import time
import json

app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello, client!'

data = {}

@app.route('/receive-json', methods=['POST'])
def receive_json():
    data.update(request.json)
    return 'Received JSON'

@app.route('/show-json', methods=['GET'])
def show_json():
    return jsonify(data)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port='3000', debug=True)

