#include "VPicker.h"
#include <iostream>

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

void cta::VPicker::mouseMoved(tgui::Vector2f pos) {

	tgui::Widget::mouseMoved(pos);
	tgui::Vector2f relativePos = pos - getPosition();

	if (relativePos.x < 0) relativePos.x = 0;
	if (relativePos.x >= getSize().x - 10) relativePos.x = getSize().x - 11;

	if (m_mouseDown) {

		selectedColorX = relativePos.x;
		updateFinalColor();

	}

}

void cta::VPicker::updateFinalColor() {

	sf::Color newSelectedColor = hsPicker->getSelectedColor();
	newSelectedColor.r *= selectedColorX / getSize().x;
	newSelectedColor.g *= selectedColorX / getSize().x;
	newSelectedColor.b *= selectedColorX / getSize().x;

	finalColor = newSelectedColor;
}

void cta::VPicker::setHSPicker(cta::HSPicker::Ptr hsPicker) {
	this->hsPicker = hsPicker;
	hsPicker->setVPicker(this);
}