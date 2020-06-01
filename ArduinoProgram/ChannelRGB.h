#pragma once
#include <PololuLedStrip.h>

// Allows for setting a color with certain channels for each color
// By default, red is channel 0, green is 1, and blue is 2, which
// results in a RGB format
// For example, a GRB format has red channel set to 1, green set to 0, and blue set to 2

class ChannelRGB {
public:

    ChannelRGB();

    ChannelRGB(int red, int green, int blue);

    ChannelRGB(int red, int green, int blue, int redChannel, int greenChannel, int blueChannel);

    ChannelRGB(const ChannelRGB& color);

    void setColor(int red, int green, int blue);

    void setRed(int red);

    void setGreen(int green);

    void setBlue(int blue);

    int getRed();

    int getGreen();

    int getBlue();

    void setColorChannels(int redChannel, int greenChannel, int blueChannel);

    void setRedChannel(int channel);

    void setGreenChannel(int channel);

    void setBlueChannel(int channel);

    int getRedChannel();

    int getGreenChannel();

    int getBlueChannel();

    rgb_color getColorForLED();

    bool equalsColor(ChannelRGB other);


private:
    int red, green, blue;
    int redChannel, greenChannel, blueChannel;



};
