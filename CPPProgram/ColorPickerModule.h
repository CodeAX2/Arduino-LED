#pragma once
#include <TGUI/TGUI.hpp>
#include "HSPicker.h"
#include "VPicker.h"
namespace cta {

	class ColorPickerModule {

	public:
		ColorPickerModule(tgui::Container::Ptr parent, tgui::Layout width = "100%",
			tgui::Layout height = "100%", tgui::Layout posX = 0, tgui::Layout posY = 0);

		/*
		
		Returns the currently selected color
		
		*/
		sf::Color getColor();

	private:

		void textChanged(tgui::Widget::Ptr widget, const std::string& signalName);


		tgui::Container::Ptr parent;
		tgui::Panel::Ptr containingPanel;
		cta::HSPicker::Ptr hsPicker;
		cta::VPicker::Ptr vPicker;
		tgui::EditBox::Ptr hexEditBox, redEditBox, greenEditBox, blueEditBox;

	};

}