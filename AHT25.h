#include <Wire.h>
#include <CRC8.h>

/** AHT25 setting value */
struct {
  int Status = 0;
  unsigned long NextTime = 0;
  unsigned long NextMeasurementTime = 0;
  CRC8 crc;
  double temperature = 0.0;
  double humidity = 0.0;
  int discomfortIndex;
  bool busy;
  bool calibration;
  String errorMessage;
  unsigned long responseSpan;
  unsigned int errorCount;
  bool retryFlag = false;
  byte state;
} AHT25value;

const double AHT25_ERROR_VALUE = 999.0;
const byte AHT25_ADDR = 0x38;
const byte AHT25_Init = 0xE1;
const byte AHT25_SoftReset = 0xBA;
const byte AHT25_TriggerMeasurement0 = 0xAC;
const byte AHT25_TriggerMeasurement1 = 0x33;
const byte AHT25_TriggerMeasurement2 = 0x00;
const byte AHT25_GetStatus = 0x71;
const byte AHT25_Calibrate = 0x18;
const byte AHT25_CRC_POLYNOME = 0x31;
const byte AHT25_CRC_START_XOR = 0xFF;

const int AHT25_Pin_Initializing = 0;
const int AHT25_Sending_Getting_Status_Code = 10;
const int AHT25_Receiving_Status_Code = 20;
const int AHT25_Sending_Measurement_Status_Code = 30;
const int AHT25_Receiving_Measurement_Status_Code = 40;
const int AHT25_ErrorReceiving_Status_Code = 99;
const int AHT25_Error_Status_Code = 98;

const int AHT25_Measurement_Interval = 2000;


void AHT25(){
  byte buf[7];
  uint32_t humidity_raw;
  uint32_t temperature_raw;

  unsigned long currentTime = millis();

  if(AHT25value.Status == AHT25_Pin_Initializing){
    // Pin Initializing
    Serial.println("");
    Serial.print("millis: "); Serial.print(currentTime);Serial.println(" Status: Pin Initializing");
    Wire.begin(AHT25_PIN_I2C_SDA, AHT25_PIN_I2C_SCL);

    AHT25value.NextTime = currentTime + 100;
    AHT25value.Status = AHT25_Sending_Getting_Status_Code;
    AHT25value.errorCount = 0;

    AHT25value.crc.setPolynome(AHT25_CRC_POLYNOME);
    AHT25value.crc.setStartXOR(AHT25_CRC_START_XOR);

  }else if(AHT25value.Status == AHT25_Sending_Getting_Status_Code && currentTime >= AHT25value.NextTime){
    // Sending Getting Status code
    Serial.println("");
    Serial.print("millis: "); Serial.print(currentTime);Serial.println(" Sending Getting Status code");
    
    Wire.beginTransmission(AHT25_ADDR);
    Wire.write(AHT25_GetStatus);
    Wire.endTransmission();

    AHT25value.Status = AHT25_Receiving_Status_Code;
    AHT25value.NextTime = currentTime + 1;
    AHT25value.responseSpan = 0;

  }else if(AHT25value.Status == AHT25_Receiving_Status_Code && currentTime >= AHT25value.NextTime){
    // Receiving Status
    Serial.println("");
    Serial.print("millis: "); Serial.print(currentTime);Serial.println(" Receiving Status");
    AHT25value.responseSpan = AHT25value.responseSpan + 1;

    Wire.requestFrom(AHT25_ADDR, 1);

    if (Wire.available() >= 1) {
      // Read the status byte
      AHT25value.state = Wire.read();
      
      Serial.print("Receiving Status(HEX): 0x");Serial.print(AHT25value.state,HEX);Serial.println("");
      Serial.print("Receiving Status(Bin): B");for (int i = 7 ; i >= 0 ; i--)Serial.print(bitRead(AHT25value.state,i));Serial.println("");
      Serial.print("Response Span: "); Serial.print(AHT25value.responseSpan); Serial.println("ms");

      // Check the status
      // Status is normal
      if(AHT25value.state == 0x18){
        Serial.println("AHT25 status is normal.");
        AHT25value.Status = AHT25_Sending_Measurement_Status_Code;
        AHT25value.NextTime = currentTime + 1;
        AHT25value.errorCount = 0;
      
      //Status is not normal
      }else{
        AHT25value.errorCount = AHT25value.errorCount + 1; // Increment error count

        // If the error count exceeds the threshold, consider it as an abnormal status and take appropriate action
        if(AHT25value.errorCount >= 3){
          Serial.println("AHT25 status is abnormal and not recovered.");
          AHT25value.Status = AHT25_ErrorReceiving_Status_Code;
          AHT25value.errorCount = 0;
          AHT25value.errorMessage = "AHT25 status is abnormal and not recovered.";
        
        // AHT25 is busy
        }else if(bitRead(AHT25value.state,7) == 1){
          Serial.println("AHT25 is busy. Waiting...");
          AHT25value.NextTime = currentTime + 80;
          AHT25value.Status = AHT25_Sending_Getting_Status_Code;
        
        // AHT25 is not busy but status is not normal
        }else {
          Serial.println("AHT25 is in an abnormal state. Resetting...");
          Wire.beginTransmission(AHT25_ADDR);
          Wire.write(AHT25_SoftReset);
          Wire.endTransmission();
          
          AHT25value.NextTime = currentTime + 100;
          AHT25value.Status = AHT25_Sending_Getting_Status_Code;
        }
      }
    // No Response from AHT25
    }else{
      AHT25value.errorCount = AHT25value.errorCount + 1; 

      AHT25value.NextTime = currentTime + 1;

      Serial.print("Receiving Status is failed. Fail count:");Serial.println(AHT25value.errorCount);
      
      if(AHT25value.errorCount >= 3){
        if(!AHT25value.retryFlag){
          Serial.print("AHT25 is not responding properly.");
          Serial.print("Sending Getting Status Code again");
          AHT25value.retryFlag = true;
          AHT25value.errorCount = 0;
          AHT25value.Status = AHT25_Sending_Getting_Status_Code;
        }else{
          Serial.print("AHT25 is not responding properly.");
          Serial.print("Please check the connection and power supply of AHT25.");
          AHT25value.retryFlag = false;
          AHT25value.errorCount = 0;
          AHT25value.Status = AHT25_ErrorReceiving_Status_Code;
          AHT25value.errorMessage = "AHT25 is not responding properly. Please check the connection and power supply of AHT25.";
        }
      }
    }
  
    // Request measurement
  }else if(AHT25value.Status == AHT25_Sending_Measurement_Status_Code && currentTime >= AHT25value.NextTime){
    Serial.print("millis: "); Serial.println(currentTime);
    Serial.print("Status: "); Serial.println(AHT25value.Status);

    Wire.beginTransmission(AHT25_ADDR);
    Wire.write(AHT25_TriggerMeasurement0);
    Wire.write(AHT25_TriggerMeasurement1);
    Wire.write(AHT25_TriggerMeasurement2);
    Wire.endTransmission();
    AHT25value.Status = AHT25_Receiving_Measurement_Status_Code;
    AHT25value.NextTime = currentTime + 80;
    AHT25value.NextMeasurementTime = currentTime + AHT25_Measurement_Interval;

    // Receiving measurement data
  }else if(AHT25value.Status == AHT25_Receiving_Measurement_Status_Code && currentTime >= AHT25value.NextTime){
    Serial.print("millis: "); Serial.println(currentTime);
    Serial.print("Status: "); Serial.println(AHT25value.Status);

    Wire.requestFrom(AHT25_ADDR, 7);
    if (Wire.available() >= 7) {
      for(int i=0; i<7; i++) {
        buf[i] = Wire.read();
      }

      if(bitRead(buf[0],7) == 1){
          Serial.println("AHT25 is busy. Waiting...");
      }else{
        AHT25value.crc.restart();
        AHT25value.crc.add(buf, 6);

        if (buf[6] == AHT25value.crc.getCRC()) {
          humidity_raw = ((uint32_t)buf[1] << 12)|((uint32_t)buf[2] << 4)|(((uint32_t)buf[3] >> 4) & 0x0F);
          temperature_raw = (((uint32_t)buf[3] & 0x0F) << 16)|((uint32_t)buf[4] << 8)|((uint32_t)buf[5]);
          AHT25value.humidity = humidity_raw / 1048576.0 * 100;
          AHT25value.temperature = temperature_raw / 1048576.0 * 200 - 50;

          /** discomfort index */
          AHT25value.discomfortIndex = AHT25value.temperature * 0.81 + 0.01 * AHT25value.humidity * (0.99 * AHT25value.temperature - 14.3) + 46.3;

          Serial.print("Aht25(");
          Serial.print("temperature: ");
          Serial.print(AHT25value.temperature);
          Serial.print(" humidity: ");
          Serial.print(AHT25value.humidity);
          Serial.print(" disconfort index: ");
          Serial.print(AHT25value.discomfortIndex);
          Serial.print(")");
          Serial.println("");

        }else{
          Serial.println("CRC check failed. Data may be corrupted.");
          AHT25value.errorMessage = "CRC check failed. Data may be corrupted.";
        }
        AHT25value.errorCount = 0;
        AHT25value.NextTime = AHT25value.NextMeasurementTime;
        AHT25value.Status = AHT25_Sending_Measurement_Status_Code;
      }

    }else{
      Serial.print("No Response from AHT25. Fail count:");Serial.println(AHT25value.errorCount);
      AHT25value.errorCount = AHT25value.errorCount + 1;
      AHT25value.NextTime = currentTime + 80;
      
      if(AHT25value.errorCount >= 3){
        Serial.println("AHT25 is not responding properly. Jump Back to Checking Status.");
        AHT25value.errorMessage = "AHT25 is not responding properly. Jump Back to Checking Status.";
        AHT25value.errorCount = 0;
        AHT25value.Status = AHT25_Sending_Getting_Status_Code;
      }
    }
    
  }
}