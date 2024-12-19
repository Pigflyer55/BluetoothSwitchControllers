#pragma once

#include <iostream>
#include <cmath>

struct rumble {
	uint16_t hf;	// high frequency
	uint16_t hfAmp; // high frequency amplitude
	uint8_t lf;		// low frequency
	uint8_t lfAmp;	// low frequency amplitude

} ;

rumble joyRumbleFreqAmp(float freq, float amp);