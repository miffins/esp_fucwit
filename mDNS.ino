/************************* backup MQTT server address for if mDNS fucks up *********************************/
const char* AIO_SERVER = "192.168.0.31"; //this happens to be my pi's wired ethernet address.
const uint16_t AIO_SERVERPORT = 1883;    // use 8883 for SSL

void mDNSSetup() {
  if (!MDNS.begin(hostString)) {
    Serial.println(F("Error setting up MDNS responder!"));
  }
  MDNS.addService("esp", "tcp", 8080); // Announce esp tcp service on port 8080
  Serial.println(F("mDNS service started."));
  mDNSServiceQuery("esp");
}


int mDNSServiceQuery(String what) {
  Serial.print(F("Sending mDNS query for "));
  Serial.print(what);
  Serial.print(F(" servers... "));
  int n = MDNS.queryService(what, "tcp"); // Send out query for esp tcp services
  
  if (n == 0) {
    Serial.println(F("no services found."));
  } else {
    Serial.print(F("done. "));
    Serial.print(n);
    if (n>1) {
      Serial.println(F(" services found."));
    } else {
      Serial.println(F(" service found."));
    }
    for (int i = 0; i < n; ++i) {
      // Print details for each service found
      Serial.print('\t');
      Serial.print(i + 1);
      Serial.print(":\t");
      Serial.print(MDNS.hostname(i));
      Serial.print("\t\t(");
      Serial.print(MDNS.IP(i));
      Serial.print(":");
      Serial.print(MDNS.port(i));
      Serial.println(")");
    }
  }
  return n;
}




void MQTT_connect_via_mDNS(String &_ip, uint16_t* _port, byte* ipAsByteArray) {
  int cnt = mDNSServiceQuery("mqtt-broker");
  if (cnt != 0) {
    char ip[16];
    sprintf(ip, "%d.%d.%d.%d", MDNS.IP(0)[0], MDNS.IP(0)[1], MDNS.IP(0)[2], MDNS.IP(0)[3]);
    for (int x = 0; x < 4; x++) {
      ipAsByteArray[x] = MDNS.IP(0)[x];
    }
    _ip = String(ip);
    *_port = MDNS.port(0);
  } else {
    Serial.println(F("Couldn't find any MQTT servers via mDNS. Going with hardcoded one."));
    _ip = AIO_SERVER;
    *_port = AIO_SERVERPORT;
  }
}
