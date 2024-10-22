/*
  The MIT License (MIT)

  Copyright (c) 2024 Felix Thommen

  Permission is hereby granted, free of charge, to any person obtaining a copy of
  this software and associated documentation files (the "Software"), to deal in
  the Software without restriction, including without limitation the rights to
  use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
  the Software, and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
  FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
  COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
  IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

  The library interface and documentation was taken from the existing library by
  Copyright (c) 2016 Christoffer Hjalmarsson see https://github.com/qhris/Muses72320/
*/
#ifndef INCLUDED_MUSES_72320
#define INCLUDED_MUSES_72320

#include <Arduino.h>
#include <SPI.h>


// control select addresses, chip address (low 4) ignored.
const uint8_t s_control_attenuation_l = 0b00000000;
const uint8_t s_control_attenuation_r = 0b00100000;
const uint8_t s_control_gain_l        = 0b00010000;
const uint8_t s_control_gain_r        = 0b00110000;
const uint8_t s_control_states        = 0b01000000;

// control state bits.
const uint8_t s_state_bit_zero_crossing = 5; // 0 = zero cross detection (default)
const uint8_t s_state_bit_gain          = 6; // 0 = independent L/R gain control (default)
const uint8_t s_state_bit_attenuation   = 7; // 0 = independent L/R volume control (default)


class Muses72320 {

public:

	// specify a connection over an address using three output pins.
	Muses72320(uint8_t _address, uint8_t _ss);

	// set the pins in their correct states.
	void begin();

	// set the volume using the following formula:
	//   (-0.5 * volume) db
	// audio level goes from [-111.5, 0.0] dB
	// input goes from -223 to 0.
	void setVolume(int16_t left, int16_t right);
	inline void setVolume(int16_t volume) { setVolume(volume, volume); }

	// gain is constrained to [0, 31.5].
	// input goes from 0 to 63.
	void setGain(int16_t left, int16_t right);
	inline void setGain(int16_t volume) { setGain(volume, volume); }

    void mute();

	// enable or disable zero crossing.
	// enabling zero crossing only works if the zero crossing terminal is set low.
	void setZeroCrossing(bool enabled);
	void setAttenuationLink(bool enabled);
	void setGainLink(bool enabled);

private:

    void transfer(uint8_t address, uint8_t data);

	// for multiple chips on the same bus line.
	uint8_t _address;
    uint8_t _ss;

	// muses state bits:
	//	 7:     link l/r attenuation
	//   6:     link l/r gain
	//   5:     disable zero crossing
	//   [4-0]: not used
	uint8_t _states;
    SPISettings _spiSettings;
};

#endif // INCLUDED_MUSES_72320