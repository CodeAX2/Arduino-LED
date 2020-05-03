#pragma once
#include "LEDMode.h"

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

		int offsetChangeDelay = 75;
		tgui::EditBox::Ptr delayEditBox;

		int waveLength = 2;
		tgui::EditBox::Ptr waveLengthEditBox;

	protected:
		void textChanged(tgui::Widget::Ptr widget, const std::string& signalName);


	};

}
