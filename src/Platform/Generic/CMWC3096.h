#ifndef CMWC3096_H
	#define CMWC3096_H

	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <time.h>

	#define CMWC_CYCLE 4096 // Marsaglia recommended
	#define CMWC_C_MAX 809430660 // Marsaglia recommended

	struct CMVC_State
	{
		uint32_t Q[CMWC_CYCLE];
		uint32_t c;
		unsigned i;
	};

	void InitCMWC(struct CMWC_State* state, unsigned int seed);
	uint32_t randCMWC(struct CMWC_State* state);

#endif