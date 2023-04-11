#include <Arduino.h>
#include <RadioLib.h>

#define LED        13

#define PIN_CS1    10
#define PIN_GDO0   14
#define PIN_GDO2   15

#define CC1101_GDO0 PIN_GDO0

void SPI_pins() {
  Serial.printf("MISO: %d\r\n", MISO);
  Serial.printf("MOSI: %d\r\n", MOSI);
  Serial.printf("SS  : %d\r\n", SS);
}

CC1101 radio = new Module(PIN_CS1, PIN_GDO0, RADIOLIB_NC, PIN_GDO2);

void setFlag(void);

void setup() {
  Serial.begin(115200);

  Serial.print("Initializing ... ");

  int state = radio.begin(
    433.809,
    1.0,
    1.587
    );
  //int state = radio.begin(433.791);

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("OK");
  } else {
    Serial.print("ERROR");
    Serial.println(state);

    for (;;);
  }


  // set the function that will be called
  // when new packet is received
  //radio.setGdo0Action(setFlag);

  radio.setOOK(true);
  radio.setEncoding(RADIOLIB_ENCODING_NRZ);

  radio.setCrcFiltering(false);
  radio.setPreambleLength(16);

  uint8_t sync[] = { 0xab, 0xaa };
  radio.setSyncWord(sync, sizeof(sync), 0);

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

const char *data = "hello casdo oansod inasoin doasin doanis doinas ";

volatile bool receivedFlag = false;

void setFlag(void) {
  // we got a packet, set the flag
  receivedFlag = true;
}

void loop() {
  Serial.print("Transmitting ...");

  byte d[] = { 0xaa, 0xab };

  int state = radio.transmit(d, sizeof(d));

  if (state == RADIOLIB_ERR_NONE) {
    Serial.println("OK");
  } else if (state == RADIOLIB_ERR_PACKET_TOO_LONG) {
    Serial.println("TOO LONG");
  } else {
    Serial.print("ERROR");
    Serial.println(state);
  }

  //delay(1000);
}

/*
void loop() {
  // check if the flag is set
  if(receivedFlag) {
    // reset flag
    receivedFlag = false;

    // you can read received data as an Arduino String
    String str;
    int state = radio.readData(str);

    if (state == RADIOLIB_ERR_NONE) {
      // packet was successfully received
      Serial.println(F("[CC1101] Received packet!"));

      // print data of the packet
      Serial.print(F("[CC1101] Data:\t\t"));
      Serial.println(str);

      // print RSSI (Received Signal Strength Indicator)
      // of the last received packet
      Serial.print(F("[CC1101] RSSI:\t\t"));
      Serial.print(radio.getRSSI());
      Serial.println(F(" dBm"));

      // print LQI (Link Quality Indicator)
      // of the last received packet, lower is better
      Serial.print(F("[CC1101] LQI:\t\t"));
      Serial.println(radio.getLQI());

    } else if (state == RADIOLIB_ERR_CRC_MISMATCH) {
      // packet was received, but is malformed
      Serial.println(F("CRC error!"));

    } else {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);

    }

    // put module back to listen mode
    radio.startReceive();
  }
}
*/
