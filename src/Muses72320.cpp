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

#include "Muses72320.h"


static inline uint8_t volume_to_attenuation(int16_t volume)
{
	// volume to attenuation data conversion:
	// #=====================================#
	// |    0.0 dB | in: [  0] -> 0b00010000 |
	// | -111.5 dB | in: [223] -> 0b11101111 |
	// #=====================================#
	return static_cast<uint8_t>(min(-volume, 223) + 0x10);
}

static inline uint8_t volume_to_gain(int16_t gain)
{
	// volume to gain data conversion:
	// #===================================#
	// |     0 dB | in: [ 0] -> 0b00000000 |
	// | +31.5 dB | in: [63] -> 0b01111111 |
	// #===================================#
	return static_cast<uint8_t>(min(static_cast<int>(gain),63));
}

Muses72320::Muses72320(uint8_t address, uint8_t ss)  {
	_address = address;
	_ss = ss;
    _states = 0b00100000;
}

void Muses72320::begin()
{
  Serial.println(String(_ss));  
  ::pinMode(_ss, OUTPUT);
  ::digitalWrite(_ss, HIGH);
  // define SPI clockspeed, endian and mode
  _spiSettings = SPISettings(250000, MSBFIRST, SPI_MODE2);
  SPI.begin();
}

void Muses72320::setVolume(int16_t lch, int16_t rch)
{
	if (bitRead(_states, s_state_bit_attenuation)) {
		// interconnected left and right channels.
		transfer(s_control_attenuation_l, volume_to_attenuation(lch));
	} else {
		// independent left and right channels.
		transfer(s_control_attenuation_l, volume_to_attenuation(lch));
		transfer(s_control_attenuation_r, volume_to_attenuation(rch));
	}
}

void Muses72320::setGain(int16_t lch, int16_t rch)
{
	if (bitRead(_states, s_state_bit_gain)) {
		// interconnected left and right channels.
		transfer(s_control_gain_l, volume_to_gain(lch));
	} else {
		// independent left and right channels.
		transfer(s_control_gain_l, volume_to_gain(lch));
		transfer(s_control_gain_r, volume_to_gain(rch));
	}
}

void Muses72320::mute()
{
  if (bitRead(_states, s_state_bit_attenuation)) {
    transfer(s_control_attenuation_l, 0);
  } else {
    transfer(s_control_attenuation_l, 0);
    transfer(s_control_attenuation_r, 0);
  }
}

void Muses72320::setZeroCrossing(bool enabled)
{
	// 0 is enabled, 1 is disabled.
	bitWrite(_states, s_state_bit_zero_crossing, !enabled);
	transfer(s_control_states, _states);
}

void Muses72320::setAttenuationLink(bool enabled)
{
	// 1 is enabled, 0 is disabled.
	bitWrite(_states, s_state_bit_attenuation, enabled);
	transfer(s_control_states, _states);
}

void Muses72320::setGainLink(bool enabled)
{
	// 1 is enabled, 0 is disabled.
	bitWrite(_states, s_state_bit_gain, enabled);
	transfer(s_control_states, _states);
}

void Muses72320::transfer(uint8_t address, uint8_t data)
{
  SPI.beginTransaction(_spiSettings);
  ::digitalWrite(_ss, LOW);
  SPI.transfer(data);
  SPI.transfer(address | _address);
  ::digitalWrite(_ss, HIGH);
  SPI.endTransaction();
}