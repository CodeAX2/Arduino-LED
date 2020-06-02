#include "LEDMode.h"
#include "LEDModeHandler.h"

cta::LEDMode::LEDMode(cta::ControllerApp* app, std::string type, sf::RenderWindow* window, tgui::Gui* windowGui, cta::ArduinoConnector* arduinoConnector) {
	this->app = app;
	this->type = type;
	this->window = window;
	this->windowGui = windowGui;
	this->arduinoConnector = arduinoConnector;
	cta::LEDModeHandler::addMode(this);
}

void cta::LEDMode::draw(int dt) {
}

void cta::LEDMode::tick(int dt) {
}

void cta::LEDMode::handleEvent(sf::Event e) {
}

std::string cta::LEDMode::getType() {
	return type;
}

void cta::LEDMode::activate() {

}

void cta::LEDMode::deActivate() {

}