// 
// Frequency code from dekuNukem Nintendo Switch Reverse Engineering
//

#include "rumbleData.h"

rumble joyRumbleFreqAmp(float inFreq, float amp) {
	
	//Float frequency to hex conversion
	float freq = inFreq;
	//float amp = 0.22f;
	if (freq < 0.0f) {
		freq = 0.0f;
	}
	else if (freq > 1252.0f) {
		freq = 1252.0f;
	}
	uint8_t encoded_hex_freq = (uint8_t)round(log2((double)freq / 10.0) * 32.0);

	//Convert to Joy-Con HF range. Range in big-endian: 0x0004-0x01FC with +0x0004 steps.
	uint16_t hf = (encoded_hex_freq - 0x60) * 4;
	//Convert to Joy-Con LF range. Range: 0x01-0x7F.
	uint8_t lf = encoded_hex_freq - 0x40;

	// Float amplitude to hex conversion
	uint8_t encoded_hex_amp = 0;
	if (amp > 0.23f) {
		encoded_hex_amp = (uint8_t)round(log2f(amp * 8.7f) * 32.f);
	}
	else if (amp > 0.12f) {
		encoded_hex_amp = (uint8_t)round(log2f(amp * 17.f) * 16.f);
	}
	else {
		// TBD
	}
	uint16_t hf_amp = encoded_hex_amp * 2;    // encoded_hex_amp<<1;
	uint8_t lf_amp = encoded_hex_amp / 2 + 64;// (encoded_hex_amp>>1)+0x40;

	return {hf, hf_amp, lf, lf_amp};
}