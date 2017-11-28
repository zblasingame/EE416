/*
 * Implementation for Steering Algorithms
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#include "steering.h"

int8_t steer(double* steering_pid, double* speed_pid, const enum command* command) {
	int32_t wait;
	int32_t wait_multiplier;

	switch(*command) {
		case RIGHT:
			*steering_pid = 1.0;
			wait_multiplier = 1;
			break;
		case LEFT:
			*steering_pid = -1.0;
			wait_multiplier = 1;
			break;
		case BACKWARD:
			*steering_pid = 1.0;
			wait_multiplier = 2;
		default:
			/* not a valid steering command */
			return -1;
	}

	*speed_pid = TURN_SPEED;

	for (wait=0; wait<(WAIT_CYCLE*wait_multiplier); ++wait);

	return 1;
}

int8_t is_steering(const enum command* c) {
	return (*c == RIGHT || *c == LEFT || *c == BACKWARD);
}
