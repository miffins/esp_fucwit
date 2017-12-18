void registerCommands() {
  Serial.print(F("Initialising command parser..."));
  cmdInitmk2();
  cmdAdd("ping", cmdPing);
  cmdAdd("state", cmdState);
  cmdAdd("RGB", cmdRGB);
  cmdAdd("LED", cmdLED);
  cmdAdd("CH1", cmdCH1);
  cmdAdd("CH2", cmdCH2);
  cmdAdd("cycle", cmdCycle);
  Serial.println(F("done!"));
}

void mqtt_handler(char* topic, byte* payload, unsigned int length) {
  String msg = (char*)payload;
  msg.remove(length);
  
  if (msg != "") {
    Serial.print(F("Got: "));
    Serial.println(msg);
    if (cmdParse(msg.c_str(), length)) {
      return;
    } else {
      SendMsg(F("Didn't understand that command."));
    }
  }
}

void cmdPing(int arg_cnt, char **args) {
  SendMsg("Pong.");
}

void cmdState(int arg_cnt, char **args) {
  String resp;
  if (arg_cnt < 2) {
    PrintStatus();
    PrintLEDStatus();
  } else {
    state = atoi(args[1]);
    stateUpdater();
  }
}


void cmdRGB(int arg_cnt, char **args) {
  String resp;
  if (arg_cnt == 4) {
    resp += "Setting LED to: RGB(";
    resp += args[1];
    resp += ", ";
    resp += args[2];
    resp += ", ";
    resp += args[3];
    resp += ")";
    SendMsg(resp);
    SetRGB(atoi(args[1]), atoi(args[2]), atoi(args[3]));
  } else {
    SendMsg("RGB command requires three arguments exactly.");
  }
}


void cmdLED(int arg_cnt, char **args) {
  if (arg_cnt > 1) {
    if (strcmp(args[1], "ON") == 0) {
        digitalWrite(LED, LOW);
    } else {
        digitalWrite(LED, HIGH);
    }
  }
  PrintLEDStatus();
}


void cmdCH1(int arg_cnt, char **args) {
  if (arg_cnt > 1) {
    if (strcmp(args[1], "ON") == 0) {
        digitalWrite(CH1pin, HIGH);
    } else {
        digitalWrite(CH1pin, LOW);
    }
    state = whatStateIsThis();
    stateUpdater(); //this function does the status printing as well
  }
}


void cmdCH2(int arg_cnt, char **args) {
  if (arg_cnt > 1) {
    if (strcmp(args[1], "ON") == 0) {
        digitalWrite(CH2pin, HIGH);
    } else {
        digitalWrite(CH2pin, LOW);
    }
    state = whatStateIsThis();
    stateUpdater(); //this function does the status printing as well
  }
}


void cmdCycle(int arg_cnt, char **args) {
  state++;
  stateUpdater();
}
