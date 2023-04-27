# Run Backend Services 
The backend services covers these five services: 
- MQTT Broker, which communicate with ESP machine, see more [ESP32](../Arduino/readme.md).
- Flask Python publisher, which publish the configuration files, that are being send via entrypoint, to *sensor/config* topic.
- Flask Python subscriber, which consume from the *sensor/termperature* topic and exposes temperature to the Client service.
- Flask Python subscriber, which consume from the *heartbeats* topic and exposes heartbeats to the Client service. 
- Client Flask service, which shows newest consmed data. 


To start these servies run the following docker-compose command: 

```
UID=$UID GID=$GID docker-compose up -d 
```
If there are changes made to the python files, remember to rebuild the docker-compose with the ```--build ``` optional parameter. 

## Test Services 

The test is split into: 
- Test MQTT
- Test entrypoint 

### Test MQTT

Start subscriber that consume to the *sensor/temperature* topic in a new shell:
```
mosquitto_sub -h localhost -t "sensor/temperature"
```
Start a publisher that produce to the *sensor/temerature* topic in a new shell:
TODO: UPDATE MESSAGE

```
mosquitto_pub -h localhost -t sensor/temperature -m "{ "machine_id": "m2", "temperatures": [22.37,22.37,22.37], "time_stamps": [1682583024,1682583025,1682583026] }"
```
</br>

The *sensor/config* messages looks abit different, here is an example. 

Start subscriber that consume to the *sensor/config* topic in a new shell:
```
mosquitto_sub -h localhost -t "sensor/config"
```
Start a publisher that produce to the *sensor/config* topic in a new shell:
TODO: UPDATE MESSAGE
```
mosquitto_pub -h localhost -t sensor/config -m '[ { "name": "m1", "sensors": [ { "name": "s1", "settings": { "samplingRate": 10.0, "batchSize": 30 } }, { "name": "s2", "settings": { "samplingRate" 100, "batchSize": 1000 } } ] }, { "name": "m2", "sensors": [ { "name": "s1", "settings": { "samplingRate": 20, "batchSize": 200 } } ] } ]'
```
</br>

Start subscriber that consume to the *heartbeats* topic in a new shell:
```
mosquitto_sub -h localhost -t "heartbeats"
```
Start a publisher that produce to the *heartbeats topic in a new shell:

TODO: UPDATE MESSAGE
```
mosquitto_pub -h localhost -t "heartbeats" -m "{ "machine_id": "m2" }"
```

### Test Entrypoint
Start subscriber that consume to the *sensor/config* topic in a new shell:
```
mosquitto_sub -h localhost -t "sensor/config"
```
Now run the following curl request: 
TODO: UPDATE MESSAGE
```
curl -X POST http://localhost:8080/config -H "Content-Type: application/json" -d '[ { "name": "m1", "sensors": [ { "name": "s1", "settings": { "samplingRate": 10.0, "batchSize": 30 } }, { "name": "s2", "settings": { "samplingRate" 100, "batchSize": 1000 } } ] }, { "name": "m2", "sensors": [ { "name": "s1", "settings": { "samplingRate": 20, "batchSize": 200 } } ] } ]'
```


# Source
The origin of the MQTT broker comes from vvatelot's reposetory: *https://github.com/vvatelot/mosquitto-docker-compose*

 <!--- curl -X POST http://localhost:3000/receive-temperature -H "Content-Type: application/json" -d '{ "name": "m2", "temperatures": [23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06,23.06] }
'
--->
