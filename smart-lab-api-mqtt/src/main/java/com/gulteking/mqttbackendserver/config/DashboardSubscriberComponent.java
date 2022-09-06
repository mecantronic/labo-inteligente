package com.gulteking.mqttbackendserver.config;

import javax.annotation.PostConstruct;
import lombok.AllArgsConstructor;
import org.eclipse.paho.client.mqttv3.IMqttDeliveryToken;
import org.eclipse.paho.client.mqttv3.MqttCallback;
import org.eclipse.paho.client.mqttv3.MqttClient;
import org.eclipse.paho.client.mqttv3.MqttConnectOptions;
import org.eclipse.paho.client.mqttv3.MqttException;
import org.eclipse.paho.client.mqttv3.MqttMessage;
import org.eclipse.paho.client.mqttv3.persist.MemoryPersistence;
import org.springframework.stereotype.Component;

@Component
@AllArgsConstructor
public class DashboardSubscriberComponent implements MqttCallback {
    /** The broker url. */
    private static final String sensorBrokerUrl ="tcp://localhost:1883";
  private static final String dashboardBrokerUrl ="tcp://io.adafruit.com:1883";
    /** The client id. */
    private static final String clientId = "dashboardSubscriber";
  private static final String clientId2 = "sensorPublisher";

  private static final String topic1 = "tomabregu/feeds/cortinas";
  private static final String topic2 = "tomabregu/feeds/onoff";
  private static final String topic3 = "tomabregu/feeds/dimmer";
  private static final String topic4 = "tomabregu/feeds/camara";

  private static final String topic5 = "tomabregu/feeds/red";

  private static final String topic6 = "tomabregu/feeds/green";

  private static final String topic7 = "tomabregu/feeds/blue";

  private static final int qos = 1;

    @PostConstruct
    public void subscribe() {
      //    logger file name and pattern to log
      MemoryPersistence persistence = new MemoryPersistence();
      try
      {
        MqttClient sampleClient2 = new MqttClient(dashboardBrokerUrl, clientId, persistence);
        MqttConnectOptions connOpts = new MqttConnectOptions();
        connOpts.setUserName("tomabregu");
        connOpts.setPassword("aio_UtiP73L2fWdYG0Z3AunzjTFKdOvY".toCharArray());
        connOpts.setCleanSession(true);
        System.out.println("checking");
        System.out.println("Mqtt Connecting to broker: " + "tcp://io.adafruit.com:1883");
        sampleClient2.connect(connOpts);
        System.out.println("Mqtt Connected");
        sampleClient2.setCallback(this);
        sampleClient2.subscribe(topic1);
        sampleClient2.subscribe(topic2);
        sampleClient2.subscribe(topic3);
        sampleClient2.subscribe(topic4);
        sampleClient2.subscribe(topic5);
        sampleClient2.subscribe(topic6);
        sampleClient2.subscribe(topic7);
        System.out.println("Subscribed");
        System.out.println("Listening");
        if(!sampleClient2.isConnected())
             sampleClient2.reconnect();

      } catch (MqttException me) {
        System.out.println(me);
      }
    }
    //Called when the client lost the connection to the broker
    public void connectionLost(Throwable arg0) {

    }
    //Called when a outgoing publish is complete
    public void deliveryComplete(IMqttDeliveryToken arg0) {
    }
    public void messageArrived(String topic, MqttMessage message) throws MqttException {
      System.out.println("Topic:" + topic);
      System.out.println("Message: " + message.toString());

      String newTopic = topic.toLowerCase().replace("tomabregu/feeds/","").concat("/set");
      System.out.println("topic: " + newTopic);
      MqttClient sampleClient3 = new MqttClient(sensorBrokerUrl, clientId2);
      MqttConnectOptions connOpts = new MqttConnectOptions();
      connOpts.setCleanSession(true);
      System.out.println("checking");
      sampleClient3.connect(connOpts);
      sampleClient3.publish(newTopic,message.getPayload(), qos, true);
      sampleClient3.disconnect();

      }




}
