#include <stdio.h>
#include "pixy_parser.h"
#include "steering.h"

void main(void) {
	enum command c = FORWARD;

	double steering_pid = 1.0;
	double speed_pid = 0.5;

	do {
		if (is_steering(&c)) {
			steer(&steering_pid, &speed_pid, &c);
			printf("%.1f, %.1f\n", steering_pid, speed_pid);
		}
	} while(++c< 4);
}
