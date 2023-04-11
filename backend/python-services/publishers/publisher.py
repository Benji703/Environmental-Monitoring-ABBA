from flask import Flask, request, jsonify
import paho.mqtt.client as mqtt
import time
import json

app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello, World!'

@app.route('/config', methods=['POST'])
def publish():
    client = mqtt.Client("configuration_publisher")
    client.connect("mosquitto", 1883)

    config = request.json

    #with open('./resources/configuration.json','r') as f:
    #    config = json.load(f)

    json_message = json.dumps(config)
    client.publish("sensor/config", json_message)
    #print("backend just published: " + str(json_message) + " to Topic: sensor/config")
    
    return jsonify({'task': 'publish', 'topic': "sensor/config", 'status': 'success'})
    # 'message': json_message,

if __name__ == '__main__':
    app.run(host='0.0.0.0', port='8080', debug=True)