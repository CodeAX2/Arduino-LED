#include "TwoColorWaveMode.h"
#include <iostream>
#include "LEDModeHandler.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Widget.hpp>
#include "ColorPickerModule.h"

const std::string cta::TwoColorWaveMode::TYPE = "TwoColorWave";

cta::TwoColorWaveMode::TwoColorWaveMode(cta::ControllerApp* app) :
	cta::LEDMode(app, TYPE, app->getMainWindowPointer(), app->getWindowGUIPointer(), app->getArduinoConnector()) {

	twoColorWavePanel = tgui::Panel::create();
	twoColorWavePanel->setSize("70%", "100%");
	twoColorWavePanel->setPosition("30%", "0%");
	twoColorWavePanel->getSharedRenderer()->setBackgroundColor(sf::Color(35, 35, 35, 255));
	windowGui->add(twoColorWavePanel, "twoColorWavePanel");

	color1Button = tgui::Button::create();
	color1Button->setSize("20%", "10%");
	color1Button->setPosition("50% - width - 5", "15");
	color1Button->setText("Pick Color 1");
	color1Button->setTextSize(24);
	color1Button->connect("pressed", [&]() {
		colorPicker->setColor(color1);
		changingColor = &color1;
		changingDisplayColor = color1Display;
		colorPicker->show();
		confirmColorButton->setVisible(true);
		color2Button->setEnabled(false);
		}
	);
	twoColorWavePanel->add(color1Button, "color1Button");

	color2Button = tgui::Button::create();
	color2Button->setSize("20%", "10%");
	color2Button->setPosition("50% + 5", "15");
	color2Button->setText("Pick Color 2");
	color2Button->setTextSize(24);
	color2Button->connect("pressed", [&]() {
		colorPicker->setColor(color2);
		changingColor = &color2;
		changingDisplayColor = color2Display;
		colorPicker->show();
		confirmColorButton->setVisible(true);
		color1Button->setEnabled(false);
		}
	);
	twoColorWavePanel->add(color2Button, "color2Button");


	colorPicker = new cta::ColorPickerModule(twoColorWavePanel, "100%", "100%", 0, "15%");
	colorPicker->hide();

	color1 = sf::Color(1, 255, 59);
	color2 = sf::Color(255, 1, 255);

	color1Display = tgui::Panel::create();
	color1Display->setSize("20%", "2%");
	color1Display->setPosition("50% - width - 5", "10% + 15 + 2");
	color1Display->getRenderer()->setBackgroundColor(color1);
	twoColorWavePanel->add(color1Display, "color1Display");

	color2Display = tgui::Panel::create();
	color2Display->setSize("20%", "2%");
	color2Display->setPosition("50% + 5", "10% + 15 + 2");
	color2Display->getRenderer()->setBackgroundColor(color2);
	twoColorWavePanel->add(color2Display, "color2Display");

	confirmColorButton = tgui::Button::create();
	confirmColorButton->setSize("20%", "10%");
	confirmColorButton->setPosition("50% - width/2", "100% - 15% - height - 30");
	confirmColorButton->setText("Confirm Color");
	confirmColorButton->setTextSize(24);
	confirmColorButton->connect("pressed", [&]() {
		*changingColor = colorPicker->getColor();
		changingDisplayColor->getRenderer()->setBackgroundColor(*changingColor);
		colorPicker->hide();
		confirmColorButton->setVisible(false);
		color1Button->setEnabled(true);
		color2Button->setEnabled(true);
		needsUpdating = true;
		}
	);
	confirmColorButton->setVisible(false);
	twoColorWavePanel->add(confirmColorButton, "confirmColorButton");

	delayLabel = tgui::Label::create();
	delayLabel->setSize("15%", "10%");
	delayLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	delayLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	delayLabel->setTextSize(24);
	delayLabel->setText("Delay Time");
	delayLabel->setPosition("50% - width - 5", "100% - height - 15 - 5% - 5");
	twoColorWavePanel->add(delayLabel, "delayLabel");


	delayEditBox = tgui::EditBox::create();
	delayEditBox->setSize("15%", "5%");
	delayEditBox->setPosition("50% - width - 5", "100% - height - 15");
	delayEditBox->setTextSize(16);
	delayEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	delayEditBox->setText("50");
	delayEditBox->setCaretPosition(0);
	delayEditBox->setMaximumCharacters(5);
	delayEditBox->connect("TextChanged", &cta::TwoColorWaveMode::textChanged, this);
	twoColorWavePanel->add(delayEditBox, "delayEditBox");

	waveLabel = tgui::Label::create();
	waveLabel->setSize("15%", "10%");
	waveLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	waveLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	waveLabel->setTextSize(24);
	waveLabel->setText("Wave Length");
	waveLabel->setPosition("50% + 5", "100% - height - 15 - 5% - 5");
	twoColorWavePanel->add(waveLabel, "waveLabel");


	waveLengthEditBox = tgui::EditBox::create();
	waveLengthEditBox->setSize("15%", "5%");
	waveLengthEditBox->setPosition("50% + 5", "100% - height - 15");
	waveLengthEditBox->setTextSize(16);
	waveLengthEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	waveLengthEditBox->setText("7");
	waveLengthEditBox->setCaretPosition(0);
	waveLengthEditBox->setMaximumCharacters(4);
	waveLengthEditBox->connect("TextChanged", &cta::TwoColorWaveMode::textChanged, this);
	twoColorWavePanel->add(waveLengthEditBox, "waveLengthEditBox");

	this->deActivate();

}

void cta::TwoColorWaveMode::draw(int dt) {


}

void cta::TwoColorWaveMode::tick(int dt) {

	if (needsUpdating && arduinoConnector->isConnected() && !arduinoConnector->isRestarting()) {
		unsigned char bytesToSend[] = { 6, color1.r, color1.g, color1.b, color2.r, color2.g, color2.b, offsetChangeDelay, offsetChangeDelay >> 8, waveLength, waveLength >> 8 };
		arduinoConnector->sendDataSlow(bytesToSend, 11, 0, 4);

		needsUpdating = false;

	}


}

void cta::TwoColorWaveMode::handleEvent(sf::Event e) {


}

void cta::TwoColorWaveMode::activate() {
	twoColorWavePanel->setVisible(true);
	needsUpdating = true;
}

void cta::TwoColorWaveMode::deActivate() {
	twoColorWavePanel->setVisible(false);
}

void cta::TwoColorWaveMode::textChanged(tgui::Widget::Ptr widget, const std::string& signalName) {
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
