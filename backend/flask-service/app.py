from flask import Flask, render_template
import os
import paho.mqtt.client as mqtt 
import time

app = Flask(__name__)

host_ip = '127.0.0.1'
port = 1883 
keepalive = 60

@app.route('/')
def home():
    #return render_template('index.html')

    topic = 'temperature'
    client = mqtt.Client()
    client.connect(host_ip, port, keepalive)

    i = 0
    while True:
        time.sleep(5)
        i+=1
        message = str(i)
        if i <=10:
            client.publish(topic, message)
        else:
            break
    client.disconnect()


    return ""


if __name__ == "__main__":
    port = int(os.environ.get('PORT', 5000))
    app.run(debug=True, host='0.0.0.0', port=port)


