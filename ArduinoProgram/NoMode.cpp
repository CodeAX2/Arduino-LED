#include "NoMode.h"
#include "Handler.h"

NoMode::NoMode() : ArduinoMode(ArduinoMode::ModeID::NO_MODE) {
}

void NoMode::handleData(byte data) {
	ArduinoMode* newMode = Handler::getModeByID((ArduinoMode::ModeID)data);
	if (newMode != nullptr) {
		Handler::setCurrentMode(newMode);
	}
}