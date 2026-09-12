#ifndef WIRE_HEADER
  #define WIRE_HEADER
  #include <Wire.h>
#endif 

#ifndef WIFI_AND_WEB_HEADER
  #define WIFI_AND_WEB_HEADER
  #include "WifiAndWeb.h"
#endif 

// TODO : 暫定
#ifndef AHT25_HEADER 
  #define AHT25_HEADER
  #include "AHT25.h"
#endif 

/** Time setting */
const unsigned long TIME_LIMIT = 2592000000; // 30days

/** AHT25 setting value */
const int AHT25_PIN_I2C_SDA = 14;
const int AHT25_PIN_I2C_SCL = 13;

/** Serial setting value */
const int SERIAL_BAUDRATE = 115200; // Baudrate for serial communication.

void setI2CPins() {
  Wire.begin(AHT25_PIN_I2C_SDA, AHT25_PIN_I2C_SCL);
}

void initialSerial(){
  /** Serial initializing */
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println("-------------------");
  Serial.println("AHT25_Wifi");
  Serial.println("Created by H.N");
  Serial.println("-------------------");
  Serial.print("TIME_LIMIT   : ");Serial.println(TIME_LIMIT);
}

void autoReset(){
  unsigned long currentTime = millis();
  
  /** Auto Reset */
  if(currentTime >= TIME_LIMIT){
    Serial.println("-------------------");
    Serial.print("currentTime : ");Serial.println(currentTime);
    Serial.print("TIME_LIMIT  : ");Serial.println(TIME_LIMIT);
    Serial.println("System reboot!");
    Serial.println("-------------------");
    ESP.restart();
  }
}