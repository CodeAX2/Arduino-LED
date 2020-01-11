#include "AllStaticColorMode.h"
#include <iostream>
#include "LEDModeHandler.h"

const std::string cta::AllStaticColorMode::TYPE = "Static Color";

cta::AllStaticColorMode::AllStaticColorMode(cta::ControllerApp* app) :
	cta::LEDMode(app, TYPE, app->getMainWindowPointer(), app->getArduinoConnector()) {

	selectedColor.r = 255;
	selectedColorDarkness.r = 255;


	// Create the color palette texture
	sf::Image colorPaletteImage;
	colorPaletteImage.create(255 * 6, 255);
	for (int i = 0; i < 255 * 6; i++) {
		for (int j = 0; j < 255; j++) {

			sf::Color color;
			switch (i / 255) {
			case 0:
				color.r = 255;
				color.g = i;
				color.b = 0;
				break;
			case 1:
				color.r = 255 * 2 - i;
				color.g = 255;
				color.b = 0;
				break;
			case 2:
				color.r = 0;
				color.g = 255;
				color.b = i - 255 * 2;
				break;
			case 3:
				color.r = 0;
				color.g = 255 * 4 - i;
				color.b = 255;
				break;
			case 4:
				color.r = i - 255 * 4;
				color.g = 0;
				color.b = 255;
				break;
			case 5:
				color.r = 255;
				color.g = 0;
				color.b = 255 * 6 - i;
				break;
			}

			color.r = color.r + (255 - color.r) * j / 255.0;
			color.g = color.g + (255 - color.g) * j / 255.0;
			color.b = color.b + (255 - color.b) * j / 255.0;

			colorPaletteImage.setPixel(i, j, color);
		}
	}

	colorPaletteTexture = new sf::Texture();
	colorPaletteTexture->loadFromImage(colorPaletteImage);


}

void cta::AllStaticColorMode::draw(int dt) {

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

	// Draw the color palette
	sf::RectangleShape colorPalette(window->getView().getSize() / 3.f);
	colorPalette.setPosition(sf::Vector2f(window->getView().getSize().x * 5 / 8, window->getView().getSize().y / 3) - colorPalette.getSize() / 2.f);
	colorPalette.setTexture(colorPaletteTexture);
	colorPalette.setOutlineColor(sf::Color(16, 19, 20));
	colorPalette.setOutlineThickness(5);
	window->draw(colorPalette);

	// Draw the selected color
	sf::CircleShape colorSelector(10, 20);
	sf::Vector2f relativePos(selectedColorX - colorSelector.getRadius(), selectedColorY - colorSelector.getRadius());
	colorSelector.setPosition(colorPalette.getPosition() + relativePos);
	colorSelector.setFillColor(selectedColor);
	colorSelector.setOutlineColor(sf::Color::White);
	colorSelector.setOutlineThickness(2);
	window->draw(colorSelector);

	// Draw the darkness gradient
	sf::RectangleShape darknessGradient(sf::Vector2f(window->getView().getSize().x / 3 - 40 * window->getView().getSize().x / 1280, window->getView().getSize().y / 21));
	darknessGradient.setFillColor(sf::Color::White);
	darknessGradient.setPosition(colorPalette.getPosition() + sf::Vector2f(20 * window->getView().getSize().x / 1280, colorPalette.getSize().y + 20 * window->getView().getSize().y / 720));
	darknessGradient.setOutlineColor(sf::Color(16, 19, 20));
	darknessGradient.setOutlineThickness(5);

	sf::Image darknessGradientImage;
	darknessGradientImage.create(255, 1);
	for (int i = 0; i < 255; i++) {
		sf::Color darknessGradientColor;
		darknessGradientColor.r = selectedColor.r * i / 255;
		darknessGradientColor.g = selectedColor.g * i / 255;
		darknessGradientColor.b = selectedColor.b * i / 255;
		darknessGradientImage.setPixel(i, 0, darknessGradientColor);
	}

	sf::Texture darknessGradientTexture;
	darknessGradientTexture.loadFromImage(darknessGradientImage);

	darknessGradient.setTexture(&darknessGradientTexture);
	window->draw(darknessGradient);

	// Draw the darkness selector
	sf::RectangleShape darknessSelector(sf::Vector2f(10, darknessGradient.getSize().y + 10));
	darknessSelector.setOutlineColor(sf::Color::White);
	darknessSelector.setOutlineThickness(2);
	darknessSelector.setFillColor(selectedColorDarkness);
	darknessSelector.setPosition(darknessGradient.getPosition() + sf::Vector2f(selectedColorDarknessX - darknessSelector.getSize().x / 2, -5));
	window->draw(darknessSelector);

}

void cta::AllStaticColorMode::tick(int dt) {

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

	if (mouseIsDown) {

		sf::Vector2f colorPaletteSize = window->getView().getSize() / 3.f;
		sf::Vector2f colorPalettePos = sf::Vector2f(window->getView().getSize().x * 5 / 8, window->getView().getSize().y / 3) - colorPaletteSize / 2.f;

		sf::Vector2f darknessGradientSize = sf::Vector2f(window->getView().getSize().x / 3 - 40 * window->getView().getSize().x / 1280, window->getView().getSize().y / 21);
		sf::Vector2f darknessGradientPos = colorPalettePos + sf::Vector2f(20 * window->getView().getSize().x / 1280, colorPaletteSize.y + 20 * window->getView().getSize().y / 720);

		bool colorChanged = false;

		if (mouseXBeginClick - colorPalettePos.x >= 0 && mouseXBeginClick - colorPalettePos.x < colorPaletteSize.x &&
			mouseYBeginClick - colorPalettePos.y >= 0 && mouseYBeginClick - colorPalettePos.y < colorPaletteSize.y) {

			// Began clicking in the color palette

			int newSelectedColorX = mouseX - colorPalettePos.x;
			int newSelectedColorY = mouseY - colorPalettePos.y;

			if (newSelectedColorX < 0) newSelectedColorX = 0;
			if (newSelectedColorX >= colorPaletteSize.x) newSelectedColorX = colorPaletteSize.x - 1;

			if (newSelectedColorY < 0) newSelectedColorY = 0;
			if (newSelectedColorY >= colorPaletteSize.y) newSelectedColorY = colorPaletteSize.y - 1;

			selectedColorX = newSelectedColorX;
			selectedColorY = newSelectedColorY;

			colorChanged = true;

		} else if (mouseXBeginClick - darknessGradientPos.x >= 0 && mouseXBeginClick - darknessGradientPos.x < darknessGradientSize.x &&
			mouseYBeginClick - darknessGradientPos.y >= 0 && mouseYBeginClick - darknessGradientPos.y < darknessGradientSize.y) {

			// Began clicking in the darkness gradient

			int newSelectedGradientX = mouseX - darknessGradientPos.x;

			if (newSelectedGradientX < 0) newSelectedGradientX = 0;
			if (newSelectedGradientX >= darknessGradientSize.x) newSelectedGradientX = darknessGradientSize.x - 1;

			selectedColorDarknessX = newSelectedGradientX;

			colorChanged = true;

		}


		if (colorChanged) {
			sf::Color newSelectedColor;

			int i = selectedColorX * 255 * 6 / colorPaletteSize.x;
			int j = selectedColorY * 255 / colorPaletteSize.y;

			switch (i / 255) {
			case 0:
				newSelectedColor.r = 255;
				newSelectedColor.g = i;
				newSelectedColor.b = 0;
				break;
			case 1:
				newSelectedColor.r = 255 * 2 - i;
				newSelectedColor.g = 255;
				newSelectedColor.b = 0;
				break;
			case 2:
				newSelectedColor.r = 0;
				newSelectedColor.g = 255;
				newSelectedColor.b = i - 255 * 2;
				break;
			case 3:
				newSelectedColor.r = 0;
				newSelectedColor.g = 255 * 4 - i;
				newSelectedColor.b = 255;
				break;
			case 4:
				newSelectedColor.r = i - 255 * 4;
				newSelectedColor.g = 0;
				newSelectedColor.b = 255;
				break;
			case 5:
				newSelectedColor.r = 255;
				newSelectedColor.g = 0;
				newSelectedColor.b = 255 * 6 - i;
				break;
			}

			newSelectedColor.r = newSelectedColor.r + (255 - newSelectedColor.r) * j / 255.0;
			newSelectedColor.g = newSelectedColor.g + (255 - newSelectedColor.g) * j / 255.0;
			newSelectedColor.b = newSelectedColor.b + (255 - newSelectedColor.b) * j / 255.0;

			selectedColor = newSelectedColor;

			sf::Color newSelectedColorDarkness = selectedColor;
			newSelectedColorDarkness.r *= selectedColorDarknessX / darknessGradientSize.x;
			newSelectedColorDarkness.g *= selectedColorDarknessX / darknessGradientSize.x;
			newSelectedColorDarkness.b *= selectedColorDarknessX / darknessGradientSize.x;

			selectedColorDarkness = newSelectedColorDarkness;

			if (arduinoConnector->isConnected()) {
				unsigned char newColorData[] = { 1,selectedColorDarkness.r,selectedColorDarkness.g,selectedColorDarkness.b };
				arduinoConnector->sendDataSlow(newColorData, 4, 0, 4);
			}
		}

	}

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
	} else if (e.type == sf::Event::MouseButtonPressed) {
		if (e.mouseButton.button == sf::Mouse::Left) {
			mouseIsDown = true;
			mouseXBeginClick = window->mapPixelToCoords(sf::Mouse::getPosition(*window)).x;
			mouseYBeginClick = window->mapPixelToCoords(sf::Mouse::getPosition(*window)).y;
			for (int i = 0; i < mouseIsHovering.size(); i++) {
				if (mouseIsHovering[i]) {
					app->setCurrentLEDMode(cta::LEDModeHandler::getAllModes()[i]);
				}
			}
		}
	} else if (e.type == sf::Event::MouseButtonReleased) {
		if (e.mouseButton.button == sf::Mouse::Left) {
			mouseIsDown = false;
		}
	} else if (e.type == sf::Event::Resized) {
		double sizeScaleX = window->getSize().x / window->getView().getSize().x;
		double sizeScaleY = window->getSize().y / window->getView().getSize().y;

		selectedColorX *= sizeScaleX;
		selectedColorY *= sizeScaleY;

		selectedColorDarknessX *= sizeScaleX;

	}

}

void cta::AllStaticColorMode::activate() {
	sf::Vector2i mousePos = sf::Mouse::getPosition(*window);
	sf::Vector2f relativeMousePos = window->mapPixelToCoords(mousePos);
	mouseX = relativeMousePos.x;
	mouseY = relativeMousePos.y;
	mouseIsInWindow = true;
	mouseIsDown = false;
	selectedColorDarknessX = window->getView().getSize().x / 3 - 40 * window->getView().getSize().x / 1280 - 1;
}

void cta::AllStaticColorMode::deActivate() {

}
