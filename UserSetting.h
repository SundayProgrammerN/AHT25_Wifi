/** Time setting */
const unsigned long TIME_LIMIT = 2592000000; // 30days

/** AHT25 setting value */
const int AHT25_PIN_I2C_SDA = 14;
const int AHT25_PIN_I2C_SCL = 13;

/** Wifi setting value */
//const char *WIFI_SSID = "hogehoge";  // ssid of your wifi router
//const char *WIFI_PASSWORD = "xxxxx"; // password of your wifi router
const int PORT = 80;                 // Port number as a webserver
const byte EEPROM24LC256_ADDR = 0b1010000; // I2C address of 24LC256RW

/** Serial setting value */
const int SERIAL_BAUDRATE = 115200; // Baudrate for serial communication.