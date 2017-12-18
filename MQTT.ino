void MQTT_reconnect() {
  // Loop until we're reconnected
  while (!mqtt.connected()) {
    Serial.print(F("Connecting to MQTT broker... "));
    // Attempt to connect
    if (mqtt.connect(hostString)) {
      Serial.println(F("connected."));
      // Once connected, publish an announcement...
      //char concatBuffer[90];
      //sprintf(concatBuffer, "%s %s", hostString, F("online."));
      mqtt.publish(mqttChannelOut, "ESP online.");
      // ... and resubscribe
      mqtt.subscribe(mqttChannelIn);
    } else {
      Serial.print(F("failed, rc="));
      Serial.print(mqtt.state());
      Serial.println(F(" try again in 5 seconds"));
      // Wait 5 seconds before retrying
      for (int x = 0; x < 20; x++) {
        analogWrite(GREEN, ((x % 2 == 0) ? 128 : 0));
        btn.update();
        if (btn.read()) {
          Serial.print(F("User quit MQTT connect loop with button. Will stop trying for a bit."));
          lastConnectAttempt = millis();
          stateUpdater();
          return;
        }
        delay(250);
      }
    }
  }
  analogWrite(GREEN, 255);
}

void SendMsg(String loMsg) {
  SendMsg(mqttChannelOut, loMsg);
}

void SendMsg(String loTopic, String loMsg) {
  Serial.print(loMsg);
  if (!mqtt.publish((char*) loTopic.c_str(), (char*) loMsg.c_str()) ) {
    Serial.print(F(" (MQTT Publish Failed!)"));
  }
  Serial.println();
}

