/*
 * Header for Pixy Parsing & Decision Algorithm
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#ifndef PIXY_PARSER_H
#define PIXY_PARSER_H

#include <stdlib.h>
#include <stdint.h>

/* 0xAA55 for object 0xAA56 for color code. */
#define VO_SIZE					5
#define VO_BYTE_SIZE			10
#define OS_SYNC				0xAA55
#define CC_SYNC				0xAA56
#define FRAME_SYNC			0xAA55
#define PYFLAGS_FRAME_SYNC		1U
#define PYFLAGS_OBJECT_SYNC		2U
#define PYFLAGS_CHECKSUM		4U
#define NUM_OBJECTS			3
#define NUM_MARKERS			2
#define SIG_THRESHOLD			300
#define X_THRESHOLD 			150
#define SIGNATURE_A			0x12
#define SIGNATURE_B			0x13
#define SIGNATURE_CAR			0x4
#define INIT_AREA_X_DEPTH		0xFFFe001 /* pixels^2 metres */


/* Macro for making a word from two bytes */
#define MAKE_WORD(x, y) (((uint16_t) (x) << 8) | (y))
/* Uncomment for Little-endian */
/* #define MAKE_WORD(y, x) (((uint16_t) (x) << 8) | (y)) */

/* List of possible commands */
enum command {FORWARD, LEFT, RIGHT, BACKWARD, NONE};

/* Holds vision data */
struct vision_object {
	uint16_t id, x, y, width, height;
};

struct vec3 {
	double x, y, z;
};

/*
 * Parse pixy data from byte stream.
 *
 * Args:
 *		cmd (enum command*): Location to store output command data.
 *		dist (struct vec3*): Location to store output of vehicle distance for collision avoidance.
 *		bytes (uint8_t*): Byte array containing the vision data.
 *			Note the array does not need to be formatted.
 *		size (uint16_t): Number of bytes.
 *
 * Returns:
 *		int8_t: Error code.
 */
int8_t parse_bytes(enum command* cmd, struct vec3* dist, uint8_t* bytes, uint16_t size);

/*
 * Parse pixy data from 16 bit word stream.
 *
 * Args:
 *		cmd (enum command*): Location to store output command data.
 *		words (uint16_t*): Word array containing the vision data.
 *		dist (struct vec3*): Location to store output of vehicle distance for collision avoidance.
 *			Note the array does not need to be formatted.
 *		size (uint16_t): Number of words.
 *
 * Returns:
 *		int8_t: Error code.
 */
int8_t parse_words(enum command* cmd, struct vec3* dist, uint16_t* words, uint16_t size);

#endif /* PIXY_PARSER_H */
