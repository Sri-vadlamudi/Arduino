/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 Update 2014 - TMRh20
 */

/**
 * Simplest possible example of using RF24Network,
 *
 * RECEIVER NODE
 * Listens for messages from the transmitter and prints them out.
 */

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>


RF24 radio(9,10);                // nRF24L01(+) radio attached using Getting Started board 

RF24Network network(radio);      // Network uses that radio
const uint16_t node00 = 00;   // Address of this wifi node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the tank node in Octal format
const uint16_t node02 = 02;    // Address of the other node in Octal format

const unsigned long interval = 2000; //ms  // How often to send 'hello world to the other unit

unsigned long last_sent;
//
//struct payload_t {                 // Structure of our payload
//  unsigned long ms;
//  unsigned long counter;
//};

float indata1[6];
void setup(void)
{
 SPI.begin();
  radio.begin();
  network.begin(90, node01 );  //(channel, node address)
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MIN);

  Serial.begin(9600);
     bool result=radio.isChipConnected();
  Serial.print("result:");
  Serial.println(result);
  
}

void loop(void){
  
  network.update();                  // Check the network regularly

receivedata();
  
  }
void receivedata(){
 float indata[6];
  while ( network.available() ) {     // Is there anything ready for us?
    
    RF24NetworkHeader header;        // If so, grab it and print it out

    network.read(header,&indata,sizeof(indata));
     if (header.from_node == 00){
      indata1[0]=indata[0];
       indata1[1]=indata[1];
        indata1[2]=indata[2];
         indata1[3]=indata[3];
          indata1[4]=indata[4];
           indata1[5]=indata[5];
         
           
    }
    Serial.print("Received data:");
    Serial.println(indata1[0]);
    Serial.println(indata1[1]);
    Serial.println(indata1[2]);
    Serial.println(indata1[3]);
    Serial.println(indata1[4]);
    Serial.println(indata1[5]);
  }
}
