#include <Arduino.h>
#include <RadioLib.h>

#define PIN_CS1    10
#define PIN_GDO0   14
#define PIN_GDO2   15

CC1101 radio = new Module(PIN_CS1, PIN_GDO0, RADIOLIB_NC, PIN_GDO2);

/*
byte data1[] = {
  0x88, 0x88, 0xe8, 0xee, 0xe8, 0x88, 0x88, 0x88, 0xe8, 0xe8, 0x8e, 0x88, 0x80
};
*/

// http://sc-tech.cn/en/ev1527.pdf
//
//            ID                 |  DATA
// 11 01 11 10 10 10 11 01 00 00 | 01 00
byte payload[] = {
  0xee, 0x8e, 0xee, 0xe8, 0xe8, 0xe8, 0xee, 0x8e, 0x88, 0x88, 0x8e, 0x88, 0x80
};

void setup() {
  delay(1000);

  Serial.begin(115200);
  Serial.print("Initializing ... ");

  int state = radio.begin(433.953, 2.93);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("OK");
  } else {
    Serial.print("ERROR");
    Serial.println(state);
    for (;;);
  }

  radio.setOOK(true);
  radio.setEncoding(RADIOLIB_ENCODING_NRZ);

  radio.setCrcFiltering(false);
  //radio.setPreambleLength(16);

  radio.disableSyncWordFiltering();
  radio.fixedPacketLengthMode(sizeof(payload));
}


void loop() {
  Serial.print("Transmitting ...");

  int state = radio.transmit(payload, sizeof(payload), 0);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("OK");
  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    Serial.println("TOO LONG");
  } else {
    Serial.print("ERROR");
    Serial.println(state);
  }

  delay(7);
}

