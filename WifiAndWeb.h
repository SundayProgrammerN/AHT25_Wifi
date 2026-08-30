#include <WiFi.h>
#include <WebServer.h>
#include <esp_mac.h>
// #ifndef EEPROM24LC256RW_HEADER
//   #define EEPROM24LC256RW_HEADER
//   #include "24LC256RW.h"
// #endif 
#ifndef EEPROM
  #define EEPROM
  #include "EEPROM.h"
#endif 

/** Wifi setting value */
struct{
  unsigned long NextTime = 0;
  int Status = 0;
  int TryCount = 0;
  String WIFI_SSID;
  String WIFI_PASSWORD;
} WifiValue;

/** Web setting value */
String json;
String HTML;
WebServer server(PORT);

void WifiAndWeb(){
  unsigned long currentTime = millis();

  uint8_t baseMac[6];
  char baseMacChr[18] = {0};

  byte ssidBuf[33] = {'\0'};
  byte pwBuf[65] = {'\0'};

  switch(WifiValue.Status){
    case 0:// Read SSID
      Serial.println("-------------------");
      Serial.println("Read Wifi setting from EEPROM");
      Serial.println("-------------------");

      WifiValue.WIFI_SSID = receiveSSID();

      WifiValue.WIFI_PASSWORD = receivePassword();

      Serial.println("-------------------");
      Serial.print("Wifi SSID     : ");Serial.println(WifiValue.WIFI_SSID);
      Serial.print("Wifi Password : ");Serial.println(WifiValue.WIFI_PASSWORD);

      // Get MAC address for WiFi station
      esp_read_mac(baseMac, ESP_MAC_WIFI_STA);
      sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);

      Serial.print("MAC address   : ");Serial.println(baseMacChr);
      Serial.println("-------------------");

      WifiValue.TryCount = 0;
      WiFi.begin(WifiValue.WIFI_SSID, WifiValue.WIFI_PASSWORD);
      WifiValue.Status = 20;

    case 20:// Trying connect
      if(currentTime < WifiValue.NextTime) break;
      if(WifiValue.TryCount == 0){
        Serial.println("Trying wifi connect...");
      }

      if(WiFi.status() == WL_CONNECTED){
        Serial.println("-------------------");
        Serial.println("Wifi connection established!");
        Serial.print("IP Address : ");Serial.println(WiFi.localIP());
        Serial.println("-------------------");

        server.begin();

        WifiValue.Status = 30;
      }else{
        WifiValue.NextTime = currentTime + 1000;

        if(++(WifiValue.TryCount) >= 10){//Reset Wifi setting
          Serial.println("Reset Wifi setting");
          WifiValue.Status = 0;
        }
        break;
      }

    default:// Established connection
      if(WiFi.status() != WL_CONNECTED){//Wifi connection lost
        Serial.println("Wifi connection lost");
        WifiValue.TryCount = 0;
        WifiValue.Status = 0;
        break;
      }

      server.handleClient();

  }
  return;
}

void serveWeb(){
  server.on("/", HTTP_GET, [](){

    //HTML記述
    HTML = "";
    HTML += "<!DOCTYPE html>";
    HTML += "<html lang=\"ja\">";
    HTML += "<head>";
    HTML += "    <meta charset=\"UTF-8\">";
    HTML += "    <meta http-equiv=\"refresh\" content=\"60;url=/\">";
    HTML += "    <title>ESP32 AHT25 Wifi Time</title>";
    HTML += "</head>";
    HTML += "<body>";
    HTML += "    <h1>ESP32 AHT25 Wifi Time</h1>";
    HTML += "    <p>気温:"; HTML += AHT25value.temperature; HTML += "℃</p>";
    HTML += "    <p>湿度:"; HTML += AHT25value.humidity; HTML += "%</p>";
    HTML += "    <p>不快指数:"; HTML += AHT25value.discomfortIndex; HTML += "</p>";
    HTML += "    <p>エラーメッセージ:"; HTML += AHT25value.errorMessage; HTML += "</p>";
    HTML += "</body>";
    HTML += "</html>";
    
    Serial.println("-------------------");
    Serial.println("Requested HTTP.");
    Serial.print("HTML data = ");Serial.println(HTML);
    Serial.println("-------------------");
    server.send(200, "text/HTML", HTML); // 値をクライアントに返す
  });

  server.on("/json", HTTP_GET, [](){

    //JSON記述
    json = "";
    json += "{";
    json += "\"ThermohygrometerData\" : {";
    json += "\"humidity\" : ";
    json += AHT25value.humidity;
    json += ",";
    json += "\"temperature\" : ";
    json += AHT25value.temperature;
    json += ",";
    json += "\"disconfortIndex\" : ";
    json += AHT25value.discomfortIndex;
    json += "}";
    json += "}";
    
    Serial.println("-------------------");
    Serial.println("Requested json data");
    Serial.print("json data = ");Serial.println(json);
    Serial.println("-------------------");
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json; charset=utf-8", json); // 値をクライアントに返す

  });
}