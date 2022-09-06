package com.gulteking.mqttbackendserver.config;

import javax.annotation.PostConstruct;
import lombok.AllArgsConstructor;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.springframework.stereotype.Component;

@Component
@AllArgsConstructor
public class SensorsSubscriberComponent implements MqttCallback {

  private static final String clientId = "sensorsSubscriber";
  private static final String clientId2 = "dashboardPublisher";

  private static final String baseDashboardTopic = "tomabregu/feeds/";

  private static final String topic1 = "cortinas/status";
  private static final String topic2 = "onoff/status";
  private static final String topic3 = "dimmer/status";
  private static final String topic4 = "camara/status";

  private static final String topic5 = "tension/status";

  private static final String topic6 = "corriente/status";

  private static final int qos = 1;

  @PostConstruct
  public void subscribe() {
    try
    {
      MqttClient sampleClient2 = new MqttClient("tcp://localhost:1883", clientId);
      MqttConnectOptions connOpts = new MqttConnectOptions();
      connOpts.setCleanSession(true);
      System.out.println("checking");
      System.out.println("Mqtt Connecting to broker: " + "tcp://localhost:1883");
      sampleClient2.connect(connOpts);
      System.out.println("Mqtt Connected");
      sampleClient2.setCallback(this);
      sampleClient2.subscribe(topic1, qos);
      sampleClient2.subscribe(topic2, qos);
      sampleClient2.subscribe(topic3, qos);
      sampleClient2.subscribe(topic4, qos);
      sampleClient2.subscribe(topic5, qos);
      sampleClient2.subscribe(topic6, qos);
      System.out.println("Subscribed");
      System.out.println("Listening");
      if(!sampleClient2.isConnected())
        sampleClient2.reconnect();

    } catch (MqttException me) {
      System.out.println(me);
    }
  }

  @Override
  public void connectionLost(Throwable throwable) {

  }

  @Override
  public void messageArrived(String topic, MqttMessage message) throws Exception {
    System.out.println("Topic:" + topic);
    System.out.println("Message: " + message.toString());

    String newTopic = baseDashboardTopic.concat(topic).replace("/status","");
    System.out.println("topic: " + newTopic);
    MqttClient sampleClient3 = new MqttClient("tcp://io.adafruit.com:1883", clientId2);
    MqttConnectOptions connOpts = new MqttConnectOptions();
    connOpts.setUserName("tomabregu");
    connOpts.setPassword("aio_UtiP73L2fWdYG0Z3AunzjTFKdOvY".toCharArray());
    connOpts.setCleanSession(true);
    System.out.println("checking");
    System.out.println("Mqtt Connecting to broker: " + "tcp://io.adafruit.com:1883");
    connOpts.setCleanSession(true);
    System.out.println("checking");
    sampleClient3.connect(connOpts);
    sampleClient3.publish(newTopic,message);
    sampleClient3.disconnect();


  }

  @Override
  public void deliveryComplete(IMqttDeliveryToken iMqttDeliveryToken) {

  }
}
