#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9, 10);

const uint64_t pipes [1] = {0xF0F0F0F0E1LL};

struct myData {
	byte x;
	byte y;
	byte button;
};

myData data;

void setup() {
	Serial.begin(57600);
	printf_begin();
	printf("\n\rGetting started with NRF24L01\n\r");

	radio.begin();
	radio.setRetries(15, 15);
	radio.setPayloadSize(8);

	radio.openReadingPipe(1, pipes[0]);

	radio.startListening();
	
	radio.printDetails();
}

void loop() {
	if (radio.available()){
		radio.read(&data, sizeof(data));
	}

	Serial.print(data.x);
	Serial.print("\t");
	Serial.print(data.y);
	Serial.print("\t");
	Serial.println(data.button);
}