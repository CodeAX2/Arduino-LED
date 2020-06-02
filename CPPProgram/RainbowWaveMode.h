#pragma once
#include "LEDMode.h"

namespace cta {

	class RainbowWaveMode : public LEDMode {

	public:
		static const std::string TYPE;

	public:


		/*

		Creates a new type of
		LEDMode, with the type
		set to "RainbowWave".

		*/
		RainbowWaveMode(cta::ControllerApp* app);

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
		tgui::Panel::Ptr rainbowWavePanel;

		int offset = 0;
		int timeSinceOffsetChange = 0;

		int offsetChangeDelay = 50;
		tgui::EditBox::Ptr delayEditBox;
		tgui::Label::Ptr delayLabel;

		int waveLength = 35;
		tgui::EditBox::Ptr waveLengthEditBox;
		tgui::Label::Ptr waveLabel;

		bool needsUpdating = false;

	protected:
		void textChanged(tgui::Widget::Ptr widget, const std::string& signalName);


	};

}
