#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9, 10);

const uint64_t pipes [2] = {0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL};

void setup() {
  Serial.begin(57600);
  printf_begin();
  printf("\n\rGetting started with NRF24L01\n\r");

  radio.begin();
  radio.setRetries(15, 15);
  radio.setPayloadSize(8);

  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1, pipes[1]);

  radio.startListening();
  
  radio.printDetails();
}

void loop() {
  radio.stopListening();

  unsigned long time = millis();
  printf("Now sending %lu...", time);

  bool OK = radio.write(&time, sizeof(unsigned long));

  if (OK){
      printf("OK...");
    } else {
      printf("Failed...");  
    }

    radio.startListening();

   // Continue listening
   unsigned long startedWaitingAt = millis();
   bool timeOut = false;

   while(! radio.available() && !timeOut){
      if (millis() - timeOut > 200){
          timeOut = false;
        }

   if(timeOut){
      printf("Failed, timeout...\n\r");
      timeOut = false;
    } else {
        unsigned long gotTime;
        radio.read(&gotTime, sizeof(unsigned long));
        printf("Got response %lu, round-trip delay: %lu\n\r", gotTime, millis()-gotTime);
      }
    }
    delay(20);
}
