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

	switch(*command) {
		case RIGHT:
			*steering_pid = 1.0;
			break;
		case LEFT:
			*steering_pid = -1.0;
			break;
		case BACKWARD:
			*steering_pid = 1.0;
			break;
		default:
			/* not a valid steering command */
			return -1;
	}

	*speed_pid = TURN_SPEED;

	for (wait=0; wait<WAIT_CYCLE; ++wait);

	if (*command == BACKWARD) {
		*steering_pid = -1.0;
		*speed_pid = -TURN_SPEED;
		for (wait=0; wait<WAIT_CYCLE; ++wait);
	}

	return 1;
}

int8_t is_steering(const enum command* c) {
	return (*c == RIGHT || *c == LEFT || *c == BACKWARD);
}
