#include "GradientMode.h"
#include <iostream>
#include "LEDModeHandler.h"
#include <TGUI/TGUI.hpp>
#include <TGUI/Widget.hpp>
#include "ColorPickerModule.h"
#include <sstream>
#include <iomanip>

const std::string cta::GradientMode::TYPE = "Gradient";

cta::GradientMode::GradientMode(cta::ControllerApp* app) :
	cta::LEDMode(app, TYPE, app->getMainWindowPointer(), app->getWindowGUIPointer(), app->getArduinoConnector()) {

	gradientPanel = tgui::Panel::create();
	gradientPanel->setSize("70%", "100%");
	gradientPanel->setPosition("30%", "0%");
	gradientPanel->getSharedRenderer()->setBackgroundColor(sf::Color(35, 35, 35, 255));
	windowGui->add(gradientPanel, "gradientPanel");

	colorListPanel = tgui::Panel::create();
	colorListPanel->setSize("75%", "30%");
	colorListPanel->setPosition("50% - width / 2", "20");
	colorListPanel->getRenderer()->setBackgroundColor(sf::Color(10, 10, 10, 255));
	gradientPanel->add(colorListPanel, "colorListPanel");

	addColorButton = tgui::Button::create();
	addColorButton->setSize("30", "30");
	addColorButton->setPosition("colorListPanel.right - width", "colorListPanel.bottom");
	addColorButton->setText("+");
	addColorButton->setTextSize(24);
	addColorButton->connect("pressed", [&]() {
		addColorToList();
		}
	);
	gradientPanel->add(addColorButton, "addColorButton");

	removeColorButton = tgui::Button::create();
	removeColorButton->setSize("30", "30");
	removeColorButton->setPosition("addColorButton.left - width", "colorListPanel.bottom");
	removeColorButton->setText("-");
	removeColorButton->setTextSize(24);
	removeColorButton->connect("pressed", [&]() {
		removeColorFromList();
		}
	);
	gradientPanel->add(removeColorButton, "removeColorButton");

	confirmGradientButton = tgui::Button::create();
	confirmGradientButton->setSize("180", "30");
	confirmGradientButton->setPosition("colorListPanel.left", "colorListPanel.bottom");
	confirmGradientButton->setText("Confirm");
	confirmGradientButton->setTextSize(22);
	confirmGradientButton->connect("pressed", [&]() {
		confirmData();
		}
	);
	gradientPanel->add(confirmGradientButton, "confirmGradientButton");


	colorPicker = new cta::ColorPickerModule(gradientPanel, "100%", "100%", "-20%", "addColorButton.bottom + 15");
	colorPicker->hide();

	fadeLabel = tgui::Label::create();
	fadeLabel->setSize("15%", "5%");
	fadeLabel->setVerticalAlignment(tgui::Label::VerticalAlignment::Center);
	fadeLabel->setHorizontalAlignment(tgui::Label::HorizontalAlignment::Center);
	fadeLabel->setTextSize(24);
	fadeLabel->setText("Fade Time");
	fadeLabel->setPosition("55% + 22.5% - width / 2", "addColorButton.bottom + (100% - addColorButton.bottom) / 2 - height - 5%");
	gradientPanel->add(fadeLabel, "fadeLabel");

	fadeEditBox = tgui::EditBox::create();
	fadeEditBox->setSize("15%", "5%");
	fadeEditBox->setPosition("55% + 22.5% - width / 2", "fadeLabel.bottom");
	fadeEditBox->setTextSize(16);
	fadeEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	fadeEditBox->setText("1000");
	fadeEditBox->setCaretPosition(0);
	fadeEditBox->setMaximumCharacters(5);
	fadeEditBox->connect("TextChanged", &cta::GradientMode::textChanged, this);
	gradientPanel->add(fadeEditBox, "fadeEditBox");

	fadeLabel->setVisible(false);
	fadeEditBox->setVisible(false);

	this->deActivate();

}

void cta::GradientMode::draw(int dt) {


}

void cta::GradientMode::tick(int dt) {

	if (selectedColorIndex != -1) {
		colors[selectedColorIndex] = colorPicker->getColor();
		std::stringstream containerName;
		containerName << "colorContainer" << selectedColorIndex;
		tgui::Button::Ptr selectedColorContainer = colorListPanel->get<tgui::Button>(containerName.str());
		selectedColorContainer->getRenderer()->setBackgroundColor(colorPicker->getColor());
		selectedColorContainer->getRenderer()->setBackgroundColorHover(colorPicker->getColor() - sf::Color(55, 55, 55, 0));
		selectedColorContainer->getRenderer()->setBackgroundColorDown(colorPicker->getColor() - sf::Color(55, 55, 55, 0));
	}

}

void cta::GradientMode::handleEvent(sf::Event e) {


}

void cta::GradientMode::activate() {
	gradientPanel->setVisible(true);
}

void cta::GradientMode::deActivate() {
	gradientPanel->setVisible(false);
}

void cta::GradientMode::textChanged(tgui::Widget::Ptr widget, const std::string& signalName) {
	std::string widgetName = widget->getWidgetName().toAnsiString();

	// Update fade
	if (widgetName == "fadeEditBox" && selectedColorIndex != -1) {
		if (fadeEditBox->getText().toAnsiString() != "")
			fadeTimes[selectedColorIndex] = std::stoi(fadeEditBox->getText().toAnsiString());
	}

}

void cta::GradientMode::addColorToList() {

	if (colors.size() >= 8) return;

	tgui::Button::Ptr newColorContainer = tgui::Button::create();
	newColorContainer->setSize("100%", "12.5%");
	newColorContainer->getRenderer()->setBackgroundColor(sf::Color(255, 0, 0, 255));
	newColorContainer->getRenderer()->setBackgroundColorHover(sf::Color(200, 0, 0, 255));
	newColorContainer->getRenderer()->setBackgroundColorDown(sf::Color(200, 0, 0, 255));

	std::stringstream percentSS;
	float percentToMove = 12.5 * colors.size();
	percentSS << std::fixed << std::setprecision(1) << percentToMove << "%";

	newColorContainer->setPosition("0", percentSS.str());
	newColorContainer->connect("pressed", &cta::GradientMode::colorSelected, this);

	std::stringstream containerName;
	containerName << "colorContainer" << colors.size();

	colorListPanel->add(newColorContainer, containerName.str());
	colors.push_back(sf::Color(255, 0, 0, 255));
	fadeTimes.push_back(1000);

}

void cta::GradientMode::removeColorFromList() {

	if (colors.size() <= 0) return;

	if (selectedColorIndex == colors.size() - 1) {
		selectedColorIndex = -1;
		colorPicker->hide();
		fadeLabel->setVisible(false);
		fadeEditBox->setVisible(false);
	}

	std::stringstream containerName;
	containerName << "colorContainer" << colors.size() - 1;
	colorListPanel->remove(colorListPanel->get<tgui::Button>(containerName.str()));

	colors.erase(colors.begin() + colors.size() - 1);
	fadeTimes.erase(fadeTimes.begin() + fadeTimes.size() - 1);

}

void cta::GradientMode::colorSelected(tgui::Widget::Ptr widget, const std::string& signalName) {

	selectedColorIndex = std::stoi(widget->getWidgetName().toAnsiString().substr(14, 1));
	sf::Color selectedColor = colors[selectedColorIndex];

	colorPicker->show();
	colorPicker->setColor(selectedColor);

	fadeLabel->setVisible(true);
	fadeEditBox->setVisible(true);

	fadeEditBox->setText(std::to_string(fadeTimes[selectedColorIndex]));

}

void cta::GradientMode::confirmData() {

	selectedColorIndex = -1;
	colorPicker->hide();
	fadeLabel->setVisible(false);
	fadeEditBox->setVisible(false);

	if (arduinoConnector->isConnected() && !arduinoConnector->isRestarting()) {
		int amountToSend = 2 + 5 * colors.size();
		unsigned char* bytesToSend = new unsigned char[amountToSend];

		bytesToSend[0] = 2;
		bytesToSend[1] = colors.size();

		for (int i = 0; i < colors.size(); i++) {

			int arrayPos = 2 + i * 5;

			bytesToSend[arrayPos] = colors[i].r;
			bytesToSend[arrayPos + 1] = colors[i].g;
			bytesToSend[arrayPos + 2] = colors[i].b;

			bytesToSend[arrayPos + 3] = fadeTimes[i];
			bytesToSend[arrayPos + 4] = fadeTimes[i] >> 8;

		}

		arduinoConnector->sendDataSlow(bytesToSend, amountToSend, 0, 8);
	}


}