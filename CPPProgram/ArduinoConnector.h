#pragma once
#include <iostream>
#include <Windows.h>
namespace cta {

	class ArduinoConnector {

	public:
		/*

		Creates a new instance to connect
		to an arduino.
		comPort is a string detailing what
		port to use to connect, e.g "COM5".
		numLEDs specifies the number of LEDs
		in the chain.
		redChannel specifies what channel the red
		color is on for the LED strip.
		greenChannel specifies what channel the green
		color is on for the LED strip.
		blueChannel specifies what channel the blue
		color is on for the LED strip.

		*/
		ArduinoConnector(std::string comPort, int numLEDs, int redChannel, int greenChannel, int blueChannel);

		/*

		Attempts to connect to the arduino.
		Returns true if connect successfully.
		Returns false if the connection failed.

		*/
		bool connect();

		/*

		Disconnects from the arduino.

		*/
		void disconnect();


		/*

		Returns true if the arduino is connected.
		Returns false if the arduino is not connected.

		*/
		bool isConnected();

		/*

		Returns true if the arduino is restarting.
		Returns false if the arduino is not restarting.

		*/
		bool isRestarting();

		/*

		Send data to the arduino.
		Returns the amount of data sent, or -1
		if unsuccessful in sending data.

		*/
		int sendData(unsigned char* bytes, int amountToSend);

		/*

		Certain arduino boards have a harder time recieving data
		when it is all sent at once.
		This method sends the data over a period of time,
		defined by msToSend.
		Returns the amount sent, or -1 if unsuccessful
		in sending data.

		*/
		int sendDataSlow(unsigned char* toSendStart, int amountToSend, int msToSend, int amountPerPackage);

		/*

		Change the current COM port
		used to connect to the arduino.
		This method does NOT attempt a connect.

		*/
		void setCOMPort(std::string comPort);


		/*

		Restarts the arduino, setting the LEDs
		back to their default state.
		Returns true if restart successfully.
		Returns false if the restart failed.
		Requires a disconnect before and a reconnect after

		*/
		bool restart();


		/*

		Returns the number of LEDs in the chain

		*/
		int getNumLEDs();

		/*

		Returns the red channel index

		*/
		int getRedChannel();

		/*

		Returns the green channel index

		*/
		int getGreenChannel();
		/*

		Returns the blue channel index

		*/
		int getBlueChannel();

	private:
		bool openSerialPort(DCB& dcbSerialParams, COMMTIMEOUTS& timeouts, bool restartArduino);

	private:
		HANDLE hSerial;
		std::string comPort;
		bool connected = false;
		bool restarting = false;
		int numLEDs;
		int redChannel, greenChannel, blueChannel;

	};

}

