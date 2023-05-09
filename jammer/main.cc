#include <Arduino.h>
#include "cc1101.h"

#define RADIO_CS_PIN 10

using namespace CC1101;

Radio radio(RADIO_CS_PIN);

uint8_t buf[64];

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("Starting ...");
  delay(3000);

  if (radio.begin() == STATUS_ERROR_CHIP_NOT_FOUND) {
    Serial.println("Chip not found!");
    for (;;);
  }

  radio.setModulation(MOD_ASK_OOK);

  radio.setFrequency(433.500 + 0.05);
  radio.setChannelSpacing(50);

  radio.setDataRate(60);
  radio.setOutputPower(10);
  radio.setPacketLengthMode(PKT_LEN_MODE_FIXED, sizeof(buf));
  radio.setAddressFilteringMode(ADDR_FILTER_MODE_NONE);
  radio.setSyncMode(SYNC_MODE_NO_PREAMBLE);
  radio.setCrc(false);

  memset(buf, 0xff, sizeof(buf));
}

void loop() {
  for (int ch = 0; ch < 25; ch++) {
    radio.setChannel(ch);
    radio.transmit(buf, sizeof(buf));
  }
}

