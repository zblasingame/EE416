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
#define VO_SIZE					10
#define SYNC_WORD				0xAA56
#define FRAME_SYNC				0xAA55
#define PYFLAGS_FRAME_SYNC		1U
#define PYFLAGS_OBJECT_SYNC		2U
#define PYFLAGS_CHECKSUM		4U
#define NUM_OBJECTS				2

/* Macro for making a word from two bytes */
#define MAKE_WORD(x, y) (((uint16_t) (x) << 8) | (y))

/* List of possible commands */
typedef enum {FORWARD, LEFT, RIGHT, BACKWARD} command;

/* Holds vision data */
typedef struct {
	uint16_t id, x, y, width, height;
} vision_object;

/*
 * Gets a command based on vision data.
 *
 * Args:
 *		bytes (uint8_t*): Byte array containing the vision data.
 *			Note the array does not need to be formatted.
 *		size (uint16_t): Number of bytes.
 *
 * Returns:
 *		command: The command for the system.
 */
command get_command(uint8_t* bytes, uint16_t size);

#endif /* PIXY_PARSER_H */
