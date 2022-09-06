package com.gulteking.mqttbackendserver.controller;

import com.gulteking.mqttbackendserver.model.MqttSubscribeModel;
import com.gulteking.mqttbackendserver.model.PublishRequest;
import java.util.List;
import java.util.Objects;
import lombok.AllArgsConstructor;
import lombok.extern.slf4j.Slf4j;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;

@Slf4j
@RestController
@RequestMapping(value = "/publish")
@AllArgsConstructor
public class BotController {

  private static final String sensorBrokerUrl ="tcp://localhost:1883";
  private static final String clientId3 = "botPublisher";

  private static final int qos = 0;

  void publishMqttMessage(String topic, String value) throws MqttException {
    MqttClient sampleClient3 = new MqttClient(sensorBrokerUrl, clientId3);
    MqttConnectOptions connOpts = new MqttConnectOptions();
    connOpts.setCleanSession(true);
    System.out.println("checking");
    sampleClient3.connect(connOpts);
    MqttMessage message = new MqttMessage( value.getBytes());
    log.info("Body del Request del mensaje : " + message);
    sampleClient3.publish(topic,message.getPayload(), qos, true);
    sampleClient3.disconnect();
  }
  @PostMapping
  public ResponseEntity publishMessage(@RequestBody PublishRequest publishRequest)
      throws MqttException {
    log.info("Body del Request del bot: "+ publishRequest.toString());
    this.publishMqttMessage(publishRequest.getTopic(), publishRequest.getValue());
    return new ResponseEntity<>(HttpStatus.OK);

  }


  @PostMapping("/scenes")
  public ResponseEntity publishScene(@RequestBody PublishRequest publishRequest)
      throws MqttException {
    log.info("Body del Request del bot: "+ publishRequest.toString());

    if(Objects.equals(publishRequest.getTopic(), "chau")){
      this.publishMqttMessage("dimmer/set","10");
      this.publishMqttMessage("cortinas/set","100");
      this.publishMqttMessage("onoff/set","off");
      this.publishMqttMessage("red/set","0");
      this.publishMqttMessage("green/set","0");
      this.publishMqttMessage("blue/set","0");
    }

    if(Objects.equals(publishRequest.getTopic(), "hola")){
      this.publishMqttMessage("dimmer/set","90");
      this.publishMqttMessage("cortinas/set","1");
      this.publishMqttMessage("onoff/set","off");
      this.publishMqttMessage("red/set","100");
      this.publishMqttMessage("green/set","0");
      this.publishMqttMessage("blue/set","100");
    }
    return new ResponseEntity<>(HttpStatus.OK);

  }
}
