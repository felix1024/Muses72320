# Muses 72320

Arduino library for communicating with the Muses 72320 audio chip.
The data sheets can be found [here](http://www.njr.com/semicon/PDF/MUSES72320_E.pdf) (pdf).

## Download

Download the latest release over at the [Releases](https://github.com/felix1024/Muses72320/releases) page.

## Example

```c++
#include <Muses72320.h>

// The address wired into the muses chip (usually 0).
const uint_8 MUSES_ADDRESS = 0;
const int CHIP_SELECT = 10;

Muses72320 Muses(MUSES_ADDRESS, CHIP_SELECT);
int CurrentVolume = -20;

void setup()
{
  // Initialize muses (SPI, pin modes)...
  Muses.begin();

  // Muses initially starts in a muted state, set a volume to enable sound.
  Muses.setVolume(CurrentVolume);

  // These are the default states and could be removed...
  Muses.setZeroCrossing(true);     // Enable zero cross detection. (only if Z/C terminal pin 32 is low)
  Muses.setAttenuationLink(false); // L/R attentuation channels are independent controlled.
  Muses.setGainLink(false);        // L/R gain channels are independent controlled.
}

void loop()
{
  CurrentVolume -= 1;
  if (CurrentVolume < -223)
  {
    CurrentVolume = 0;
  }

  Muses.setVolume(CurrentVolume);
  // Equivalent to 'Muses.setVolume(CurrentVolume, CurrentVolume)' for L/R ch.

  delay(10);
}

```

## Problems

Please post any problems on the [Issues](https://github.com/felix1024/Muses72320/issues) page.

## License

Please read over the LICENSE file included in the project.
