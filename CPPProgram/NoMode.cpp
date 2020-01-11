#include "NoMode.h"
#include <iostream>
#include "LEDModeHandler.h"

const std::string cta::NoMode::TYPE = "None";

cta::NoMode::NoMode(cta::ControllerApp* app) :
	cta::LEDMode(app, TYPE, app->getMainWindowPointer(), app->getArduinoConnector()) {

}

void cta::NoMode::draw(int dt) {

	// Draw the background
	sf::RectangleShape bg(window->getView().getSize());
	bg.setFillColor(sf::Color(34, 40, 43));
	window->draw(bg);

	// Draw the sidebar
	std::vector<cta::LEDMode*> allModes = cta::LEDModeHandler::getAllModes();
	int numModes = allModes.size();
	sf::RectangleShape shape(sf::Vector2f(window->getView().getSize().x / 4, window->getView().getSize().y / numModes));
	for (int i = 0; i < numModes; i++) {

		shape.setPosition(0, i * shape.getSize().y);

		if (mouseIsHovering[i] || allModes[i] == this) {
			// Mouse is within box, or this is the current mode
			shape.setFillColor(sf::Color(49, 50, 51));
		} else {
			// Mouse is not within box
			shape.setFillColor(sf::Color(16, 19, 20));
		}

		shape.setOutlineThickness(-1);
		shape.setOutlineColor(sf::Color::Black);
		window->draw(shape);

		sf::Text boxTitle;
		boxTitle.setFont(app->getMainFont());
		boxTitle.setString(allModes[i]->getType());
		boxTitle.setCharacterSize(32.0 / 1280.0 * window->getView().getSize().x);
		boxTitle.setFillColor(sf::Color::White);

		int textOffsetX = boxTitle.getGlobalBounds().left - boxTitle.getPosition().x;
		int textOffsetY = boxTitle.getGlobalBounds().top - boxTitle.getPosition().y;

		sf::Vector2f textOffset(-textOffsetX, -textOffsetY);

		boxTitle.setPosition(
			shape.getPosition() + textOffset +
			sf::Vector2f(shape.getSize()) / 2.f -
			sf::Vector2f(boxTitle.getGlobalBounds().width / 2, boxTitle.getGlobalBounds().height / 2)
		);

		window->draw(boxTitle);

	}

	// Draw the arduino status

	sf::Text arduinoStatusTitle;
	arduinoStatusTitle.setFont(app->getMainFont());

	std::string arduinoStatusText = "";
	if (arduinoConnector->isConnected()) {
		arduinoStatusText = "ARDUINO CONNECTED";
	} else {
		arduinoStatusText = "ARDUINO NOT CONNECTED";
	}

	arduinoStatusTitle.setString(arduinoStatusText);
	arduinoStatusTitle.setCharacterSize(48.0 / 1280.0 * window->getView().getSize().x);
	arduinoStatusTitle.setFillColor(sf::Color(16, 19, 20));

	int arduinoStatusTextOffsetX = arduinoStatusTitle.getGlobalBounds().left - arduinoStatusTitle.getPosition().x;
	int arduinoStatusTextOffsetY = arduinoStatusTitle.getGlobalBounds().top - arduinoStatusTitle.getPosition().y;

	sf::Vector2f arduinoStatustextOffset(-arduinoStatusTextOffsetX, -arduinoStatusTextOffsetY);
	sf::Vector2f mainAreaCenter(window->getView().getSize().x * 5 / 8, window->getView().getSize().y / 2);

	arduinoStatusTitle.setPosition(
		mainAreaCenter +
		arduinoStatustextOffset -
		sf::Vector2f(arduinoStatusTitle.getGlobalBounds().width / 2, arduinoStatusTitle.getGlobalBounds().height / 2)
	);

	window->draw(arduinoStatusTitle);


}

void cta::NoMode::tick(int dt) {

	if (mouseIsHovering.size() != cta::LEDModeHandler::getNumberOfModes()) {
		mouseIsHovering = std::vector<bool>(cta::LEDModeHandler::getNumberOfModes(), false);
	}

	for (int i = 0; i < mouseIsHovering.size(); i++) {
		if (mouseX >= 0 && mouseX < window->getView().getSize().x / 4 &&
			mouseY >= window->getView().getSize().y * i / mouseIsHovering.size() &&
			mouseY < window->getView().getSize().y * (i + 1) / mouseIsHovering.size() &&
			mouseIsInWindow) {
			mouseIsHovering[i] = true;
		} else {
			mouseIsHovering[i] = false;
		}
	}



}

void cta::NoMode::handleEvent(sf::Event e) {

	if (e.type == sf::Event::MouseMoved) {
		sf::Vector2i mousePos(e.mouseMove.x, e.mouseMove.y);
		sf::Vector2f relativeMousePos = window->mapPixelToCoords(mousePos);
		mouseX = relativeMousePos.x;
		mouseY = relativeMousePos.y;
	} else if (e.type == sf::Event::MouseLeft) {
		mouseIsInWindow = false;
	} else if (e.type == sf::Event::MouseEntered) {
		mouseIsInWindow = true;
	} else if (e.type == sf::Event::MouseButtonPressed) {
		if (e.mouseButton.button == sf::Mouse::Left) {
			for (int i = 0; i < mouseIsHovering.size(); i++) {
				if (mouseIsHovering[i]) {
					app->setCurrentLEDMode(cta::LEDModeHandler::getAllModes()[i]);
				}
			}
		}
	}

}

void cta::NoMode::activate() {
	sf::Vector2i mousePos = sf::Mouse::getPosition();
	sf::Vector2f relativeMousePos = window->mapPixelToCoords(mousePos);
	mouseX = relativeMousePos.x;
	mouseY = relativeMousePos.y;
	mouseIsInWindow = true;
}

void cta::NoMode::deActivate() {

}
