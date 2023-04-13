# Run Backend Services 
The backend services covers these three: 
- MQTT Broker 
- Flask Python server, which publish sensor configuration files, that are being send via entrypoint, to *sensor/config* topic.
- Python subscriber, which listen to the *sensor/termperature* topic.

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
```
mosquitto_pub -h localhost -t sensor/temperature -m 23
```
</br>

The *sensor/config* messages looks abit different, here is an example. 

Start subscriber that consume to the *sensor/config* topic in a new shell:
```
mosquitto_sub -h localhost -t "sensor/config"
```
Start a publisher that produce to the *sensor/config* topic in a new shell:
```
mosquitto_pub -h localhost -t sensor/config -m '[ { "name": "m1", "sensors": [ { "name": "s1", "settings": { "samplingRate": 100.0, "batchSize": 3000 } }, { "name": "s2", "settings": { "samplingRate": 3.3333333, "batchSize": 1000 } } ] }, { "name": "m2", "sensors": [ { "name": "s1", "settings": { "samplingRate": 1666.6666, "batchSize": 300 } } ] } ]'
```
### Test Entrypoint
Start subscriber that consume to the *sensor/config* topic in a new shell:
```
mosquitto_sub -h localhost -t "sensor/config"
```
Now run the following curl request: 
```
curl -X POST http://localhost:8080/config -H "Content-Type: application/json" -d '[ { "name": "m1", "sensors": [ { "name": "s1", "settings": { "samplingRate": 100.0, "batchSize": 3000 } }, { "name": "s2", "settings": { "samplingRate": 3.3333333, "batchSize": 1000 } } ] }, { "name": "m2", "sensors": [ { "name": "s1", "settings": { "samplingRate": 1666.6666, "batchSize": 300 } } ] } ]'
```

# Source
The origin of the MQTT broker comes from vvatelot's reposetory: *https://github.com/vvatelot/mosquitto-docker-compose*
