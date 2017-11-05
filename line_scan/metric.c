/*
 * Program to compare ideal and measured distributions
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#include <stdio.h>
#include "metric.h"

/*
 * Mass function evaluation to find centre of signal.
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


/*
 * Mass function evaluation to find centre of signal,
 * with LPF
 *
 * Args:
 *		data (uint16_t*): Black and white pixel array.
 *			Note that the BLACK values have to be HIGH,
 *			similiarly WHITE values are LOW.
 *
 * Returns:
 *		uint16_t: Correction Term, in [-64, 63]
 */
int16_t metric_lpf(uint16_t* data) {
	/* Here be vars */
	uint16_t i, n;
	uint16_t lpf_order;
	uint16_t lpf_data[128];
	uint32_t arg_ev = 0;
	uint32_t net_weight = 0;
	uint16_t thresh = 0;

	/* Low pass filter */
	for (n=CUTOFF_WINDOW; n<128-CUTOFF_WINDOW; ++n) {
		lpf_data[n] = 0;
		lpf_order = n - (LPF_ORDER+1) > 0 ? LPF_ORDER + 1 : n;

		for (i=0; i<lpf_order; ++i) {
			lpf_data[n] += data[n-i];
		}
		/* lpf_data[n] /= LPF_ORDER + 1; */

	}

	/* Find max value */
	for (i=CUTOFF_WINDOW; i<128-CUTOFF_WINDOW; ++i) {
		if (lpf_data[i] > thresh) {
			thresh = lpf_data[i];
		}
	}

	thresh >>= 1;

	/* Threshold data */
	for (i=CUTOFF_WINDOW; i<128-CUTOFF_WINDOW; ++i) {
		if (lpf_data[i] >= thresh) {
			lpf_data[i] = MAX_VALUE;
		} else {
			lpf_data[i] = 0;
		}
	}

	/* Arthmetic */
	for (i=CUTOFF_WINDOW; i<128-CUTOFF_WINDOW; ++i) {
		arg_ev += i * lpf_data[i];
		net_weight += lpf_data[i];
	}

	if (!net_weight) {
		/* We dun messed up! */
		return DEFAULT_RTN;
	}

	return (int16_t)(arg_ev/net_weight) - CNTR;
}
