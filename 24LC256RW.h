#ifndef WIRE_HEADER
  #define WIRE_HEADER
  #include <Wire.h>
#endif 


/**
 * @brief 現在の値を読み取る
 *
 * I2Cデバイスから現在の値を取得し、指定された変数に格納する
 *
 * @return 読み取った現在値
 *
 * @note 実際のデータ形式やレジスタ構成に応じて、戻り値の型や説明を調整する
 */
byte readCurrent(byte device_addr){

  byte buf;

  Wire.beginTransmission(device_addr);
  Wire.endTransmission();

  // 返信
  Wire.requestFrom(device_addr, (uint8_t)1);

  if (Wire.available() >= 1) {
    buf = Wire.read();
  }

    // Serial.print("readcurrent : ");Serial.println((char)buf);

  return buf;
}

/**
 * @brief I2Cメモリから1バイトのデータを読む
 * 
 * 指定されたアドレスから1バイトのデータを読み込む
 * 
 * @param device_addr I2Cデバイスのスレーブアドレス
 * @param high_address 読み込みアドレスの高位バイト
 * @param low_address 読み込みアドレスの低位バイト
 * 
 * @return 読み込んだバイトデータ（byte）
 * 
 * @note Wire.available()が1以上でない場合、デフォルト値（0）が返される
 */
byte read1byte(byte device_addr, byte high_address, byte low_address){

  byte buf;

  Wire.beginTransmission(device_addr);

  Wire.write(high_address);
  Wire.write(low_address);
  
  Wire.endTransmission();

  // 返信
  Wire.requestFrom(device_addr, (uint8_t)1);

  if (Wire.available() >= 1) {
    buf = Wire.read();
    
  }
  // Serial.print("read1byte : ");Serial.println((char)buf);

  return buf;

}

/**
 * @brief I2Cメモリからxバイトのデータをreadする
 * 
 * 指定されたアドレスからx個のバイトデータを読み込む
 * 
 * @param device_addr I2Cデバイスのスレーブアドレス
 * @param high_address 読み込み開始アドレスの高位バイト
 * @param low_address 読み込み開始アドレスの低位バイト
 * @param x 読み込むバイト数
 * @param outBuf 読み込んだデータを格納する出力バッファ（ポインタ）
 * 
 * @return なし（void）
 * 
 * @note Wire.available()が1以上の場合のみデータを読み込む
 */
void readxbytes(byte device_addr, byte high_address, byte low_address, uint8_t x, byte* outBuf){

  byte temp[x];

  Wire.beginTransmission(device_addr);

  Wire.write(high_address);
  Wire.write(low_address);
  
  Wire.endTransmission();

  // 返信
  Wire.requestFrom(device_addr,x);

  if (Wire.available() >= 1) {
    for(int i = 0; i < x ; i++){
      temp[i] = Wire.read();
      // Serial.print((char)outBuf[i]);
    }

  }

  outBuf = temp;
}

/**
 * @brief I2Cメモリへ1バイトを書き込む
 *
 * 指定されたメモリアドレスに1バイトのデータを書き込む
 *
 * @param device_addr I2Cデバイスのスレーブアドレス
 * @param high_address 書き込み先アドレスの高位バイト
 * @param low_address 書き込み先アドレスの低位バイト
 * @param word 書き込む1バイトデータ
 *
 * @return Wire.endTransmission() の戻り値
 *         0: 成功, 1: データ長不足, 2: アドレス送信失敗, 3: ACK受信失敗, 4: その他のエラー
 *
 * @note この関数は1バイトの書き込みを実行し、その結果を返す
 */
int write1byte(byte device_addr, byte high_address, byte low_address, byte word){

  //書き込み
  Wire.beginTransmission(device_addr);
  Wire.write(high_address);
  Wire.write(low_address);
  Wire.write(word);
  return Wire.endTransmission();

}

/**
 * @brief EEPROMの1ページにデータを書き込む
 *
 * 指定されたアドレスからページ単位でデータを書き込む
 *
 * @param device_addr I2Cデバイスのスレーブアドレス
 * @param high_address 書き込み開始アドレスの高位バイト
 * @param low_address 書き込み開始アドレスの低位バイト
 * @param buf 書き込むデータの配列
 * @param length 書き込むデータ長
 *
 * @return Wire.endTransmission() の戻り値
 *         0: 成功
 *         1: データ長不足
 *         2: アドレス送信失敗
 *         3: ACK受信失敗
 *         4: その他のエラー
 *
 * @note 1ページ境界を超えない範囲で書き込むこと
 */
int writePage(byte device_addr, byte high_address, byte low_address, byte words[], uint8_t length){

  if(length > 64){
    return -1;
  }

  //書き込み
  Wire.beginTransmission(device_addr);
  Wire.write(high_address);
  Wire.write(low_address);
  
  for(int i = 0; i < length; i++){
    Wire.write(words[i]);
    // Serial.print(words[i]);
  }

  
  return Wire.endTransmission();
}
