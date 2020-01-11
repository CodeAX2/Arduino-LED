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


	private:
		/*

		Mouse position, relative to the
		view, not the window.

		*/
		int mouseX, mouseY;
		int mouseXBeginClick, mouseYBeginClick;
		bool mouseIsInWindow;
		bool mouseIsDown;

		std::vector<bool> mouseIsHovering;

		sf::Texture* colorPaletteTexture;

		// Relative to the color palette
		int selectedColorX = 0, selectedColorY = 0;

		// Relative to the darkness gradient
		int selectedColorDarknessX = 0;

		// Selected color (no darkness)
		sf::Color selectedColor;

		// Selected color (with darkness)
		sf::Color selectedColorDarkness;



	};
}

