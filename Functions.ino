void SetRGB(uint8_t R, uint8_t G, uint8_t B) {
  analogWrite(RED, R);
  analogWrite(GREEN, G);
  analogWrite(BLUE, B);
}



void PrintLEDStatus() {
  String resp;
  resp += "LED: ";
  resp += (digitalRead(LED) == LOW) ? "ON" : "OFF"; //yeah, led is active-low.
  SendMsg(resp);
}



void PrintCH1Status() {
  String resp;
  resp += "CH1: ";
  resp += (digitalRead(CH1pin) == HIGH) ? "ON" : "OFF"; //ch1 and 2 are for powering a pair of SSRs, so they're active-high.
  SendMsg(resp);
}



void PrintCH2Status() {
  String resp;
  resp += "CH2: ";
  resp += (digitalRead(CH2pin) == HIGH) ? "ON" : "OFF";
  SendMsg(resp);
}



void PrintStatus() {
  PrintCH1Status();
  PrintCH2Status();
  //PrintLEDStatus();
}
