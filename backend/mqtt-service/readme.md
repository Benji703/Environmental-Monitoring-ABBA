# Run service

UID=$UID GID=$GID docker-compose up -d

open new shell to subsribe to a topic: 
mosquitto_sub -h localhost -t "sensor/temperature"

open new shell to publish to a topic: 
mosquitto_pub -h localhost -t sensor/temperature -m 23

#reference: https://github.com/vvatelot/mosquitto-docker-compose