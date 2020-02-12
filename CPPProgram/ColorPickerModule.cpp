#include "ColorPickerModule.h"
#include <iostream>

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
	hexEditBox->setText("#000000");
	hexEditBox->setCaretPosition(0);
	hexEditBox->setMaximumCharacters(7);
	hexEditBox->connect("TextChanged", &cta::ColorPickerModule::textChanged, this);
	containingPanel->add(hexEditBox, "hexEditBox");

	redEditBox = tgui::EditBox::create();
	redEditBox->setSize("11%", "4%");
	redEditBox->setPosition("38%", "50%");
	redEditBox->setTextSize(16);
	redEditBox->setAlignment(tgui::EditBox::Alignment::Center);
	redEditBox->setText("0");
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
	std::string widgetName = widget->getWidgetName().toAnsiString();

	// Update colors
	if (widgetName == "hexTextBox") {

	} else if (widgetName == "redTextBox") {

	} else if (widgetName == "greenTextBox") {

	} else if (widgetName == "blueTextBox") {

	}



	
}