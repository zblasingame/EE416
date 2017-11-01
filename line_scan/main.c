/*
 * Program to compare ideal and measured distributions
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#include<stdint.h>
#include<stdio.h>

#include "metric.h"

uint16_t* create_signal(uint16_t start, uint16_t stop);

int main(void) {
	uint16_t start, stop;
	uint16_t i;
	int16_t div;

	start = 0;
	stop = 5;

	for (i=0; i < 120; ++i) {
		div = metric_lpf(create_signal(start, stop));
		printf("[%d, %d]\n", start, stop);
		printf("The divergence: %d\n", div);
		++start;
		++stop;
	}

	return 0;
}

uint16_t* create_signal(uint16_t start, uint16_t stop) {
	static uint16_t signal[128];
	uint16_t i;

	for (i=0; i < 128; ++i) {
		if ((i > start) && (i < stop)) {
			*(signal+i) = 1;
		} else {
			*(signal+i) = 0;
		}
	}

	return signal;
}
