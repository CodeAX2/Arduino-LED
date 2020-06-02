#include "RainbowWaveMode.h"
#include <iostream>
#include "LEDModeHandler.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Widget.hpp>
#include "ColorPickerModule.h"

const std::string cta::RainbowWaveMode::TYPE = "RainbowWave";

cta::RainbowWaveMode::RainbowWaveMode(cta::ControllerApp* app) :
	cta::LEDMode(app, TYPE, app->getMainWindowPointer(), app->getWindowGUIPointer(), app->getArduinoConnector()) {

	rainbowWavePanel = tgui::Panel::create();
	rainbowWavePanel->setSize("70%", "100%");
	rainbowWavePanel->setPosition("30%", "0%");
	rainbowWavePanel->getSharedRenderer()->setBackgroundColor(sf::Color(35, 35, 35, 255));
	windowGui->add(rainbowWavePanel, "rainbowWavePanel");

	delayLabel = tgui::Label::create();
	delayLabel->setSize("15%", "10%");
	delayLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	delayLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	delayLabel->setTextSize(24);
	delayLabel->setText("Delay Time");
	delayLabel->setPosition("50% - width - 5", "100% - height - 15 - 5% - 5");
	rainbowWavePanel->add(delayLabel, "delayLabel");


	delayEditBox = tgui::EditBox::create();
	delayEditBox->setSize("15%", "5%");
	delayEditBox->setPosition("50% - width - 5", "100% - height - 15");
	delayEditBox->setTextSize(16);
	delayEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	delayEditBox->setText("50");
	delayEditBox->setCaretPosition(0);
	delayEditBox->setMaximumCharacters(5);
	delayEditBox->connect("TextChanged", &cta::RainbowWaveMode::textChanged, this);
	rainbowWavePanel->add(delayEditBox, "delayEditBox");

	waveLabel = tgui::Label::create();
	waveLabel->setSize("15%", "10%");
	waveLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	waveLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	waveLabel->setTextSize(24);
	waveLabel->setText("Wave Length");
	waveLabel->setPosition("50% + 5", "100% - height - 15 - 5% - 5");
	rainbowWavePanel->add(waveLabel, "waveLabel");


	waveLengthEditBox = tgui::EditBox::create();
	waveLengthEditBox->setSize("15%", "5%");
	waveLengthEditBox->setPosition("50% + 5", "100% - height - 15");
	waveLengthEditBox->setTextSize(16);
	waveLengthEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	waveLengthEditBox->setText("35");
	waveLengthEditBox->setCaretPosition(0);
	waveLengthEditBox->setMaximumCharacters(4);
	waveLengthEditBox->connect("TextChanged", &cta::RainbowWaveMode::textChanged, this);
	rainbowWavePanel->add(waveLengthEditBox, "waveLengthEditBox");

	this->deActivate();

}

void cta::RainbowWaveMode::draw(int dt) {


}

void cta::RainbowWaveMode::tick(int dt) {

	if (needsUpdating && arduinoConnector->isConnected() && !arduinoConnector->isRestarting()) {

		unsigned char bytesToSend[] = { 7, offsetChangeDelay, offsetChangeDelay >> 8, waveLength, waveLength >> 8 };
		std::cout << arduinoConnector->sendDataSlow(bytesToSend, 5, 0, 5) << std::endl;

		needsUpdating = false;

	}


}

void cta::RainbowWaveMode::handleEvent(sf::Event e) {


}

void cta::RainbowWaveMode::activate() {
	rainbowWavePanel->setVisible(true);
	needsUpdating = true;
}

void cta::RainbowWaveMode::deActivate() {
	rainbowWavePanel->setVisible(false);
}

void cta::RainbowWaveMode::textChanged(tgui::Widget::Ptr widget, const std::string& signalName) {
	std::string widgetName = widget->getWidgetName().toAnsiString();

	// Update speed
	if (widgetName == "delayEditBox") {
		if (delayEditBox->getText().toAnsiString() != "")
			offsetChangeDelay = std::stoi(delayEditBox->getText().toAnsiString());

	} else if (widgetName == "waveLengthEditBox") {
		if (waveLengthEditBox->getText().toAnsiString() != "")
			waveLength = std::stoi(waveLengthEditBox->getText().toAnsiString());
	}

	needsUpdating = true;


}
