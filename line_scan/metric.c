/*
 * Program to compare ideal and measured distributions
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#include <stdint.h>
#include <stdio.h>

#define SHIFT_TERM		64
#define CUTOFF_WINDOW	12
#define MAX_VALUE		1

/*
 * Measure argmax cross correlation
 *
 * Args:
 *		x (uint16_t*): Measured distribution.
 *
 * Returns:
 *		int16_t: Correction Term.
 */
int16_t metric(uint16_t* x) {
	/* This is our ideal distribution */
	static const uint16_t ideal_dist[128] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, MAX_VALUE, MAX_VALUE, MAX_VALUE,
		MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE,
		MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE,
		MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE,
		MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE,
		MAX_VALUE, MAX_VALUE, MAX_VALUE, MAX_VALUE, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	};

	/* Here be vars */
	uint16_t n;

	uint16_t* curr_real = x;
	const uint16_t* stop_address = ideal_dist + 128;
	const uint16_t* curr_ideal = ideal_dist;

	uint16_t start;
	uint16_t sum;
	uint16_t max = 0;
	uint16_t argmax = 0;


	/* code */

	/* Invert array so black pixels equals max value */
	for (curr_real=x; curr_real < stop_address; ++curr_real) {
		*curr_real = MAX_VALUE - *curr_real;
	}

	/* Perform cross correlation and argmax calculation with bounding */
	for (n=CUTOFF_WINDOW; n < 128-CUTOFF_WINDOW; ++n) {
		sum = 0;
		start = (SHIFT_TERM - n > 0) ? SHIFT_TERM - n : 0;
		curr_ideal = ideal_dist + start + n - SHIFT_TERM;
		curr_real = x + start;

		while (curr_ideal < stop_address) {
			sum += *curr_real++ * *curr_ideal++;
		}

		if (sum > max) {
			max = sum;
			argmax = n;
		}
	}

	return argmax;

}
