#include "SetupMode.h"
#include "Handler.h"

SetupMode::SetupMode() : ArduinoMode(ArduinoMode::ModeID::SETUP_MODE) {}

void SetupMode::handleData(byte data) {
	switch (currentByteInStream) {
	case 0:
		Handler::setNumLEDs(data);
		break;
	case 1:
		Handler::setNumLEDs(Handler::getNumLEDs() + 256 * (int)data);
		break;
	case 2:
		Handler::setRGBChannels(data, -1, -1);
		break;
	case 3:
		Handler::setRGBChannels(-1, data, -1);
		break;
	case 4:
		Handler::setRGBChannels(-1, -1, data);
		Handler::setCurrentMode(Handler::getModeByID(ArduinoMode::ModeID::NO_MODE));
		break;
	}

	currentByteInStream++;
}