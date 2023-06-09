from flask import Flask, request, jsonify
import time
import json

app = Flask(__name__)

@app.route('/')
def hello_world():
    return 'Hello, client!'

temperatures = []
heartbeats = []

@app.route('/receive-temperature', methods=['POST'])
def receive_temperature():
    if(len(temperatures) > 20):
        temperatures.pop(0)
    temperatures.append(request.json)
    #data.update(request.json)
    return 'Received Temperature'

@app.route('/show-temperature', methods=['GET'])
def show_temperature():
    return jsonify(temperatures)


@app.route('/receive-heartbeats', methods=['POST'])
def receive_heartbeats():
    #heartbeat = jsonify(machine_id=request.json, timestamp=time.time())
    heartbeats.append(request.json)
    return 'Received Heartbeat'

@app.route('/show-heartbeats', methods=['GET'])
def show_heartbeats():
    return jsonify(heartbeats)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port='3000', debug=True)

