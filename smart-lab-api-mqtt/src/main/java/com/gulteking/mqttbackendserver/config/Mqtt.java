package com.gulteking.mqttbackendserver.config;

import lombok.extern.slf4j.Slf4j;
import org.eclipse.paho.client.mqttv3.IMqttClient;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.springframework.context.annotation.Bean;
import org.springframework.stereotype.Component;

@Slf4j
public class Mqtt {

    //private static final String MQTT_PUBLISHER_ID = "clientId-w3mV3ouZo6";
    private static final String MQTT_PUBLISHER_ID = "JavaSample";
  // private static final String MQTT_SERVER_ADDRES= "tcp://dc036b40064d485da7d5106e54ab7c99.s1.eu.hivemq.cloud:8883";
    //private static final String MQTT_SERVER_ADDRES="tcp://mqtt.eclipseprojects.io:1883";
  private static final String MQTT_SERVER_ADDRES="tcp://localhost:1883";
    private static IMqttClient instance;
    //private static String pass = "Tomas1234";

    public static IMqttClient getInstance() {
        try {

              if (instance == null) {
            instance = new MqttClient(MQTT_SERVER_ADDRES, MQTT_PUBLISHER_ID);
            log.info("entre a instance");
              }

            MqttConnectOptions options = new MqttConnectOptions();
//            options.setPassword(pass.toCharArray());
//           options.setUserName("Tomas");
            options.setAutomaticReconnect(true);
            options.setCleanSession(true);
            options.setConnectionTimeout(10);


          if (!instance.isConnected()) {
            instance.connect(options);
            log.info("pase el .connect");
          }


        } catch (MqttException e) {
            log.info("no me pude conectar");
            e.printStackTrace();
        }

        return instance;
    }

    private Mqtt() {

    }
}
