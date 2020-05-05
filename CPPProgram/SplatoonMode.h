#pragma once
#include "LEDMode.h"
#include "ColorPickerModule.h"

namespace cta {

	class SplatoonMode : public LEDMode {

	public:
		static const std::string TYPE;

	public:


		/*

		Creates a new type of
		LEDMode, with the type
		set to "Splatoon".

		*/
		SplatoonMode(cta::ControllerApp* app);

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
		tgui::Panel::Ptr splatoonPanel;

		int offset = 0;
		int timeSinceOffsetChange = 0;

		int offsetChangeDelay = 50;
		tgui::EditBox::Ptr delayEditBox;
		tgui::Label::Ptr delayLabel;

		int waveLength = 7;
		tgui::EditBox::Ptr waveLengthEditBox;
		tgui::Label::Ptr waveLabel;

		sf::Color color1, color2;

		tgui::Button::Ptr color1Button;
		tgui::Button::Ptr color2Button;
		tgui::Panel::Ptr color1Display;
		tgui::Panel::Ptr color2Display;

		cta::ColorPickerModule* colorPicker;
		sf::Color* changingColor;
		tgui::Panel::Ptr changingDisplayColor;
		tgui::Button::Ptr confirmColorButton;

		bool sendData = true;

	protected:
		void textChanged(tgui::Widget::Ptr widget, const std::string& signalName);


	};

}
