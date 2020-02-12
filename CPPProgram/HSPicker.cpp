#include "HSPicker.h"
#include <iostream>
#include "VPicker.h"

cta::HSPicker::HSPicker() {

	m_draggableWidget = true;
	m_type = "HSPicker";

	// Create the color palette texture
	sf::Image hsPickerImage;
	hsPickerImage.create(255 * 6, 255);
	for (int i = 0; i < 255 * 6; i++) {
		for (int j = 0; j < 255; j++) {

			sf::Color color;
			switch (i / 255) {
			case 0:
				color.r = 255;
				color.g = i;
				color.b = 0;
				break;
			case 1:
				color.r = 255 * 2 - i;
				color.g = 255;
				color.b = 0;
				break;
			case 2:
				color.r = 0;
				color.g = 255;
				color.b = i - 255 * 2;
				break;
			case 3:
				color.r = 0;
				color.g = 255 * 4 - i;
				color.b = 255;
				break;
			case 4:
				color.r = i - 255 * 4;
				color.g = 0;
				color.b = 255;
				break;
			case 5:
				color.r = 255;
				color.g = 0;
				color.b = 255 * 6 - i;
				break;
			}

			color.r = color.r + (255 - color.r) * j / 255.0;
			color.g = color.g + (255 - color.g) * j / 255.0;
			color.b = color.b + (255 - color.b) * j / 255.0;

			hsPickerImage.setPixel(i, j, color);
		}
	}

	hsPickerTexture = new sf::Texture();
	hsPickerTexture->loadFromImage(hsPickerImage);

}

cta::HSPicker::Ptr cta::HSPicker::create() {
	return std::make_shared<cta::HSPicker>();
}

cta::HSPicker::Ptr cta::HSPicker::copy(HSPicker::ConstPtr widget) {
	if (widget) {
		return std::static_pointer_cast<cta::HSPicker>(widget->clone());
	} else {
		return nullptr;
	}
}

void cta::HSPicker::draw(sf::RenderTarget& target, sf::RenderStates states) const {

	states.transform.translate(getPosition());

	// Draw the color palette
	sf::RectangleShape colorPalette(getSize() - sf::Vector2f(10, 10));
	colorPalette.setPosition(5, 5);
	colorPalette.setTexture(hsPickerTexture);
	colorPalette.setOutlineColor(sf::Color(16, 19, 20));
	colorPalette.setOutlineThickness(5);
	target.draw(colorPalette, states);

	// Draw the selected color
	sf::CircleShape colorSelector(10, 20);
	sf::Vector2f relativePos(selectedColorX - colorSelector.getRadius(), selectedColorY - colorSelector.getRadius());
	colorSelector.setPosition(colorPalette.getPosition() + relativePos);
	colorSelector.setFillColor(selectedColor);
	colorSelector.setOutlineColor(sf::Color::White);
	colorSelector.setOutlineThickness(2);
	target.draw(colorSelector, states);


}

void cta::HSPicker::mouseMoved(tgui::Vector2f pos) {

	tgui::Widget::mouseMoved(pos);
	tgui::Vector2f relativePos = pos - getPosition();

	if (relativePos.x < 0) relativePos.x = 0;
	if (relativePos.x >= getSize().x - 10) relativePos.x = getSize().x - 11;
	if (relativePos.y < 0) relativePos.y = 0;
	if (relativePos.y >= getSize().y - 10) relativePos.y = getSize().y - 11;

	if (m_mouseDown) {

		selectedColorX = relativePos.x;
		selectedColorY = relativePos.y;

		// Update the selected color
		sf::Color newSelectedColor;

		int i = selectedColorX * 255 * 6 / getSize().x;
		int j = selectedColorY * 255 / getSize().y;

		switch (i / 255) {
		case 0:
			newSelectedColor.r = 255;
			newSelectedColor.g = i;
			newSelectedColor.b = 0;
			break;
		case 1:
			newSelectedColor.r = 255 * 2 - i;
			newSelectedColor.g = 255;
			newSelectedColor.b = 0;
			break;
		case 2:
			newSelectedColor.r = 0;
			newSelectedColor.g = 255;
			newSelectedColor.b = i - 255 * 2;
			break;
		case 3:
			newSelectedColor.r = 0;
			newSelectedColor.g = 255 * 4 - i;
			newSelectedColor.b = 255;
			break;
		case 4:
			newSelectedColor.r = i - 255 * 4;
			newSelectedColor.g = 0;
			newSelectedColor.b = 255;
			break;
		case 5:
			newSelectedColor.r = 255;
			newSelectedColor.g = 0;
			newSelectedColor.b = 255 * 6 - i;
			break;
		}

		newSelectedColor.r = newSelectedColor.r + (255 - newSelectedColor.r) * j / 255.0;
		newSelectedColor.g = newSelectedColor.g + (255 - newSelectedColor.g) * j / 255.0;
		newSelectedColor.b = newSelectedColor.b + (255 - newSelectedColor.b) * j / 255.0;

		selectedColor = newSelectedColor;
		vPicker->updateFinalColor();

	}

}

sf::Color cta::HSPicker::getSelectedColor() {
	return selectedColor;
}

void cta::HSPicker::setVPicker(VPicker* vPicker) {
	this->vPicker = vPicker;
}