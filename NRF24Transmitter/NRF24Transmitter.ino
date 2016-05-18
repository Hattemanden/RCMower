#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9, 10);

const uint64_t pipes [1] = {0xF0F0F0F0E1LL};

const int VRx = 0;
const int VRy = 1;
const int buttonPin = 6;

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

	radio.openWritingPipe(pipes[0]);

	radio.startListening();
	
	radio.printDetails();

	pinMode(buttonPin, INPUT_PULLUP);

	data.x = 127;
	data.y = 127;
	data.button = 0;
}

void loop() {
	radio.stopListening();

	data.x = readPot(VRx);
	data.y = readPot(VRy);
	data.button = readPin(buttonPin);

	radio.write(&data, sizeof(data));
}

byte readPot(const int pin){
	int value = analogRead(pin);
	return map(value, 0, 1023, 0, 255);
}

byte readPin(const int pin){
	return digitalRead(pin);
}