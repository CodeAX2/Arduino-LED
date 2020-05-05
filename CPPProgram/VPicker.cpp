#include "VPicker.h"
#include <iostream>
#include <iomanip>
#include "Utils.h"

cta::VPicker::VPicker() {

	m_draggableWidget = true;
	m_type = "VPicker";

}

cta::VPicker::Ptr cta::VPicker::create() {
	return std::make_shared<cta::VPicker>();
}

cta::VPicker::Ptr cta::VPicker::copy(VPicker::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<cta::VPicker>(widget->clone());
	} else {
		return nullptr;
	}
}

void cta::VPicker::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	if (hsPicker == nullptr) return;
	states.transform.translate(getPosition());

	sf::RectangleShape darknessGradient(getSize() - sf::Vector2f(10, 10));
	darknessGradient.setFillColor(sf::Color::White);
	darknessGradient.setOutlineColor(sf::Color(16, 19, 20));
	darknessGradient.setOutlineThickness(5);
	darknessGradient.setPosition(5, 5);

	sf::Color selectedColor = hsPicker->getSelectedColor();
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
	target.draw(darknessGradient, states);

	// Draw the darkness selector
	sf::RectangleShape darknessSelector(sf::Vector2f(10, darknessGradient.getSize().y + 10));
	darknessSelector.setOutlineColor(sf::Color::White);
	darknessSelector.setOutlineThickness(2);
	darknessSelector.setFillColor(finalColor);
	darknessSelector.setPosition(darknessGradient.getPosition() + sf::Vector2f(selectedColorX - darknessSelector.getSize().x / 2, -5));
	target.draw(darknessSelector, states);


}

void cta::VPicker::update(sf::Time elapsedTime) {

	if (firstUpdate) {
		selectedColorX = getSize().x - 11;
		firstUpdate = false;
	}


}

void cta::VPicker::mouseMoved(tgui::Vector2f pos) {

	updatingFromMouse = true;

	tgui::Widget::mouseMoved(pos);
	tgui::Vector2f relativePos = pos - getPosition();

	if (relativePos.x < 0) relativePos.x = 0;
	if (relativePos.x >= getSize().x - 10) relativePos.x = getSize().x - 11;

	if (m_mouseDown) {

		selectedColorX = relativePos.x;
		updateFinalColor();

	}

	updatingFromMouse = false;

}

void cta::VPicker::updateFinalColor() {

	sf::Color newSelectedColor = hsPicker->getSelectedColor();
	newSelectedColor.r *= selectedColorX / (getSize().x - 11);
	newSelectedColor.g *= selectedColorX / (getSize().x - 11);
	newSelectedColor.b *= selectedColorX / (getSize().x - 11);

	finalColor = newSelectedColor;

	// Update the text boxes
	tgui::EditBox::Ptr hexTextBox = getParent()->get<tgui::EditBox>("hexEditBox");
	std::stringstream hexSS;
	hexSS << "#";
	hexSS << std::hex << std::setfill('0') << std::setw(2);
	hexSS << (int)finalColor.r << std::setfill('0') << std::setw(2);
	hexSS << (int)finalColor.g << std::setfill('0') << std::setw(2);
	hexSS << (int)finalColor.b;

	hexTextBox->setText(hexSS.str());
	hexTextBox->setCaretPosition(0);

	tgui::EditBox::Ptr rTextBox = getParent()->get<tgui::EditBox>("redEditBox");
	std::stringstream rSS;
	rSS << (int)finalColor.r;
	rTextBox->setText(rSS.str());
	rTextBox->setCaretPosition(0);

	tgui::EditBox::Ptr gTextBox = getParent()->get<tgui::EditBox>("greenEditBox");
	std::stringstream gSS;
	gSS << (int)finalColor.g;
	gTextBox->setText(gSS.str());
	gTextBox->setCaretPosition(0);

	tgui::EditBox::Ptr bTextBox = getParent()->get<tgui::EditBox>("blueEditBox");
	std::stringstream bSS;
	bSS << (int)finalColor.b;
	bTextBox->setText(bSS.str());
	bTextBox->setCaretPosition(0);

}

void cta::VPicker::setHSPicker(cta::HSPicker::Ptr hsPicker) {
	this->hsPicker = hsPicker;
	hsPicker->setVPicker(this);
}

sf::Color cta::VPicker::getFinalColor() {
	return finalColor;
}

void cta::VPicker::setSelectedColor(sf::Color color) {
	float hsv[] = { 0,0,0 };
	cta::Utils::colorToHSV(color, hsv);

	selectedColorX = (getSize().x - 10) * hsv[2] / 100.0;

	finalColor = color;

}

bool cta::VPicker::isUpdatingFromMouse() {
	return updatingFromMouse;
}