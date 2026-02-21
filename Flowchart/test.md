mermaid
```
graph TD
    A[開始] --> B{現在の時刻を取得<br>currentTime = millis()}
    B --> C{Status == 0?}
    C -->|はい| D[Wire.begin<br>(SDA, SCL)]
    D --> E[NextTime = currentTime + 100]
    E --> F[Status = 1]
    F --> Z[終了]

    C -->|いいえ| G{Status == 1 かつ<br>currentTime >= NextTime?}
    G -->|はい| H[Wire.beginTransmission<br>(AHT25_ADDR)]
    H --> I[Wire.write(0x71)]
    I --> J[Wire.endTransmission()]
    J --> K[NextTime = currentTime + 10]
    K --> L[Status = 2]
    L --> Z

    G -->|いいえ| M{Status == 2 かつ<br>currentTime >= NextTime?}
    M -->|はい| N[crc.setPolynome(0x31)<br>crc.setStartXOR(0xFF)]
    N --> O[Status = 3]
    O --> Z

    M -->|いいえ| P{Status == 3 かつ<br>currentTime >= NextTime?}
    P -->|はい| Q[Wire.beginTransmission<br>(AHT25_ADDR)]
    Q --> R[Wire.write(0xAC)<br>Wire.write(0x33)<br>Wire.write(0x00)]
    R --> S[Wire.endTransmission()]
    S --> T[NextTime = currentTime + 80]
    T --> U[Status = 4]
    U --> Z

    P -->|いいえ| V{Status == 4 かつ<br>currentTime >= NextTime?}
    V -->|はい| W[Wire.requestFrom<br>(AHT25_ADDR, 7)]
    W --> X{7バイト受信可能?}
    X -->|はい| Y[buf[0..6]にデータを読み込む]
    Y --> AA{buf[0] & 0x80 != 0?}
    AA -->|はい| AB[NextTime = currentTime + 80]
    AB --> AC[Status = 4]
    AC --> Z

    AA -->|いいえ| AD[crc.restart()<br>crc.add(buf, 6)]
    AD --> AE{buf[6] == crc.getCRC()?}
    AE -->|はい| AF[humidity_raw, temperature_rawを計算]
    AF --> AG[humidity = humidity_raw / 1048576.0 * 100<br>temperature = temperature_raw / 1048576.0 * 200 - 50]
    AG --> AH[discomfortIndex = 0.81 * temperature + 0.01 * humidity * (0.99 * temperature - 14.3) + 46.3]
    AH --> AI[シリアル出力<br>(temperature, humidity, discomfortIndex)]
    AI --> AJ[NextTime = currentTime + 1000 - 80]
    AJ --> AK[Status = 3]
    AK --> Z

    AE -->|いいえ| AL[humidity = AHT25_ERROR_VALUE<br>temperature = AHT25_ERROR_VALUE]
    AL --> AI

    X -->|いいえ| Z
    V -->|いいえ| Z
    ```