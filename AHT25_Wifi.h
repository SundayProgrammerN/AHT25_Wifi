#ifndef USER_SETTING_HEADER 
  #define USER_SETTING_HEADER
  #include "UserSetting.h"
#endif 

#ifndef AHT25_HEADER 
  #define AHT25_HEADER
  #include "AHT25.h"
#endif 

#ifndef WIFI_AND_WEB_HEADER
  #define WIFI_AND_WEB_HEADER
  #include "WifiAndWeb.h"
#endif 

#ifndef EEPROM24LC256RW_HEADER
  #define EEPROM24LC256RW_HEADER
  #include "24LC256RW.h"
#endif 

#ifndef WIRE_HEADER
  #define WIRE_HEADER
  #define PAGE_SIZE 64 // TODO:24LC256RWのページサイズ。のちのち削除
  #include <Wire.h>
#endif 

#ifndef EEPROM
  #define EEPROM
  #include "EEPROM.h"
#endif 

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