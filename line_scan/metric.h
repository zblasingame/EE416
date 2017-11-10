/*
 * Program to compare ideal and measured distributions
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#include <stdint.h>

#ifndef METRIC_H
#define METRIC_H

#define CNTR			64
#define CUTOFF_WINDOW	0
#define MAX_VALUE		1
#define DEFAULT_RTN	    0xF000
#define LPF_ORDER		5

/*
 * Measure argmax cross correlation
 *
 * Args:
 *		x (uint16_t*): Measured distribution.
 *
 * Returns:
 *		int16_t: Correction Term.
 */
int16_t metric(uint16_t* x);
int16_t metric_lpf(uint16_t* x);
int16_t metric_ml(uint16_t* x);

#endif /* ifndef METRIC_H */
