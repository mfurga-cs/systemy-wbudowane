/*
 * Datasheet: http://ww1.microchip.com/downloads/en/devicedoc/40138c.pdf
 */
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

byte buff[(66 * 3 + 1) / 8 + 1] = {};

//byte payload[] = {0xaa, 0xab};
byte sync[] = { 0xaa, 0x00 };

void setup() {
  delay(1000);

  Serial.begin(115200);
  Serial.print("Initializing ... ");

  //int state = radio.begin(433.798 + 0.050, 2.639);

  int state = radio.begin(433.888 + 0.050, 2.694);


  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("OK");
  } else {
    Serial.print("ERROR");
    Serial.println(state);
    for (;;);
  }

  radio.setOOK(true);
  radio.setEncoding(RADIOLIB_ENCODING_NRZ);

  state = radio.setSyncWord(sync, 2);
  Serial.println(state);

  radio.setPreambleLength(16);

  //radio.disableSyncWordFiltering();
  radio.setCrcFiltering(false);

  //radio.disableSyncWordFiltering();
  radio.fixedPacketLengthMode(sizeof(buff));

  // start listening for packets
  Serial.print(F("[CC1101] Starting to listen ... "));
  state = radio.startReceive();
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true);
  }
}

void print_array(byte *arr, int size) {
  for (int i = 0; i < size; i++) {
    Serial.print(arr[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
}

bool recv = true;

void loop() {
  //Serial.print(F("[CC1101] Waiting for incoming transmission ... "));

  // you can receive data as an Arduino String
  //String str;
  //int state = radio.receive(str);

  if (recv) {

    int state = radio.receive(buff, sizeof(buff));

    if (state == RADIOLIB_ERR_NONE) {
      print_array(buff, sizeof(buff));
      recv = false;
    } else if (state == RADIOLIB_ERR_RX_TIMEOUT) {
      Serial.println(F("timeout!"));
    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      // packet was received, but is malformed
      Serial.println(F("CRC error!"));
    } else {
      Serial.print(F("failed, code "));
      Serial.println(state);
    }
  } else {
    delay(30000);

    int state = radio.transmit(buff, sizeof(buff), 0);

    if (state == RADIOLIB_ERR_NONE) {
      Serial.println("OK");
    } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
      Serial.println("TOO LONG");
    } else {
      Serial.print("ERROR");
      Serial.println(state);
    }
  }
}

