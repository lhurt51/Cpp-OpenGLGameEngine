#include "CMWC3096.h"

// Make 32 bit random number (some systems use 16 bit RAND_MAX [Visual C 2012 uses 15 bit])
uint32_t rand32(void)
{
	uint32_t result = rand();
	return (result << 16) ^ rand();
}

// Init the state with a seed 
void InitCMWC(struct CMWC_State* state, unsigned int seed)
{
	srand(seed);
	for (int i = 0; i < CMWC_CYCLE; i++)
		state->Q[i] = rand32();
	do
		state->c = rand32();
	while (state->c >= CMWC_C_MAX);
	state->i = CMWC_CYCLE - 1;
}

// CMWC Engine
uint32_t randCMWC(struct CMWC_State* state)
{
	uint64_t const a = 18782; // Marsaglia recommended
	uint64_t const m = 0xfffffffe; // Marsaglia recommended
	uint64_t t;
	uint64_t x;

	state->i = (state->i + 1) & (CMWC_CYCLE - 1);
	t = a * state->Q[state->i] + state->c;
	// Let c = t / 0xfffffff, x = t mod 0xffffffff
	state->c = t >> 32;
	x = t + state->c;
	if (x < state->c)
	{
		x++;
		state->c++;
	}
	return state->Q[state->i] = m - x;
}