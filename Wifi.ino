#define WLAN_SSID "generic_SSID" //in case of fail
#define WLAN_PASS "generic_password"

const uint8_t knownWLANs = 3;

//Picks the first one off the list. If more than one is available, the highest in the list is chosen.
const char* WLAN_SSIDs[][2] = {
   {"SSID_the_first", "p4ssw0rd"},
   {"SSID_the_second", "swordfish"},
   {"SSID_the_third", "hunter2"},
};


void wifi_ManualSetup(const char* ssid = WLAN_SSID, const char* pass = WLAN_PASS) {
  sprintf(hostString, "ESP_%06X", ESP.getChipId());
  
  WiFi.hostname(hostString);

  // Connect to WiFi access point.
  Serial.println();
  Serial.print(F("Connecting to "));
  Serial.print(ssid);
  Serial.print(F(" as: "));
  Serial.println(hostString);

  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED, !digitalRead(LED));
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  digitalWrite(LED, HIGH); //apparently is active low
  analogWrite(GREEN, 32);

  Serial.print(F("WiFi connected. IP address: "));
  Serial.println(WiFi.localIP());
  Serial.println(F("\n********************************************************************\n"));
}


void wifi_AutoSetup() {
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  int k = -1;
  Serial.print("Scanning for Wifi networks... ");

  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  Serial.print(F("done. "));
  if (n == 0) {
    Serial.println(F("No networks found.")); //should start its own AP with blackjack and hookers
  } else {
    Serial.print(n);
    Serial.println(F(" networks found:"));

    for (int i = 0; i < n; ++i) {
      // Print SSID and RSSI for each network found
      Serial.print('\t');
      Serial.print(i + 1);
      Serial.print(":\t");
      Serial.print(WiFi.SSID(i));
      Serial.print("\t\t(");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);

      for (int j = 0; j < knownWLANs; j++) {
        if (WiFi.SSID(i) == WLAN_SSIDs[j][0]) {
          k = j;
          break;
        }
      }
    }
  }
  
  if (k != -1) {
    wifi_ManualSetup(WLAN_SSIDs[k][0], WLAN_SSIDs[k][1]);
  } else {
    //this is where we'd set up our own AP... IF WE HAD ONE
    Serial.println(F("\nNo known networks in range, starting own AP."));
    //Serial.println("");  
  }
}
