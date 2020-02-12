#include "NoMode.h"
#include <iostream>
#include "LEDModeHandler.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Widget.hpp>
#include "ColorPickerModule.h"

const std::string cta::NoMode::TYPE = "None";

cta::NoMode::NoMode(cta::ControllerApp* app) :
	cta::LEDMode(app, TYPE, app->getMainWindowPointer(), app->getWindowGUIPointer(), app->getArduinoConnector()) {

	noModePanel = tgui::Panel::create();
	noModePanel->setSize("70%", "100%");
	noModePanel->setPosition("30%", "0%");

	noModePanel->getSharedRenderer()->setBackgroundColor(sf::Color(35, 35, 35, 255));

	windowGui->add(noModePanel, "noModePanel");

	cta::ColorPickerModule cpm(noModePanel);

	return;

	arduinoStatusLabel = tgui::Label::create();
	arduinoStatusLabel->setSize("75%", "100%");
	arduinoStatusLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	arduinoStatusLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	arduinoStatusLabel->setTextSize(64);
	arduinoStatusLabel->setPosition("(parent.size - size) / 2");

	noModePanel->add(arduinoStatusLabel, "arduinoStatusLabel");


}

void cta::NoMode::draw(int dt) {


}

void cta::NoMode::tick(int dt) {

	std::string arduinoStatusText = "Arduino is not connected";
	if (app->getArduinoConnector()->isConnected()) {
		arduinoStatusText = "Arduino is connected";
	}

	//arduinoStatusLabel->setText(arduinoStatusText);

}

void cta::NoMode::handleEvent(sf::Event e) {


}

void cta::NoMode::activate() {
	noModePanel->setVisible(true);
}

void cta::NoMode::deActivate() {
	noModePanel->setVisible(false);
}