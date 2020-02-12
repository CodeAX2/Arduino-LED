#include "NoMode.h"
#include <iostream>
#include "LEDModeHandler.h"
#include <TGUI/TGUI.hpp>
#include "HSPicker.h"
#include <TGUI/Widget.hpp>
#include "VPicker.h"

const std::string cta::NoMode::TYPE = "None";

cta::NoMode::NoMode(cta::ControllerApp* app) :
	cta::LEDMode(app, TYPE, app->getMainWindowPointer(), app->getWindowGUIPointer(), app->getArduinoConnector()) {

	noModePanel = tgui::Panel::create();
	noModePanel->setSize("70%", "100%");
	noModePanel->setPosition("30%", "0%");

	noModePanel->getSharedRenderer()->setBackgroundColor(sf::Color(35, 35, 35, 255));

	windowGui->add(noModePanel, "noModePanel");

	cta::HSPicker::Ptr hsPicker = cta::HSPicker::create();
	hsPicker->setSize("50%", "35%");
	hsPicker->setPosition("25%", "2.5%");
	noModePanel->add(hsPicker, "hsPicker1");

	cta::VPicker::Ptr vPicker = cta::VPicker::create();
	vPicker->setSize("50%", "7.5%");
	vPicker->setPosition("25%", "40%");
	vPicker->setHSPicker(hsPicker);
	noModePanel->add(vPicker, "vPicker1");

	tgui::EditBox::Ptr hexTextbox = tgui::EditBox::create();
	hexTextbox->setSize("11%", "4%");
	hexTextbox->setPosition("25%", "50%");
	hexTextbox->setTextSize(16);
	hexTextbox->setAlignment(tgui::EditBox::Alignment::Center);
	hexTextbox->setText("#000000");
	hexTextbox->setCaretPosition(0);
	hexTextbox->setMaximumCharacters(7);
	noModePanel->add(hexTextbox, "hexTextbox");

	tgui::EditBox::Ptr rTextbox = tgui::EditBox::create();
	rTextbox->setSize("11%", "4%");
	rTextbox->setPosition("38%", "50%");
	rTextbox->setTextSize(16);
	rTextbox->setAlignment(tgui::EditBox::Alignment::Center);
	rTextbox->setText("0");
	rTextbox->setCaretPosition(0);
	rTextbox->setMaximumCharacters(3);
	noModePanel->add(rTextbox, "rTextbox");

	tgui::EditBox::Ptr gTextbox = tgui::EditBox::create();
	gTextbox->setSize("11%", "4%");
	gTextbox->setPosition("51%", "50%");
	gTextbox->setTextSize(16);
	gTextbox->setAlignment(tgui::EditBox::Alignment::Center);
	gTextbox->setText("0");
	gTextbox->setCaretPosition(0);
	gTextbox->setMaximumCharacters(3);
	noModePanel->add(gTextbox, "gTextbox");

	tgui::EditBox::Ptr bTextbox = tgui::EditBox::create();
	bTextbox->setSize("11%", "4%");
	bTextbox->setPosition("64%", "50%");
	bTextbox->setTextSize(16);
	bTextbox->setAlignment(tgui::EditBox::Alignment::Center);
	bTextbox->setText("0");
	bTextbox->setCaretPosition(0);
	bTextbox->setMaximumCharacters(3);
	noModePanel->add(bTextbox, "bTextbox");

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