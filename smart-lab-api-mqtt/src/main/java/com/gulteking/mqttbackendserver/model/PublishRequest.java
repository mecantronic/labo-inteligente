package com.gulteking.mqttbackendserver.model;

import com.fasterxml.jackson.annotation.JsonIgnore;
import lombok.Data;

@Data
public class PublishRequest {

  private String topic;
  private String value;
}
