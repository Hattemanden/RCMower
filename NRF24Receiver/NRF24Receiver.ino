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

  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();
  
  radio.printDetails();
}

void loop() {
  if (radio.available()){
      unsigned long gotTime;
      bool done = false;

      while (!done){
          done = radio.read(&gotTime, sizeof(unsigned long));
          printf("Got time: %lu...\n\r", gotTime);

          //Little delay to let the other unit shift to receive mode
          delay(20);
        }

        radio.stopListening();

        radio.write(&gotTime, sizeof(unsigned long));
        printf("Sent response. \n\r");

        radio.startListening();
    }
}
