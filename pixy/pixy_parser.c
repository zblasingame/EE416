/*
 * Pixy Parsing & Decision Algorithm
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#include <stdio.h>
#include "pixy_parser.h"

/*
 * Gets a command based on vision data.
 *
 * Args:
 *		cmd (enum command*): Location to store output command data.
 *		sig (uint_t*): Location to store output siginificance flag.
 *		bytes (uint8_t*): Byte array containing the vision data.
 *			Note the array does not need to be formatted.
 *		size (uint16_t): Number of bytes.
 *
 * Returns:
 *		int8_t: Error code.
 */

int8_t get_command(enum command* cmd, uint8_t* sig, uint8_t* bytes, uint16_t size) {
	static const command LUT[4] = {
		FORWARD,
		LEFT,
		RIGHT,
		BACKWARD
	};

	/* Here be vars */
	uint16_t sig_comb;
	uint8_t FLAGS = 0;
	uint16_t checksum;
	uint8_t num_objects = 0;

	uint8_t* stop_address = bytes + size;
	uint8_t* vo_stop;
	uint8_t* curr_byte;
	uint8_t* curr_vo_byte;
	uint8_t* vo_object = (uint8_t*) malloc(sizeof(uint8_t) * VO_SIZE);

	struct vision_object* vo_objects = (struct vision_object*) malloc(sizeof(vision_object) * NUM_OBJECTS);
	struct vision_object* curr_vo_object = vo_objects;
	struct vision_object* tmp = (struct vision_object*) malloc(sizeof(vision_object));

	/* code */

	/* iterate through all of the bytes */
	do {
		if (!(FLAGS & PYFLAGS_FRAME_SYNC) && MAKE_WORD(*(bytes+1), *bytes) == FRAME_SYNC) {
			FLAGS |= PYFLAGS_FRAME_SYNC;
			bytes += 2;
		} else if (!(FLAGS & PYFLAGS_OBJECT_SYNC) && (FLAGS & PYFLAGS_FRAME_SYNC) && MAKE_WORD(*(bytes+1), *bytes) == SYNC_WORD) {
			FLAGS |= PYFLAGS_OBJECT_SYNC;
			bytes += 2;
		} else if (!(FLAGS & PYFLAGS_CHECKSUM) && (FLAGS & PYFLAGS_OBJECT_SYNC)) {
			vo_stop = bytes + VO_SIZE + 2;
			if (vo_stop <= stop_address) {
				checksum = 0;
				for (curr_byte=bytes+2; curr_byte < vo_stop - 1; curr_byte += 2) {
					checksum += MAKE_WORD(*(curr_byte+1), *curr_byte);
				}
				if (MAKE_WORD(*(bytes+1), *bytes) == checksum) {
					FLAGS |= PYFLAGS_CHECKSUM;
					bytes += 2;
				} else {
					FLAGS &= ~PYFLAGS_OBJECT_SYNC;
					bytes += VO_SIZE;
				}
			} else {
				++bytes;
			}
		} else if (FLAGS & PYFLAGS_CHECKSUM) {
			vo_stop = bytes + VO_SIZE;
			curr_vo_byte = vo_object;

			do {
				*curr_vo_byte++ = *bytes;
			} while (++bytes < vo_stop);

			tmp = (vision_object*) vo_object;

			if (tmp->width * tmp->height > SIG_THRESHOLD) {
				*curr_vo_object++ = *tmp;
				++num_objects;
			}

			FLAGS &= ~PYFLAGS_OBJECT_SYNC;
			FLAGS &= ~PYFLAGS_CHECKSUM;
		} else {
			++bytes;
		}

	} while (bytes < stop_address - 1 && num_objects < NUM_OBJECTS);


	if (num_objects < NUM_OBJECTS) {
		*sig = 0;
		return 1;
	}

	if (vo_objects->x > (vo_objects+1)->x) {
		*tmp = *vo_objects;
		*vo_objects = *(vo_objects+1);
		*(vo_objects+1) = *tmp;
	}

	sig_comb = (vo_objects->id<<1) + (vo_objects+1)->id;

	free(vo_object);
	free(vo_objects);
	free(tmp);

	*sig = 1;
	*cmd = LUT[sig_comb];

	return 1;
}
