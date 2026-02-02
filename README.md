# Arduino LED Controller

An LED control system using Arduino and GUI software run on a PC. The system enables real time control of addressable LED strips via serial USB communication, supporting multiple effect modes.

## Features

### LED Control Modes
- `SETUP_MODE` - Configure LED count and RGB channel mapping
- `ALL_STATIC_COLOR_MODE` - Set all LEDs to a single color
- `INDIVIDUAL_STATIC_COLOR_MODE` - Set specific LEDs to different colors
- `GRADIENT_COLOR_MODE` - Create gradient-based transitions with configurable fade times (maximum of 8 colors)
- `TWO_COLOR_WAVE_MODE` - Animated waves alternating between two colors
- `RAINBOW_WAVE_MODE` - Full hue spectrum wave around the strip
- `AUDIO_MODE` - Visualize audio input with color mapping and peak detection
- `STARTUP_DATA_SET_MODE` - Save default LED configurations to EEPROM for startup

### Hardware Features
- Arduino control with configuration via serial USB
- Support for addressable RGB LED strips
- EEPROM-based persistent storage for startup configurations

### Software Architecture
- OOP mode-based design with dedicated classes for each LED mode
- Efficient serial protocol with custom binary commands

## Tech Stack

### Hardware
- Arduino (any compatible with USB serial support)
- Addressable RGB LED strips (WS2812)

### Software
- Arduino Sketch: Arduino C++ with PololuLedStrip library
- PC: C++ with SFML and TGUI

## Getting Started

### Arduino Setup

1. Open the Arduino IDE or compatible environment
2. Open `ArduinoProgram/ArduinoProgram.ino`
3. Select your Arduino board and COM port
4. Upload the sketch to your Arduino
5. The Arduino will start in `NO_MODE` and await commands via serial

### PC Application Setup

1. Navigate to `CPPProgram/`
2. Build the application using Visual Studio's MSVC Compiler:
   ```bash
   msbuild CPPProgram.vcxproj /p:Configuration=Release
   ```
3. Connect the Arduino via USB
4. Run the compiled application to begin sending commands

### Hardware Wiring

- Connect addressable LED strip data line to appropriate Arduino GPIO pin (default is pin 12)
- Connect LED power and ground
- Connect Arduino to PC via USB

## Communication Protocol

### Startup Behavior

The Arduino begins in `NO_MODE`. On startup, it checks EEPROM:
- At index `0`, there is the number of bytes, `n`, of startup instructions stored (0-64)
  - If `n = 0` or `n > 64`: No startup instructions, use default color
  - If `0 < n <= 64`: Load and execute `n` bytes of commands from EEPROM

### Command Structure

Everything is assumed to be little-endian.

#### `NO_MODE`
Send a single byte to select a mode:

| Command | Mode |
|---------|------|
| `0` | `SETUP_MODE` |
| `1` | `ALL_STATIC_COLOR_MODE` |
| `2` | `GRADIENT_COLOR_MODE` |
| `3` | `INDIVIDUAL_STATIC_COLOR_MODE` |
| `4` | `AUDIO_MODE` |
| `5` | `STARTUP_DATA_SET_MODE` |
| `6` | `TWO_COLOR_WAVE_MODE` |
| `7` | `RAINBOW_WAVE_COLOR_MODE` |

#### `SETUP_MODE`
Configure LED strip parameters:
- Bytes `0-1`: Total number of LEDs
- Byte `2`: Red channel
- Byte `3`: Green channel
- Byte `4`: Blue channel

After processing, mode returns to `NO_MODE`.

#### `ALL_STATIC_COLOR_MODE`
Set all LEDs to a single color:
- Byte `0`: Red value
- Byte `1`: Green value
- Byte `2`: Blue value

After processing, mode returns to `NO_MODE`.

#### `GRADIENT_COLOR_MODE`
Gradient-based color transitions between multiple colors:
- Byte `0`: Number of colors (max of 8)
- For each color (`n` segments of 5 bytes):
  - Byte `0`: Red value
  - Byte `1`: Green value
  - Byte `2`: Blue value
  - Bytes `3-4`: Fade time to next color in MS

Colors fade in sequence with specified fade times and loop.

After processing, mode returns to `NO_MODE`.

#### `INDIVIDUAL_STATIC_COLOR_MODE`
Set specific LEDs to individual colors. All other LEDs set to black:
- Bytes `0-1`: Number of LEDs to set
- For each LED (`n` segments of 5 bytes):
  - Bytes `0-1`: LED index/ID
  - Byte `2`: Red value
  - Byte `3`: Green value
  - Byte `4`: Blue value

After processing, mode returns to `NO_MODE`.

#### `AUDIO_MODE`
Visualize audio input with peak detection and color mapping:
- Byte `0`: Number of colors (max of 8)
- Next `3n` bytes: RGB values for each color
- Next sets of `2` bytes: audio level values (0-1000) until exit command received
- Exit: Send 65535 (0xFFFF) to return to `NO_MODE`

Audio levels are input into a peak detection algorithm which uses the defined colors to control the LEDs.

#### `STARTUP_DATA_SET_MODE`
Save default LED configuration to EEPROM for automatic loading on startup:
- Byte `0`: Number of bytes to save (0-64)
- Next `n` bytes: Command sequence to execute on startup

Saved commands are executed automatically whenever the Arduino restarts.

After processing, mode returns to `NO_MODE`.

#### `TWO_COLOR_WAVE_MODE`
Create waves with two alternating colors that circle around the LED strip:
- Bytes `0-2`: First wave color (RGB)
- Bytes `3-5`: Second wave color (RGB)
- Bytes `6-7`: Delay between wave updates in MS
- Bytes `8-9`: Wave length in number of LEDs

After processing, mode returns to `NO_MODE`.

#### `RAINBOW_WAVE_COLOR_MODE`
Display a hue spectrum wave around the LED strip:
- Bytes `0-1`: Transition delay in MS per LED
- Bytes `2-3`: Number of LEDs in a single wave
- Byte `4`: Saturation level (0-255)
- Byte `5`: Brightness level (0-255)

After processing, mode returns to `NO_MODE`.