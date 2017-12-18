
//============================================================================

void stateUpdater() {
   static int oldState = 0;
   if (state != oldState) {
     String resp = F("State is ");
     switch (state) {
        case 1: //0 is reserved - HW on.
          SetRGB(0, 0, 255);
          digitalWrite(CH1pin, true);
          digitalWrite(CH2pin, false);
          resp += "HOT WATER ONLY";
          break;
        case 2: //HW + heating
          SetRGB(255, 0, 255);
          digitalWrite(CH1pin, true);
          digitalWrite(CH2pin, true);
          resp += "HOT WATER AND HEATING";
          break;
        case 3: //heating only
          SetRGB(255, 0, 0);
          digitalWrite(CH1pin, false);
          digitalWrite(CH2pin, true);
          resp += "HEATING ONLY";
          break;
        default:
          state = 0;
          SetRGB();
          digitalWrite(CH1pin, false);
          digitalWrite(CH2pin, false);
          resp += "OFF";
          break;
      }
      oldState = state;
      SendMsg(resp);
   }
}


uint8_t whatStateIsThis() {
  bool ch1state = digitalRead(CH1pin);
  bool ch2state = digitalRead(CH2pin);

  if (!ch1state && !ch2state) {
    return 0;
  } else if (ch1state && !ch2state) {
    return 1;
  } else if (ch1state && ch2state) {
    return 2;
  } else if (!ch1state && ch2state) {
    return 3;
  }
}
