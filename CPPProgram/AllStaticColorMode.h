#pragma once
#include "LEDMode.h"
#include "ArduinoConnector.h"

namespace cta {
	class AllStaticColorMode : public LEDMode {

	public:
		static const std::string TYPE;

	public:


		/*

		Creates a new type of
		LEDMode, with the type
		set to "Static Color".

		*/
		AllStaticColorMode(cta::ControllerApp* app);

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


	private:
		/*

		Mouse position, relative to the
		view, not the window.

		*/
		int mouseX, mouseY;
		bool mouseIsInWindow;



	};
}

