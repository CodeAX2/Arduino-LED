#pragma once
#include "LEDMode.h"
#include "ColorPickerModule.h"

namespace cta {

	class AllStaticMode :
		public LEDMode {

	public:
		static const std::string TYPE;

	public:


		/*

		Creates a new type of
		LEDMode, with the type
		set to "AllStatic".

		*/
		AllStaticMode(cta::ControllerApp* app);

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
		tgui::Panel::Ptr allStaticPanel;
		cta::ColorPickerModule* colorPicker;


	};

}

