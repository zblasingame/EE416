/*
 * Header for Steering Algorithms
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#ifndef STEERING_H
#define STEERING_H

#include "pixy_parser.h"

#define WAIT_CYCLE		100000000
#define TURN_SPEED		0.1

/*
 * Steer vehicle according to command.
 *
 * Args:
 *		command (const enum command*): Location to read command.
 *		steering_pid (double*): Location to store ouput steering pid.
 *		speed_pid (double*): Location to store output speed pid.
 * Returns:
 *		int8_t: Error code.
 */
int8_t steer(double* steering_pid, double* speed_pid, const enum command* command);

/*
 * Check if valid steering command
 *
 * Args:
 *		command (const enum command*): Location to read command.
 *
 * Returns:
 *		int8_t: 1 if steering command.
 */
int8_t is_steering(const enum command* command);

#endif /* STEERING_H */
