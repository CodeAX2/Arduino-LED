#include "AllStaticColorMode.h"
#include <iostream>
#include "LEDModeHandler.h"

const std::string cta::AllStaticColorMode::TYPE = "Static Color";

cta::AllStaticColorMode::AllStaticColorMode(cta::ControllerApp* app) :
	cta::LEDMode(app, TYPE, app->getMainWindowPointer(), app->getArduinoConnector()) {

}

void cta::AllStaticColorMode::draw(int dt) {



}

void cta::AllStaticColorMode::tick(int dt) {


}

void cta::AllStaticColorMode::handleEvent(sf::Event e) {

	if (e.type == sf::Event::MouseMoved) {
		sf::Vector2i mousePos(e.mouseMove.x, e.mouseMove.y);
		sf::Vector2f relativeMousePos = window->mapPixelToCoords(mousePos);
		mouseX = relativeMousePos.x;
		mouseY = relativeMousePos.y;
	} else if (e.type == sf::Event::MouseLeft) {
		mouseIsInWindow = false;
	} else if (e.type == sf::Event::MouseEntered) {
		mouseIsInWindow = true;
	}

}
