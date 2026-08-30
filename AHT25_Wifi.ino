#include "AHT25_Wifi.h"

void setup() {

  /** Serial initializing */
  initialSerial();

  /** I2C pins initializing */
  setI2CPins();

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