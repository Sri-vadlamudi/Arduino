/**
 * Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * version 2 as published by the Free Software Foundation.
 *
 * Update 2014 - TMRh20
 */

/**
 * Simplest possible example of using RF24Network
 *
 * TRANSMITTER NODE
 * Every 2 seconds, send a payload to the receiver node.
 */

#include <SPI.h>
#include <RF24.h>
#include <RF24Network.h>

RF24 radio(9, 10);                    // nRF24L01(+) radio attached using Getting Started board

RF24Network network(radio);          // Network uses that radio

const uint16_t this_node = 01;       // Address of our node in Octal format
const uint16_t other_node = 00;      // Address of the other node in Octal format

const unsigned long interval = 1000; // How often (in ms) to send 'hello world' to the other unit

unsigned long last_sent;             // When did we last send?
unsigned long packets_sent;          // How many have we sent already


//struct payload_t {                   // Structure of our payload
//  float ms ;
//  float counter;
//};

void setup(void) {
  Serial.begin(115200);
  if (!Serial) {
    // some boards need this because of native USB capability
  }
  Serial.println(F("RF24Network/examples/helloworld_tx/"));

  SPI.begin();
  if (!radio.begin()) {
    Serial.println(F("Radio hardware not responding!"));
    while (1) {
      // hold in infinite loop
    }
  }
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  bool result=radio.isChipConnected();
  Serial.print("result:");
  Serial.println(result);
}

void loop() {
int outdata[2];
outdata[0]=44;
outdata[1]=24;

  network.update(); // Check the network regularly

  unsigned long now = millis();

  // If it's time to send a message, send it!
  if (now - last_sent >= interval) {
    last_sent = now;

    Serial.print("Sending...");
   // payload_t payload = { 88 , 66 };
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header, &outdata, sizeof(outdata));
    if (ok){
      Serial.println("ok.");
     // Serial.println(payload.ms);
     // Serial.println(payload.counter);
      Serial.println(sizeof(outdata));
      }
    else{
      Serial.println("failed.");
      Serial.println(sizeof(outdata));
    }
  }

  delay(100);
  network.update();                  // Check the network regularly

  while (network.available()) {      // Is there anything ready for us?

    RF24NetworkHeader header;        // If so, grab it and print it out
    int indata[2];
    network.read(header, &indata, sizeof(indata));
    Serial.print("Received packet #");
    Serial.print(indata[0]);
    Serial.print(" at ");
    Serial.println(indata[1]);
  }
  delay(100);
}
