#include "ArduinoMode.h"
#include "Handler.h"

ArduinoMode::ArduinoMode(ArduinoMode::ModeID id) {
	this->id = id;
	Handler::addMode(this);
}

ArduinoMode::ModeID ArduinoMode::getID() {
	return id;
}

void ArduinoMode::handleData(byte data) {
}

void ArduinoMode::loop() {
}

void ArduinoMode::activate() {
	currentByteInStream = 0;
	prevUpdate = 0;
}

void ArduinoMode::deactivate() {

}