#from flask import Flask, jsonify
import paho.mqtt.client as mqtt 
from random import uniform
import time

#app = Flask(__name__)

#host_ip = '127.0.0.1'
#port = 1883 
#keepalive = 60

#mqttBroker = "mqtt.eclipseprojects.io"
client = mqtt.Client("Temperature_collector")
#client.connect(mqttBroker)
client.connect("127.0.0.1", 1883)
#client.connect(host="127.0.0.1", port=1883)

#topic = 'temperature'
#client = mqtt.Client()
#client.connect(host_ip, port, keepalive)

while True:
    randNumber = uniform(20.0, 21.0)
    client.publish("sensor/temperature", randNumber)
    print("backend just published: " + str(randNumber) + " to Topic: sensor/temperature")
    time.sleep(1)
#
#def on_message(client, userdata, message):
#    # Do something with the message here
#    print(message.topic, message.payload)
#
#client.on_message = on_message
##"127.0.0.1", 1883
#client.subscribe("sensor/temperature")
#
#client.loop_start()
#
#@app.route('/messages')
#def get_messages():
#    # Return the messages here
#    return jsonify({'messages': messages})
#
#if __name__ == "__main__":
#    app.run(debug=True, host='127.0.0.1', port=8080)
#
#