#include "AllStaticMode.h"
#include <iostream>
#include "LEDModeHandler.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Widget.hpp>
#include "ColorPickerModule.h"

const std::string cta::AllStaticMode::TYPE = "AllStatic";

cta::AllStaticMode::AllStaticMode(cta::ControllerApp* app) :
	cta::LEDMode(app, TYPE, app->getMainWindowPointer(), app->getWindowGUIPointer(), app->getArduinoConnector()) {

	allStaticPanel = tgui::Panel::create();
	allStaticPanel->setSize("70%", "100%");
	allStaticPanel->setPosition("30%", "0%");

	allStaticPanel->getSharedRenderer()->setBackgroundColor(sf::Color(35, 35, 35, 255));

	windowGui->add(allStaticPanel, "allStaticPanel");

	colorPicker = new cta::ColorPickerModule(allStaticPanel);

	this->deActivate();

}

void cta::AllStaticMode::draw(int dt) {


}

void cta::AllStaticMode::tick(int dt) {

	if (arduinoConnector->isConnected()) {
		unsigned char newColorData[] = { 1,colorPicker->getColor().r,colorPicker->getColor().g,colorPicker->getColor().b };
		arduinoConnector->sendDataSlow(newColorData, 4, 0, 4);
	}


}

void cta::AllStaticMode::handleEvent(sf::Event e) {


}

void cta::AllStaticMode::activate() {
	allStaticPanel->setVisible(true);
}

void cta::AllStaticMode::deActivate() {
	allStaticPanel->setVisible(false);
}
