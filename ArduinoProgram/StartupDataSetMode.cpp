#include "StartupDataSetMode.h"
#include <Arduino.h>
#include <EEPROM.h>
#include "Handler.h"

StartupDataSetMode::StartupDataSetMode() : ArduinoMode(ArduinoMode::ModeID::STARTUP_DATA_SET_MODE) {}

void StartupDataSetMode::handleData(byte data) {
	switch (currentByteInStream) {
	case 0:
		numStoredBytes = data;
		if (data <= 64)
			EEPROM.update(0, data);
		break;
	default:
		if (currentByteInStream <= 65) {
			EEPROM.update(currentByteInStream, data);
			if (currentByteInStream == numStoredBytes + 1) {
				Handler::setCurrentMode(Handler::getModeByID(ArduinoMode::ModeID::NO_MODE));
			}
		}
		break;
	}

	currentByteInStream++;
}

