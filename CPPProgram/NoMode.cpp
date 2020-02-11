#include "NoMode.h"
#include <iostream>
#include "LEDModeHandler.h"

const std::string cta::NoMode::TYPE = "None";

cta::NoMode::NoMode(cta::ControllerApp* app) :
	cta::LEDMode(app, TYPE, app->getMainWindowPointer(), app->getArduinoConnector()) {

}

void cta::NoMode::draw(int dt) {

	
}

void cta::NoMode::tick(int dt) {

	

}

void cta::NoMode::handleEvent(sf::Event e) {


}

void cta::NoMode::activate() {

}

void cta::NoMode::deActivate() {

}