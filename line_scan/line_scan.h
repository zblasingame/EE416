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
#define CUTOFF_WINDOW   12
#define MAX_VALUE		1
#define LPF_ORDER		5
#define LINE_THR		24
#define STOP_THR		70
#define INTER_THR		100
#define LINET_THR		0x00FF

enum line_type {
	LINE, STOP, INTERSECTION
};

/*
 * Measure distance between line and camera correlation
 *
 * Args:
 *		data (uint16_t*): Measured distribution.
 *		default_val (int16_t): Default value if error.
 *
 * Returns:
 *		int16_t: Correction Term.
 */
int16_t metric(uint16_t* data, int16_t default_val);
int16_t metric_lpf(uint16_t* data, int16_t default_val);
int16_t metric_ml(uint16_t* data);

/*
 * Determine if line is stop or intersection.
 *
 * Args:
 *		line_type (enum line_type*): Location to store line type.
 *		data (uint16_t*): Line scan pidatael data.
 */
void get_line_type(enum line_type* line_type, uint16_t* data);

#endif /* ifndef METRIC_H */
