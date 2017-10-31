/*
 * Program to compare ideal and measured distributions
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#include <stdint.h>
#include <stdio.h>

#define CNTR			64
#define CUTOFF_WINDOW	12
#define MAX_VALUE		1
#define DEFAULT_RTN	    0xF000

/*
 * Measure argmax cross correlation
 *
 * Args:
 *		data (uint16_t*): Black and white pixel array.
 *			Note that the BLACK values have to be HIGH,
 *			similiarly WHITE values are LOW.
 *
 * Returns:
 *		uint16_t: Correction Term, in [-64, 63]
 */
int16_t metric(uint16_t* data) {
	/* Here be vars */
	uint16_t i;
	uint32_t arg_ev = 0;
	uint32_t net_weight = 0;

	/* Arthmetic */
	for (i=CUTOFF_WINDOW; i<128-CUTOFF_WINDOW; ++i) {
		/* Inversion of data */
		data[i] = MAX_VALUE - data[i];

		arg_ev += i * data[i];
		net_weight += data[i];
	}

	if (!net_weight) {
		/* We dun messed up! */
		return DEFAULT_RTN;
	}

	return (int16_t)(arg_ev/net_weight) - CNTR;
}
