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
#define NUM_BLOBS		3
#define BLOB_SIZE	    20
#define BLOB_NOISE_GAP	5
#define IS_BLACK_THR	0x00FF
#define NUM_TYPES		4

enum line_type {
	NOLINE, LINE, STOP, INTERSECTION
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
 *		data (uint16_t*): Line scan data, already thresholded to {0,1}
 *
 * Returns:
 *		int8_t: Error code.
 */
int8_t get_line_type(enum line_type* line_type, uint16_t* data);

/*
 * Get most likely line type from array.
 *
 * Args:
 *		dest (enum line_type*): Location to store output.
 *		data (enum line_type*): Array of enum line_type.
 *		size (uint8_t): Size of array.
 */
void get_mode(enum line_type* dest, enum line_type* data, uint8_t size);

#endif /* ifndef METRIC_H */
