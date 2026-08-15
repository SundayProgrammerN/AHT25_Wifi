#include "AHT25_Wifi.h"

void setup() {

  /** Serial initializing */
  initialSerial();

  Wire.begin(AHT25_PIN_I2C_SDA, AHT25_PIN_I2C_SCL);

  /** Wifi and Web Server initializing */
  serveWeb();


}

void loop() {

  /** Auto Reset */
  autoReset();

  /** Get tempereture and humidy values from AHT25 */
  AHT25();

  /** Wifi and Web Server */
  WifiAndWeb();

}