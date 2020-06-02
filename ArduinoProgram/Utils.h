#pragma once
#include "ChannelRGB.h"

class Utils {

public:
	static void RGBtoHSV(float fR, float fG, float fB, float& fH, float& fS, float& fV);
	static void HSVtoRGB(float h, float s, float v, int& r, int& g, int& b);
	static ChannelRGB Utils::setBrightness(ChannelRGB baseColor, float brightness);

private:
	static float fract(float x);
	static float mix(float a, float b, float t);

};

