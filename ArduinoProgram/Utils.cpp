#include "Utils.h"
#include <Arduino.h>
#include "Handler.h"

void Utils::RGBtoHSV(float fR, float fG, float fB, float& fH, float& fS, float& fV) {
	float fCMax = max(max(fR, fG), fB);
	float fCMin = min(min(fR, fG), fB);
	float fDelta = fCMax - fCMin;

	if (fDelta > 0) {
		if (fCMax == fR) {
			fH = 60 * (fmod(((fG - fB) / fDelta), 6));
		} else if (fCMax == fG) {
			fH = 60 * (((fB - fR) / fDelta) + 2);
		} else if (fCMax == fB) {
			fH = 60 * (((fR - fG) / fDelta) + 4);
		}

		if (fCMax > 0) {
			fS = fDelta / fCMax;
		} else {
			fS = 0;
		}

		fV = fCMax;
	} else {
		fH = 0;
		fS = 0;
		fV = fCMax;
	}

	if (fH < 0) {
		fH = 360 + fH;
	}
}

void Utils::HSVtoRGB(float h, float s, float v, int& r, int& g, int& b) {
	r = v * mix(1.0, constrain(abs(fract(h + 1.0) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s) * 255;
	g = v * mix(1.0, constrain(abs(fract(h + 0.6666666) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s) * 255;
	b = v * mix(1.0, constrain(abs(fract(h + 0.3333333) * 6.0 - 3.0) - 1.0, 0.0, 1.0), s) * 255;
}

float Utils::fract(float x) {
	return x - int(x);
}
float Utils::mix(float a, float b, float t) {
	return a + (b - a) * t;
}

ChannelRGB Utils::setBrightness(ChannelRGB baseColor, float brightness) {

	float fHue, sat, val;
	float r = baseColor.getRed() / (float)255;
	float g = baseColor.getGreen() / (float)255;
	float b = baseColor.getBlue() / (float)255;

	RGBtoHSV(r, g, b, fHue, sat, val);

	val = brightness;
	int hue = fHue;

	hue %= 360;
	while (hue < 0)
		hue += 360;

	if (sat < 0.f)
		sat = 0.f;
	if (sat > 1.f)
		sat = 1.f;

	if (val < 0.f)
		val = 0.f;
	if (val > 1.f)
		val = 1.f;

	int h = hue / 60;
	float f = float(hue) / 60 - h;
	float p = val * (1.f - sat);
	float q = val * (1.f - sat * f);
	float t = val * (1.f - sat * (1 - f));

	switch (h) {
	default:
	case 0:
	case 6:
		int redChannel, greenChannel, blueChannel;
		Handler::getRGBChannels(&redChannel, &greenChannel, &blueChannel);
		return ChannelRGB(val * 255, t * 255, p * 255, redChannel, greenChannel,
			blueChannel);
	case 1:
		return ChannelRGB(q * 255, val * 255, p * 255, redChannel, greenChannel,
			blueChannel);
	case 2:
		return ChannelRGB(p * 255, val * 255, t * 255, redChannel, greenChannel,
			blueChannel);
	case 3:
		return ChannelRGB(p * 255, q * 255, val * 255, redChannel, greenChannel,
			blueChannel);
	case 4:
		return ChannelRGB(t * 255, p * 255, val * 255, redChannel, greenChannel,
			blueChannel);
	case 5:
		return ChannelRGB(val * 255, p * 255, q * 255, redChannel, greenChannel,
			blueChannel);
	}
}