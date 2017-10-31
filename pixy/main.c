/*
 * Program to yield signature decision
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define VO_SIZE		14
#define SYNC_WORD	0xAA55

#define MAKE_WORD(x, y) (((uint16_t) *(x) << 8) | *(y))

typedef enum {FORWARD, LEFT, RIGHT, BACKWARD} command;

typedef struct {
	uint16_t sync, checksum, id, x, y, width, height;
} vision_object;

/*
 * Gets the command based on vision data.
 *
 * Args:
 *		bytes (uint8_t*): Byte array.
 *		size (uint16_t): Number of bytes.
 *
 * Returns:
 *		command: The command for the system.
 */
command get_command(uint8_t* bytes, uint16_t size) {
	static const command LUT[4] = {
		FORWARD,
		LEFT,
		RIGHT,
		BACKWARD
	};

	/* Here be vars */
	uint16_t sig_comb;

	uint8_t* sync_byte;
	uint8_t* curr_byte;
	uint8_t* curr_vo_byte;
	uint8_t* vo_bytes_start = (uint8_t*) malloc(sizeof(uint8_t) * VO_SIZE);
	uint8_t* stop_address = bytes + size;

	vision_object* tmp;
	vision_object left = {0, 0, 0, 0, 0, 0, 0};
	vision_object right = {0, 0, 0, 0, 0, 0, 0};

	/* code */

	/* Find first sync byte start from there */
	while (bytes < stop_address-1 && (MAKE_WORD(bytes+1, bytes) != SYNC_WORD)) {
		++bytes;
	}

	/* Ensure enough data remains */
	if ((stop_address-bytes) < 2 * VO_SIZE) {
		return FORWARD;
	}

	/* Grab vision objects */
	for (sync_byte=bytes; sync_byte < stop_address-VO_SIZE; sync_byte+=VO_SIZE) {
		curr_vo_byte = vo_bytes_start;
		for (curr_byte=sync_byte; curr_byte-sync_byte<VO_SIZE; ++curr_byte) {
			*curr_vo_byte++ = *curr_byte;
		}

		tmp = (vision_object*) vo_bytes_start;

		if ((tmp->width * tmp->height) > (left.width * left.height)) {
			right = left;
			left = *tmp;
		} else if ((tmp->width * tmp->height) > (right.width * right.height)) {
			right = *tmp;
		}
	}

	if (left.x > right.x) {
		*tmp = left;
		left = right;
		right = *tmp;
	}

	/* printf("(%d, %d)\n", left.id, right.id); */

	sig_comb = (left.id<<1) + right.id;
	free(vo_bytes_start);

	return LUT[sig_comb];
}

int main(void) {
	uint8_t bytes[70] = {
		0x55, 0xaa, 0xff, 0xff, 0x01, 0x00, 0x95, 0x39, 0xf5, 0x6f, 0x13, 0x4c, 0xbe, 0x0d,
		0x55, 0xaa, 0xff, 0xff, 0x00, 0x00, 0xe8, 0x1b, 0x07, 0x7d, 0x0d, 0x0f, 0x2d, 0x37,
		0x55, 0xaa, 0xff, 0xff, 0x00, 0x00, 0x3f, 0x5e, 0x29, 0x72, 0xfb, 0x04, 0x81, 0xba,
		0x55, 0xaa, 0xff, 0xff, 0x00, 0x00, 0x98, 0xed, 0x57, 0xcc, 0x9e, 0x03, 0xa8, 0x3f,
		0x55, 0xaa, 0xff, 0xff, 0x00, 0x00, 0x2b, 0xa2, 0x97, 0xd9, 0xbb, 0xb0, 0xbf, 0x43,
	};

	command c = RIGHT;

	c = get_command(bytes, 50);

	printf("%d\n", c);
}
