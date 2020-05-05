#include "ColorPickerModule.h"
#include <iostream>
#include <iomanip>

cta::ColorPickerModule::ColorPickerModule(tgui::Container::Ptr parent, tgui::Layout width,
	tgui::Layout height, tgui::Layout posX, tgui::Layout posY) {

	this->parent = parent;

	containingPanel = tgui::Panel::create();
	containingPanel->setSize(width, height);
	containingPanel->setPosition(posX, posY);

	containingPanel->getSharedRenderer()->setBackgroundColor(sf::Color(35, 35, 35, 255));

	parent->add(containingPanel);

	hsPicker = cta::HSPicker::create();
	hsPicker->setSize("50%", "35%");
	hsPicker->setPosition("25%", "2.5%");
	containingPanel->add(hsPicker, "hsPicker");

	vPicker = cta::VPicker::create();
	vPicker->setSize("50%", "7.5%");
	vPicker->setPosition("25%", "40%");
	vPicker->setHSPicker(hsPicker);
	containingPanel->add(vPicker, "vPicker");

	hexEditBox = tgui::EditBox::create();
	hexEditBox->setSize("11%", "4%");
	hexEditBox->setPosition("25%", "50%");
	hexEditBox->setTextSize(16);
	hexEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	hexEditBox->setText("#ff0000");
	hexEditBox->setCaretPosition(0);
	hexEditBox->setMaximumCharacters(7);
	hexEditBox->connect("TextChanged", &cta::ColorPickerModule::textChanged, this);
	containingPanel->add(hexEditBox, "hexEditBox");

	redEditBox = tgui::EditBox::create();
	redEditBox->setSize("11%", "4%");
	redEditBox->setPosition("38%", "50%");
	redEditBox->setTextSize(16);
	redEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	redEditBox->setText("255");
	redEditBox->setCaretPosition(0);
	redEditBox->setMaximumCharacters(3);
	redEditBox->connect("TextChanged", &cta::ColorPickerModule::textChanged, this);
	containingPanel->add(redEditBox, "redEditBox");

	greenEditBox = tgui::EditBox::create();
	greenEditBox->setSize("11%", "4%");
	greenEditBox->setPosition("51%", "50%");
	greenEditBox->setTextSize(16);
	greenEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	greenEditBox->setText("0");
	greenEditBox->setCaretPosition(0);
	greenEditBox->setMaximumCharacters(3);
	greenEditBox->connect("TextChanged", &cta::ColorPickerModule::textChanged, this);
	containingPanel->add(greenEditBox, "greenEditBox");

	blueEditBox = tgui::EditBox::create();
	blueEditBox->setSize("11%", "4%");
	blueEditBox->setPosition("64%", "50%");
	blueEditBox->setTextSize(16);
	blueEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	blueEditBox->setText("0");
	blueEditBox->setCaretPosition(0);
	blueEditBox->setMaximumCharacters(3);
	blueEditBox->connect("TextChanged", &cta::ColorPickerModule::textChanged, this);
	containingPanel->add(blueEditBox, "blueEditBox");

}

sf::Color cta::ColorPickerModule::getColor() {
	return vPicker->getFinalColor();
}

void cta::ColorPickerModule::textChanged(tgui::Widget::Ptr widget, const std::string& signalName) {

	if (updatingFromSelf || vPicker->isUpdatingFromMouse() || hsPicker->isUpdatingFromMouse()) return;

	updatingFromSelf = true;

	std::string widgetName = widget->getWidgetName().toAnsiString();

	sf::Color newColor = getColor();

	// Update colors
	if (widgetName == "hexEditBox") {

		// Editing hex box

		std::string hexString = hexEditBox->getText().toAnsiString();
		if (hexString.size() != 0 && hexString.at(0) == '#') {
			hexString = hexString.substr(1, std::string::npos);
		}

		if (hexString != "") {

			int newColorAsInt = 0;
			try {
				newColorAsInt = std::stoi(hexString, 0, 16);
			} catch (std::invalid_argument e) {
				hexEditBox->setText("#000000");
			}

			newColorAsInt <<= 8;
			newColor = sf::Color(newColorAsInt);

			// Set the strings
			redEditBox->setText(std::to_string(newColor.r));
			greenEditBox->setText(std::to_string(newColor.g));
			blueEditBox->setText(std::to_string(newColor.b));

			newColor.a = 255;

			// Update HS and V pickers
			hsPicker->setSelectedColor(newColor);
			vPicker->setSelectedColor(newColor);

		}

		updatingFromSelf = false;

	} else {

		// Editing red, green, or blue boxes

		if (widgetName == "redEditBox") {

			if (redEditBox->getText().toAnsiString() != "") {
				int newRed = 0;
				try {
					newRed = std::stoi(redEditBox->getText().toAnsiString());
				} catch (std::invalid_argument e) {
					redEditBox->setText("0");
				}
				if (newRed > 255) {
					redEditBox->setText("255");
					newColor.r = 255;
				} else if (newRed < 0) {
					redEditBox->setText("0");
					newColor.r = 0;
				} else {
					newColor.r = newRed;
				}
			}

		} else if (widgetName == "greenEditBox") {

			if (greenEditBox->getText().toAnsiString() != "") {
				int newGreen = 0;
				try {
					newGreen = std::stoi(greenEditBox->getText().toAnsiString());
				} catch (std::invalid_argument e) {
					greenEditBox->setText("0");
				}
				if (newGreen > 255) {
					greenEditBox->setText("255");
					newColor.g = 255;
				} else if (newGreen < 0) {
					greenEditBox->setText("0");
					newColor.g = 0;
				} else {
					newColor.g = newGreen;
				}
			}

		} else if (widgetName == "blueEditBox") {

			if (blueEditBox->getText().toAnsiString() != "") {
				int newBlue = 0;
				try {
					newBlue = std::stoi(blueEditBox->getText().toAnsiString());
				} catch (std::invalid_argument e) {
					blueEditBox->setText("0");
				}
				if (newBlue > 255) {
					blueEditBox->setText("255");
					newColor.b = 255;
				} else if (newBlue < 0) {
					blueEditBox->setText("0");
					newColor.b = 0;
				} else {
					newColor.b = newBlue;
				}
			}
		}

		newColor.a = 255;

		// Set the hex text box

		std::stringstream ss;
		ss << "#" << std::setfill('0') << std::setw(6) << std::hex << (newColor.toInteger() >> 8);

		hexEditBox->setText(ss.str());


		hsPicker->setSelectedColor(newColor);
		vPicker->setSelectedColor(newColor);

		updatingFromSelf = false;

	}

}

void cta::ColorPickerModule::setColor(sf::Color color) {
	redEditBox->setText(std::to_string(color.r));
	greenEditBox->setText(std::to_string(color.g));
	blueEditBox->setText(std::to_string(color.b));
}

void cta::ColorPickerModule::hide() {
	containingPanel->setVisible(false);
}

void cta::ColorPickerModule::show() {
	containingPanel->setVisible(true);
}