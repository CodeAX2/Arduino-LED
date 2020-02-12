#pragma once
#include <TGUI/TGUI.hpp>

namespace cta {
	class VPicker;
	class HSPicker : public tgui::ClickableWidget {

	public:
		/*

		Required members and methods in order to
		be a clickable widget.

		*/
		typedef std::shared_ptr<HSPicker> Ptr;
		typedef std::shared_ptr<const HSPicker> ConstPtr;
		static HSPicker::Ptr create();
		static HSPicker::Ptr copy(HSPicker::ConstPtr widget);

		/*

		Creates a hue/saturation picker
		using the TGUI library.

		*/
		HSPicker();

		/*

		Draw the hue/saturation picker
		to the render target.

		*/
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		/*

		Called when the mouse moves.
		If the mouse is down, updates
		the circle to select the color.

		*/
		void mouseMoved(tgui::Vector2f pos) override;

		/*

		Gets the currently selected
		color. Only supports hue and
		saturation.

		*/
		sf::Color getSelectedColor();

		/*
		
		Set the corresponding value
		picker related to this 
		hue/saturation picker.
		
		*/
		void setVPicker(VPicker* vPicker);


	private:



		/*

		Texture created when the instance is
		constructed. Represents a hue/saturation
		gradient for selecting colors.

		*/
		sf::Texture* hsPickerTexture;

		/*

		The relative x and y coordinates of
		the currently selected color in the
		gradient.

		*/
		int selectedColorX = 0, selectedColorY = 0;

		/*

		The color that is currently selected.
		Does not include brightness, only hue
		and saturation.

		*/
		sf::Color selectedColor = sf::Color::Red;

		VPicker* vPicker;

	};

}