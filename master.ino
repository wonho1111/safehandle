#include <SoftwareSerial.h>

SoftwareSerial HC05_1(10, 11); // RX, TX
SoftwareSerial HC05_2(12, 13); // RX, TX

int LED = 7;                     // LED
int bt_1 = 0;
int bt_2 = 0;

void setup() {
  
  Serial.begin(9600); // PC - Arduino
  HC05_1.begin(38400); // Arduino - 
  HC05_2.begin(38400);
  
  pinMode(LED, OUTPUT);
}

void loop() { // run over and over
  HC05_1.listen();
  if(HC05_1.available()) {
    
      bt_1 = HC05_1.read();
      if(bt_1 == 120) {
        digitalWrite(LED, HIGH);
        HC05_1.write(bt_1);
        HC05_2.write(bt_1); 
        delay(20000);

      }
      else if(bt_1 == 0) {
        digitalWrite(LED, LOW);
      }
      Serial.println(bt_1);
    }
    
  HC05_2.listen();
  if(HC05_2.available()) {
    
      bt_2 = HC05_2.read();
      if(bt_2 == 120) {
        digitalWrite(LED, HIGH);
        HC05_2.write(bt_2);
        HC05_1.write(bt_2);
        delay(20000);
      }
      else if(bt_2 == 0) {
        digitalWrite(LED, LOW);
      }
      Serial.println(bt_2);
    }
    
  HC05_1.flush();
  HC05_2.flush();
}
