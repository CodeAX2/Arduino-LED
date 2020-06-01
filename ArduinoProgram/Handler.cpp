#include "Handler.h"
#include "ArduinoMode.h"
#include <MemoryFree.h>

ArduinoMode* Handler::currentMode;
PololuLedStrip<12> Handler::ledStrip;
int Handler::numLed = 60;
int Handler::redChannel = 0;
int Handler::greenChannel = 1;
int Handler::blueChannel = 2;
rgb_color Handler::offColor(0, 0, 0);
ArduinoMode* Handler::allModes[16];

ArduinoMode* Handler::getCurrentMode() {
	return currentMode;
}

ArduinoMode* Handler::getModeByID(ArduinoMode::ModeID id) {
	for (int i = 0; i < sizeof(allModes) / sizeof(ArduinoMode*); i++) {
		if (allModes[i] != nullptr && allModes[i]->getID() == id) return allModes[i];
	}
	return nullptr;
}

PololuLedStrip<12>* Handler::getLEDStrip() {
	return &ledStrip;
}

int Handler::getNumLEDs() {
	return numLed;
}


void Handler::getRGBChannels(int* rChannel, int* gChannel, int* bChannel) {
	*rChannel = redChannel;
	*gChannel = greenChannel;
	*bChannel = blueChannel;
}

rgb_color Handler::getOffColor() {
	return offColor;
}

void Handler::setCurrentMode(ArduinoMode* newMode) {

	if (currentMode != nullptr) {
		currentMode->deactivate();
	}

	currentMode = newMode;
	currentMode->activate();

}

void Handler::setNumLEDs(int amount) {
	numLed = amount;
}

void Handler::setRGBChannels(int rChannel, int gChannel, int bChannel) {
	if (rChannel != -1)
		redChannel = rChannel;
	if (gChannel != -1)
		greenChannel = gChannel;
	if (bChannel != -1)
		blueChannel = bChannel;
}

void Handler::setOffColor(rgb_color color) {
	offColor = color;
}

void Handler::addMode(ArduinoMode* mode) {
	if (getModeByID(mode->getID()) == nullptr) {
		for (int i = 0; i < sizeof(allModes) / sizeof(ArduinoMode*); i++) {
			if (allModes[i] == nullptr) {
				allModes[i] = mode;
				return;
			}
		}
	}
}