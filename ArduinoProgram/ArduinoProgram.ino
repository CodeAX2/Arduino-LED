#include <MemoryFree.h>
#include "ChannelRGB.h"
#include "Handler.h"
#include "ArduinoMode.h"
#include "AllStaticColorMode.h"
#include "AudioMode.h"
#include "GradientColorMode.h"
#include "IndividualStaticColorMode.h"
#include "NoMode.h"
#include "RainbowColorWaveMode.h"
#include "SetupMode.h"
#include "StartupDataSetMode.h"
#include "TwoColorWaveMode.h"
#include <Arduino.h>
#include <EEPROM.h>
#include <PololuLedStrip.h>

byte defaultStartupData[] = { 0, 60, 0, 0, 1, 2, 1, 255, 0, 0 };

void setup() {

	Serial.begin(9600);

	new NoMode();
	new AllStaticColorMode();
	new AudioMode();
	new GradientColorMode();
	new IndividualStaticColorMode();
	new RainbowColorWaveMode();
	new SetupMode();
	new StartupDataSetMode();
	new TwoColorWaveMode();

	Handler::setCurrentMode(Handler::getModeByID(ArduinoMode::ModeID::NO_MODE));

	byte numStartupBytes = EEPROM.read(0);

	if (numStartupBytes == 0 || numStartupBytes > 64) {
		for (int i = 0; i < sizeof(defaultStartupData) / sizeof(byte); i++) {
			Handler::getCurrentMode()->handleData(defaultStartupData[i]);
		}
		EEPROM.update(0, 0);
	} else {

		for (int i = 0; i < numStartupBytes; i++) {
			byte currentByte = EEPROM.read(1 + i);
			Serial.print((int)defaultStartupData[i]);
			Serial.print(" ");
			Handler::getCurrentMode()->handleData(defaultStartupData[i]);
		}
		Serial.println();
	}

	Serial.print('j');

}

void loop() {
	while (Serial.available() > 0) {
		// Recieved data over serial
		byte incomingByte = Serial.read();
		Handler::getCurrentMode()->handleData(incomingByte);
	}

	Handler::getCurrentMode()->loop();

}