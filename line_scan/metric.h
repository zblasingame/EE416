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

#endif /* ifndef METRIC_H */
