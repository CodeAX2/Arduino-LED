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

		/*
		
		Update information about
		the widget

		*/
		void update(sf::Time elapsedTime) override;

		/*
		
		Override of the mouseMoved event function
		to allow for moving the slidebar to
		select the value.
		
		*/
		void mouseMoved(tgui::Vector2f pos) override;

		/*
		
		Sets this value slider's repsective
		hue/saturation picker. Implicitly
		sets the hue/saturation picker's
		respective value slider to this.
		
		*/
		void setHSPicker(cta::HSPicker::Ptr hsPicker);

		/*
		
		Update the final color and the
		text boxes based on a change in
		either the hue/saturation picker
		or in the value slider.
		
		*/
		void updateFinalColor();

		/*
		
		Returns the final color that
		is currently selected.
		
		*/
		sf::Color getFinalColor();

		/*
		
		Sets the currently selected color
		and changes the position of the 
		slider accordingly
		
		*/
		void setSelectedColor(sf::Color color);

		/*
		
		Returns true if the color is currently being
		updated from the mouse
		
		*/
		bool isUpdatingFromMouse();

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
		sf::Color finalColor = sf::Color::Red;

		bool updatingFromMouse = false;
		bool firstUpdate = true;

	};

}