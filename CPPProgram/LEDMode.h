#pragma once
#include <SFML/Graphics.hpp>
#include "ArduinoConnector.h"
#include "ControllerApp.h"

namespace cta {
	/*

	Provides the base class for any other
	types of LEDModes, which are able to
	control the LED strips in differnt
	ways.

	*/
	class LEDMode {

	public:

		/*

		Create a new LEDMode with a given type

		*/
		LEDMode(ControllerApp* app, std::string type, sf::RenderWindow* window, ArduinoConnector* arduinoConnector);

		/*

		Draw the current frame to the screen.
		This does not get called if the window
		is not visible.

		*/
		virtual void draw(int dt);


		/*

		Perform logic to change LEDs. This
		is called every frame, even if the
		window is not visible.

		*/
		virtual void tick(int dt);

		/*

		Handle any events that happened in
		relation to the window.

		*/
		virtual void handleEvent(sf::Event e);


		/*

		Return's the objects type,
		based on what was input when
		it was created.

		*/
		std::string getType();

		/*
		
		Called when this mode becomes
		the currently active mode.
		
		*/
		virtual void activate();


		/*
		
		Called when this mode
		is no longer the currently
		active mode.
		
		*/
		virtual void deActivate();

	protected:
		std::string type;
		sf::RenderWindow* window;
		ArduinoConnector* arduinoConnector;
		ControllerApp* app;


	};
}
