#include <Adafruit_NeoPixel.h>
#include <SoftwareSerial.h>

#define PIN1 11
#define PIN2 12
#define NUMPIXELS 2

Adafruit_NeoPixel strip1 = Adafruit_NeoPixel(NUMPIXELS, PIN1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel strip2 = Adafruit_NeoPixel(NUMPIXELS, PIN2, NEO_GRB + NEO_KHZ800);
SoftwareSerial HC06(2, 3); // RX, TX

const int btn = 13;               // 버튼 pin
const int check_interval = 10;    // 버튼 체크할 시간 10ms

int LED_switch = 0;               // LED 켜는 신호
int btn_sec = 3;                  // 몇 초 누르면 신고될지 (sec)
int push_count = 0;               // 버튼 몇초 누르고 있는지 체크

void NeopixelOn() {
  strip1.setPixelColor(0, 255, 0, 0);
  strip1.setPixelColor(1, 255, 0, 0);
  strip2.setPixelColor(0, 255, 0, 0);
  strip2.setPixelColor(1, 255, 0, 0);
  strip1.show();
  strip2.show();
  delay(2000);
  strip1.setPixelColor(0, 0, 0, 0 );
  strip1.setPixelColor(1, 0, 0, 0 );
  strip2.setPixelColor(0, 0, 0, 0 );
  strip2.setPixelColor(1, 0, 0, 0 );
  strip1.show();
  strip2.show();
  delay(2000);
  strip1.setPixelColor(0, 255, 0, 0);
  strip1.setPixelColor(1, 255, 0, 0);
  strip2.setPixelColor(0, 255, 0, 0);
  strip2.setPixelColor(1, 255, 0, 0);
  strip1.show();
  strip2.show();
  delay(2000);
  strip1.setPixelColor(0, 0, 0, 0 );
  strip1.setPixelColor(1, 0, 0, 0 );
  strip2.setPixelColor(0, 0, 0, 0 );
  strip2.setPixelColor(1, 0, 0, 0 );
  strip1.show();
  strip2.show();
  delay(2000);
  strip1.setPixelColor(0, 255, 0, 0);
  strip1.setPixelColor(1, 255, 0, 0);
  strip2.setPixelColor(0, 255, 0, 0);
  strip2.setPixelColor(1, 255, 0, 0);
  strip1.show();
  strip2.show();
  delay(2000);
  strip1.setPixelColor(0, 0, 0, 0 );
  strip1.setPixelColor(1, 0, 0, 0 );
  strip2.setPixelColor(0, 0, 0, 0 );
  strip2.setPixelColor(1, 0, 0, 0 );
  strip1.show();
  strip2.show();
  delay(2000);
  strip1.setPixelColor(0, 255, 0, 0);
  strip1.setPixelColor(1, 255, 0, 0);
  strip2.setPixelColor(0, 255, 0, 0);
  strip2.setPixelColor(1, 255, 0, 0);
  strip1.show();
  strip2.show();
  delay(2000);
  strip1.setPixelColor(0, 0, 0, 0 );
  strip1.setPixelColor(1, 0, 0, 0 );
  strip2.setPixelColor(0, 0, 0, 0 );
  strip2.setPixelColor(1, 0, 0, 0 );
  strip1.show();
  strip2.show();
  delay(2000);
  strip1.setPixelColor(0, 255, 0, 0);
  strip1.setPixelColor(1, 255, 0, 0);
  strip2.setPixelColor(0, 255, 0, 0);
  strip2.setPixelColor(1, 255, 0, 0);
  strip1.show();
  strip2.show();
  delay(2000);
  strip1.setPixelColor(0, 0, 0, 0 );
  strip1.setPixelColor(1, 0, 0, 0 );
  strip2.setPixelColor(0, 0, 0, 0 );
  strip2.setPixelColor(1, 0, 0, 0 );
  strip1.show();
  strip2.show();
  delay(2000);

  strip1.setPixelColor(0, 255, 0, 0 );
  strip1.setPixelColor(1, 255, 0, 0 );
  strip2.setPixelColor(0, 255, 0, 0 );
  strip2.setPixelColor(1, 255, 0, 0 );
  strip1.show();
  strip2.show();
  delay(2000);  
}

void NeopixelOff() {
  strip1.setPixelColor(0, 0, 0, 0 );
  strip1.setPixelColor(1, 0, 0, 0 );
  strip2.setPixelColor(0, 0, 0, 0 );
  strip2.setPixelColor(1, 0, 0, 0 );
  strip1.show();
  strip2.show();
}

void setup() {
  Serial.begin(9600); // PC - Arduino
  HC06.begin(9600); // Arduino - hc05
  strip1.begin();
  strip2.begin();
  pinMode(btn, INPUT);
}

void loop() { // run over and over
  push_count = 0;
  if(LED_switch == 0)
  {
    while(digitalRead(btn) == HIGH) // 버튼 누르는 시간 체크 후 LED_switch = 1
    {
      push_count += 1;
//      Serial.print("push count ");
//      Serial.println(push_count);
      if(push_count >= btn_sec) 
      {
        LED_switch = 1;
        break;  
      }
      delay(1000);
  
    }
  }
  
  
  HC06.write(LED_switch); // LED_switch 블루투스 전송
//  Serial.print("LED state ");
//  Serial.println(LED_switch);
  
  HC06.listen();
  if(HC06.available()) {
      int bt;
      bt = HC06.read();
      
      if(bt == 255) { // 마스터에서 보낸 LED on 신호 수신했을 때 
        
        Serial.println(bt);
        NeopixelOn();
        LED_switch = 0;
      }
      else
      {
        NeopixelOff();
      }
    }

    else {
        NeopixelOff();
    }
  delay(check_interval);
}
