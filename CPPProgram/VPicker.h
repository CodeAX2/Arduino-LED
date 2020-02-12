#pragma once
#include <TGUI/TGUI.hpp>
#include "HSPicker.h"

namespace cta {
	class VPicker : public tgui::ClickableWidget {

	public:
		/*

		Required members and methods in order to
		be a clickable widget.

		*/
		typedef std::shared_ptr<VPicker> Ptr;
		typedef std::shared_ptr<const VPicker> ConstPtr;
		static VPicker::Ptr create();
		static VPicker::Ptr copy(VPicker::ConstPtr widget);

		/*

		Creates a value picker
		using the TGUI library.

		*/
		VPicker();

		/*

		Draw the value picker
		to the render target.

		*/
		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		void mouseMoved(tgui::Vector2f pos) override;

		void setHSPicker(cta::HSPicker::Ptr hsPicker);

		void updateFinalColor();

	private:



		/*

		Texture created dynamically
		constructed. Represents a value
		gradient for selecting colors.

		*/
		sf::Texture* vPickerTexture;

		/*

		The relative x coordinate of
		the currently selected color in the
		gradient.

		*/
		int selectedColorX = 0;

		/*
		
		The hue/saturation picker that
		is related to this value picker.

		*/
		cta::HSPicker::Ptr hsPicker;

		/*

		The color that is currently selected.
		Includes hue, saturation, and value,
		so it is the final color value.

		*/
		sf::Color finalColor = sf::Color::Black;



	};

}