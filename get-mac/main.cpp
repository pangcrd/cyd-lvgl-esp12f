
//#define BOARD_ESP8266
#define BOARD_ESP32

#if defined(BOARD_ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(BOARD_ESP32)
  #include <WiFi.h>
#endif

void setup() {
  Serial.begin(115200);
  Serial.println();

#if defined(BOARD_ESP8266) || defined(BOARD_ESP32)
  Serial.print("ESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
#else
  Serial.println("No board selected!");
#endif
}

void loop() {

}
