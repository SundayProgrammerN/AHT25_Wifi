#ifndef EEPROM24LC256RW_HEADER
  #define EEPROM24LC256RW_HEADER
  #include "24LC256RW.h"
#endif 

const byte EEPROM24LC256_ADDR = 0b1010000; // I2C address of 24LC256RW

/** @brief SSIDを受信する
 *
 * @return 受信したSSIDの文字列
 */
String receiveSSID(){
  // 受信バッファの初期化
  String str = "";

  // 受信バッファの確保
  byte outBuf[33] = {'\0'};

  // 受信処理の実行
  readxbytes(EEPROM24LC256_ADDR, 0x00, 0x00, 32, outBuf);

  // 受信データを文字列に変換
  for(int i=0; i< 32; i++){

    // 文字列の終端を検出した場合、ループを終了
    if(outBuf[i] == 0xFF){

      // 文字列の終端を設定
      outBuf[i] = '\0';

      // ループを終了
      break;
    } 

    // 文字列に追加
    str += (char)outBuf[i];
  }

  // 受信データを返す
  return str;
}

/** @brief Wifiパスワードを受信する
 *
 * @return 受信したWifiパスワードの文字列
 */
String receivePassword(){
  // 受信バッファの初期化
  String str = "";

  // 受信バッファの確保
  byte outBuf[65] = {'\0'};

  // 受信処理の実行
  readxbytes(EEPROM24LC256_ADDR, 0x00, 0x30, 64, outBuf);

  // 受信データを文字列に変換
  for(int i=0; i<= 64; i++){

    // 文字列の終端を検出した場合、ループを終了
    if(outBuf[i] == 0xFF){

      // 文字列の終端を設定
      outBuf[i] = '\0';

      // ループを終了
      break;
    }
    
    // 文字列に追加
    str += (char)outBuf[i];
  
  }

  // 受信データを返す
  return str;
}