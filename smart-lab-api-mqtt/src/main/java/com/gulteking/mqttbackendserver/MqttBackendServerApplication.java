package com.gulteking.mqttbackendserver;

import com.gulteking.mqttbackendserver.config.DashboardSubscriberComponent;
import lombok.AllArgsConstructor;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

@SpringBootApplication
@AllArgsConstructor
public class MqttBackendServerApplication {

    private final DashboardSubscriberComponent dashboardSubscriberComponent;

    public static void main(String[] args) {
        SpringApplication.run(MqttBackendServerApplication.class, args);

    }


//    @PostConstruct
//    public void a(){
//        String topic        = "MQTT Examples";
//        String content      = "Message from MqttPublishSample";
//        int qos             = 2;
//        String broker       = "tcp://mqtt.eclipseprojects.io:1883";
//        String clientId     = "JavaSample";
//        //MemoryPersistence persistence = new MemoryPersistence();
//
//        try {
//            MqttClient sampleClient = new MqttClient(broker, clientId);
//            MqttConnectOptions connOpts = new MqttConnectOptions();
//            connOpts.setCleanSession(true);
//            System.out.println("Connecting to broker: "+broker);
//            sampleClient.connect(connOpts);
//            System.out.println("Connected");
//            System.out.println("Publishing message: "+content);
//            MqttMessage message = new MqttMessage(content.getBytes());
//            message.setQos(qos);
//            sampleClient.publish(topic, message);
//            System.out.println("Message published");
//            sampleClient.disconnect();
//            System.out.println("Disconnected");
//            System.exit(0);
//        } catch(MqttException me) {
//            System.out.println("reason "+me.getReasonCode());
//            System.out.println("msg "+me.getMessage());
//            System.out.println("loc "+me.getLocalizedMessage());
//            System.out.println("cause "+me.getCause());
//            System.out.println("excep "+me);
//            me.printStackTrace();
//        }
//    }
}