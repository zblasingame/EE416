/*
 * Program to compare ideal and measured distributions
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#include <stdint.h>
#include <stdio.h>

/* For some reason SHIFT_TERM being 80 works */
/* better than 64? */
#define SHIFT_TERM		64
/* I shouldn't need this at all... */
#define CENTERING_BIAS  10
#define CUTOFF_WINDOW	12

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
	static const uint16_t ideal_dist[128] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0
	};

	uint16_t n, m;
	uint16_t start, sum;
	uint16_t idx;
	uint16_t max = 0;
	int16_t argmax = 0;

	for (n=CUTOFF_WINDOW; n < 128-CUTOFF_WINDOW; ++n) {
		sum = 0;
		start = (SHIFT_TERM-n > 0) ? SHIFT_TERM-n : 0;

		for (m=start; m < 128-n; ++m) {
			/* Shift term centers on 64, rather than 0. */
			/* Fixes weird negative indicies for argmax */
			idx = m + n - SHIFT_TERM;

			sum += *(x+m) * *(ideal_dist + idx);
		}

		if (sum > max) {
			max = sum;
			argmax = n;
		}
	}

	return -1 * (argmax - SHIFT_TERM + CENTERING_BIAS);
}
