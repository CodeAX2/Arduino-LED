#include "ArduinoConnector.h"
#include <sstream>
#include <SFML/System.hpp>

cta::ArduinoConnector::ArduinoConnector(std::string comPort) {
	this->comPort = comPort;
}

bool cta::ArduinoConnector::connect() {

	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts = { 0 };

	if (!openSerialPort(dcbSerialParams, timeouts)) {
		connected = false;
		return false;
	}

	connected = true;
	return true;
}



bool cta::ArduinoConnector::openSerialPort(DCB& dcbSerialParams, COMMTIMEOUTS& timeouts) {

	std::stringstream ss;
	ss << "\\\\.\\";
	ss << comPort;

	std::string sTemp = ss.str();
	std::wstring wsTemp = std::wstring(sTemp.begin(), sTemp.end());
	LPCWSTR comData = wsTemp.c_str();

	hSerial = CreateFile(
		comData, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL
	);

	if (hSerial == INVALID_HANDLE_VALUE) {
		return false;
	}

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == 0) {
		CloseHandle(hSerial);
		return false;
	}

	dcbSerialParams.BaudRate = CBR_9600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;
	if (SetCommState(hSerial, &dcbSerialParams) == 0) {
		CloseHandle(hSerial);
		return false;
	}

	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutConstant = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	if (SetCommTimeouts(hSerial, &timeouts) == 0) {
		CloseHandle(hSerial);
		return false;
	}

	return true;
}

void cta::ArduinoConnector::disconnect() {
	CloseHandle(hSerial);
}

bool cta::ArduinoConnector::isConnected() {
	return connected;
}

int cta::ArduinoConnector::sendData(unsigned char* bytes, int amountToSend) {

	DWORD amountSent;

	if (!WriteFile(hSerial, bytes, amountToSend, &amountSent, NULL)) {
		CloseHandle(hSerial);
		return -1;
	}
	return (int)amountSent;

}

int cta::ArduinoConnector::sendDataSlow(unsigned char* toSendStart, int amountToSend, int msToSend, int amountPerPackage) {
	int amountSent = 0;
	for (int i = 0; i < amountToSend / amountPerPackage + 1; i++) {
		unsigned char* currentByte = new unsigned char[amountPerPackage];
		for (int j = 0; j < amountPerPackage && i * amountPerPackage + j < amountToSend; j++) {
			currentByte[j] = *(toSendStart + i * amountPerPackage + j);
		}

		int curAmnt = amountPerPackage;
		if (i == amountToSend / amountPerPackage) {
			curAmnt = amountToSend % amountPerPackage;
		}

		int currentAmountSent = sendData(currentByte, curAmnt);

		if (currentAmountSent == -1) return -1;

		amountSent += currentAmountSent;
		delete[] currentByte;
		if (msToSend != 0)
			sf::sleep(sf::milliseconds(msToSend / (amountToSend / amountPerPackage + 1)));
	}

	return amountToSend;
}

void cta::ArduinoConnector::setCOMPort(std::string comPort) {
	this->comPort = comPort;
}