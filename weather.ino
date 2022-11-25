#include <Adafruit_NeoPixel.h>
#define PIN 12
#define NUMPIXELS 13

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS,PIN, NEO_RGB + NEO_KHZ800);

#include "WiFiEsp.h"

#include <SoftwareSerial.h> 
SoftwareSerial esp01(2, 3); // RX, TX

char ssid[] = "cafe_dongnae_daeya_2.4g";    // your network SSID (name)
char pass[] = "daeya5097800!";         // your network password
int status = WL_IDLE_STATUS;        // the Wifi radio's status

// https://www.kma.go.kr/wid/queryDFSRSS.jsp?zone=4139059700
const char* host = "www.kma.go.kr";
const String url = "/wid/queryDFSRSS.jsp?zone=4139059700";

WiFiEspClient client; // WiFiEspClient 객체 선언

// RSS 날씨 정보 저장 변수
String line0 = "";
String line1 = "";
String line2 = "";

uint8_t count = 0; // RSS 날씨 정보 분류 카운터

// RSS 날씨 정보 수신에 소요되는 시간 확인용 변수
unsigned long int count_time = 0;
bool count_start = false;
int count_val = 0;

int resetcnt = 0;

void resetFunc()
{
  asm volatile ("jmp 0");
}

void get_weather() {
  Serial.println(F("Starting connection to server..."));
  if (client.connect(host, 80)) {
    Serial.println(F("Connected to server"));
    client.print("GET " + url + " HTTP/1.1\r\n" +  
                 "Host: " + host + "\r\n" +  
                 "Connection: close\r\n\r\n");
    count_start = true;
  }
}

void setup() {
  pixels.begin();
  Serial.begin(9600);  //시리얼모니터
  esp01.begin(9600);   //와이파이 시리얼
  WiFi.init(&esp01);   // initialize ESP module
  while ( status != WL_CONNECTED) {   // attempt to connect to WiFi network
    Serial.print(F("Attempting to connect to WPA SSID: "));
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);    // Connect to WPA/WPA2 network
  }
  Serial.println(F("You're connected to the network"));
  Serial.println();
  delay(1000);
  
}

void check_client_available() {  // loop()함수 진입 및 스캔 테스트 코드
  if (count_start == true) {
    if (millis() - count_time >= 100) {
      count_time = millis();
      count_val++;
      Serial.print(".");
      if (count_val == 30) count_start = false;
    }
  }
}

// 온도 저장 변수
float pty0;

void loop() {
  get_weather();
  resetcnt = resetcnt + 1;
  
  check_client_available();
  while(client.available()) { // 날씨 정보가 들어오는 동안f
    char c = client.read();
    if (c == '<') count++;
    if (count > 28 && count <= 78) line0 += c;
    else if (count > 78 && count <= 118) line1 += c;
    else if (count > 118) break; // 중간에 빠져나가고 
  }
  if (count != 0) { // 수신 값 있으면 
    while(client.available()) {  // 나머지 날씨정보 버리기
      char c = client.read(); 
    }
    if (!client.connected()) {   // 날씨정보 수신 종료됐으면
      Serial.println();
      Serial.println(F("Disconnecting from server..."));
      client.stop();
    }
    parsing();

    if(pty0 > 0) {
        pixels.setPixelColor(0, 255, 232,244 );
        pixels.setPixelColor(1, 255, 232,244 );
        pixels.setPixelColor(2, 255, 232,244 );
        pixels.setPixelColor(3, 255, 232,244 );
        pixels.setPixelColor(4, 255, 232,244 );
        pixels.setPixelColor(5, 255, 232,244 );
        pixels.setPixelColor(6, 255, 232,244 );
        pixels.setPixelColor(7, 255, 232,244 );
        pixels.setPixelColor(8, 255, 232,244 );
        pixels.setPixelColor(9, 255, 232,244 );
        pixels.setPixelColor(10, 255, 232,244 );
        pixels.setPixelColor(11, 255, 232,244 );
        pixels.setPixelColor(12, 255, 232,244 );
      }
    else {
        pixels.setPixelColor(0, 0, 0,255 );
        pixels.setPixelColor(1, 0, 0,255 );
        pixels.setPixelColor(2, 0, 0,255 );
        pixels.setPixelColor(3, 0, 0,255 );
        pixels.setPixelColor(4, 0, 0,255 );
        pixels.setPixelColor(5, 0, 0,255 );
        pixels.setPixelColor(6, 0, 0,255 );
        pixels.setPixelColor(7, 0, 0,255 );
        pixels.setPixelColor(8, 0, 0,255 );
        pixels.setPixelColor(9, 0, 0,255 );
        pixels.setPixelColor(10, 0, 0,255 );
        pixels.setPixelColor(11, 0, 0,255 );
        pixels.setPixelColor(12, 0, 0,255 );
      }
    pixels.show();
    count = 0;
  }

  delay(2000);
  if(resetcnt == 10)  resetFunc();


}

void parsing() {
  String announce_time;
  int tm_start= line0.indexOf(F("<tm>")); // "<tm>"문자가 시작되는 인덱스 값('<'의 인덱스)을 반환한다. 
  int tm_end= line0.indexOf(F("</tm>"));  
  announce_time = line0.substring(tm_start + 4, tm_end); // +4: "<tm>"스트링의 크기 4바이트, 4칸 이동
  Serial.print(F("announce_time: ")); Serial.println(announce_time);
  
  String pty;
  int pty_start= line0.indexOf(F("<pty>"));
  int pty_end= line0.indexOf(F("</pty>"));
  pty = line0.substring(pty_start + 5, pty_end);
  pty0 = pty.toFloat();   // 자료형 변경 String -> float
  Serial.print(F("pty0: ")); Serial.println(pty0);

  line0 = "";
  line1 = ""; // 스트링 변수 line1 데이터 추출 완료 
}
