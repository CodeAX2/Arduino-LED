#include "SplatoonMode.h"
#include <iostream>
#include "LEDModeHandler.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Widget.hpp>
#include "ColorPickerModule.h"

const std::string cta::SplatoonMode::TYPE = "Splatoon";

cta::SplatoonMode::SplatoonMode(cta::ControllerApp* app) :
	cta::LEDMode(app, TYPE, app->getMainWindowPointer(), app->getWindowGUIPointer(), app->getArduinoConnector()) {

	splatoonPanel = tgui::Panel::create();
	splatoonPanel->setSize("70%", "100%");
	splatoonPanel->setPosition("30%", "0%");

	splatoonPanel->getSharedRenderer()->setBackgroundColor(sf::Color(35, 35, 35, 255));

	windowGui->add(splatoonPanel, "splatoonPanel");


	delayEditBox = tgui::EditBox::create();
	delayEditBox->setSize("15%", "5%");
	delayEditBox->setPosition("50% - width - 5", "50% - height / 2");
	delayEditBox->setTextSize(16);
	delayEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	delayEditBox->setText("75");
	delayEditBox->setCaretPosition(0);
	delayEditBox->setMaximumCharacters(5);
	delayEditBox->connect("TextChanged", &cta::SplatoonMode::textChanged, this);
	splatoonPanel->add(delayEditBox, "delayEditBox");

	waveLengthEditBox = tgui::EditBox::create();
	waveLengthEditBox->setSize("15%", "5%");
	waveLengthEditBox->setPosition("50% + 5", "50% - height / 2");
	waveLengthEditBox->setTextSize(16);
	waveLengthEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	waveLengthEditBox->setText("2");
	waveLengthEditBox->setCaretPosition(0);
	waveLengthEditBox->setMaximumCharacters(4);
	waveLengthEditBox->connect("TextChanged", &cta::SplatoonMode::textChanged, this);
	splatoonPanel->add(waveLengthEditBox, "waveLengthEditBox");



	this->deActivate();

}

void cta::SplatoonMode::draw(int dt) {


}

void cta::SplatoonMode::tick(int dt) {

	timeSinceOffsetChange += dt;
	if (timeSinceOffsetChange >= offsetChangeDelay) {
		offset++;
		timeSinceOffsetChange = 0;
	}

	if (arduinoConnector->isConnected()) {

		unsigned char newColorData[53];
		newColorData[0] = 3;
		newColorData[1] = 10;
		newColorData[2] = 0;

		unsigned char* curByte = newColorData + 3;

		for (int i = 0; i < 10; i++) {
			sf::Color color;
			if (abs(i - offset) % (waveLength * 2) < waveLength) {
				color = sf::Color(1, 255, 59);
			} else {
				color = sf::Color(255, 1, 255);
			}

			*curByte = i;
			curByte++;

			*curByte = 0;
			curByte++;

			*curByte = color.r;
			curByte++;

			*curByte = color.g;
			curByte++;

			*curByte = color.b;
			curByte++;

		}


		arduinoConnector->sendDataSlow(newColorData, 53, 0, 4);
	}


}

void cta::SplatoonMode::handleEvent(sf::Event e) {


}

void cta::SplatoonMode::activate() {
	splatoonPanel->setVisible(true);
}

void cta::SplatoonMode::deActivate() {
	splatoonPanel->setVisible(false);
}

void cta::SplatoonMode::textChanged(tgui::Widget::Ptr widget, const std::string& signalName) {
	std::string widgetName = widget->getWidgetName().toAnsiString();

	// Update speed
	if (widgetName == "delayEditBox") {
		if (delayEditBox->getText().toAnsiString() != "")
			offsetChangeDelay = std::stoi(delayEditBox->getText().toAnsiString());
	} else if (widgetName == "waveLengthEditBox") {
		if (waveLengthEditBox->getText().toAnsiString() != "")
			waveLength = std::stoi(waveLengthEditBox->getText().toAnsiString());
	}


}