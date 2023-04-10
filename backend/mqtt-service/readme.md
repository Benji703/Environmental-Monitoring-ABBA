# Run service

UID=$UID GID=$GID docker-compose up -d 

open new shell to subsribe to a topic: 
mosquitto_sub -h localhost -t "sensor/temperature"

open new shell to publish to a topic: 
mosquitto_pub -h localhost -t sensor/temperature -m 23


open new shell to subsribe to a topic: 
mosquitto_sub -h localhost -t "sensor/config"

open new shell to publish to a topic: 
mosquitto_pub -h localhost -t sensor/config -m [ { "name": "m1", "sensors": [ { "name": "s1", "settings": { "samplingRate": 100.0, "batchSize": 3000 } }, { "name": "s2", "settings": { "samplingRate": 3.3333333, "batchSize": 1000 } } ] }, { "name": "m2", "sensors": [ { "name": "s1", "settings": { "samplingRate": 1666.6666, "batchSize": 300 } } ] } ]



#reference: https://github.com/vvatelot/mosquitto-docker-compose