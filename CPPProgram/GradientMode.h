#pragma once
#include "LEDMode.h"
#include "ColorPickerModule.h"

namespace cta {

	class GradientMode : public LEDMode {

	public:
		static const std::string TYPE;

	public:


		/*

		Creates a new type of
		LEDMode, with the type
		set to "Gradient".

		*/
		GradientMode(cta::ControllerApp* app);

		/*

		Override of the LEDMode's
		draw method.

		*/
		void draw(int dt) override;

		/*

		Override of the LEDMode's
		tick method.

		*/
		void tick(int dt) override;

		/*

		Override of the LEDMode's
		handleEvent method.

		*/
		void handleEvent(sf::Event e) override;

		/*

		Override of the LEDMode's
		activate method.

		*/
		void activate() override;


		/*

		Override of the LEDMode's
		deActivate method.

		*/
		void deActivate() override;

	protected:
		tgui::Panel::Ptr gradientPanel;
		tgui::Panel::Ptr colorListPanel;
		tgui::Button::Ptr addColorButton;
		tgui::Button::Ptr removeColorButton;
		tgui::Button::Ptr confirmGradientButton;

		cta::ColorPickerModule* colorPicker;
		tgui::EditBox::Ptr fadeEditBox;
		tgui::Label::Ptr fadeLabel;

		std::vector<sf::Color> colors;
		std::vector<int> fadeTimes;
		int selectedColorIndex = -1;


	protected:
		void textChanged(tgui::Widget::Ptr widget, const std::string& signalName);
		void addColorToList();
		void removeColorFromList();
		void colorSelected(tgui::Widget::Ptr widget, const std::string& signalName);
		void confirmData();


	};

}
