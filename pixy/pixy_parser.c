/*
 * Pixy Parsing & Decision Algorithm
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#include <stdio.h>
#include "pixy_parser.h"

int8_t parse_bytes(enum command* cmd, uint8_t* sig, struct vec3* dist, uint8_t* bytes, uint16_t size) {
	/* Here be vars */
	uint16_t sig_comb;
	uint8_t FLAGS = 0;
	uint16_t checksum;
	uint8_t num_objects = 0;
	int8_t error_code = 1;

	uint8_t* stop_address = bytes + size;
	uint8_t* vo_stop;
	uint8_t* curr_byte;
	uint8_t* curr_vo_byte;
	uint8_t* vo_object = (uint8_t*) malloc(sizeof(uint8_t) * VO_BYTE_SIZE);

	struct vision_object* vo_objects = (struct vision_object*) malloc(sizeof(struct vision_object) * NUM_OBJECTS);
	struct vision_object* curr_vo_object = vo_objects;
	struct vision_object* tmp;

	/* code */

	/* iterate through all of the bytes */
	do {
		if (!(FLAGS & PYFLAGS_FRAME_SYNC) && MAKE_WORD(*(bytes+1), *bytes) == FRAME_SYNC) {
			FLAGS |= PYFLAGS_FRAME_SYNC;
			bytes += 2;
		} else if (!(FLAGS & PYFLAGS_OBJECT_SYNC)
				&& (FLAGS & PYFLAGS_FRAME_SYNC)
				&& (MAKE_WORD(*(bytes+1), *bytes) == OS_SYNC || MAKE_WORD(*(bytes+1), *bytes) == CC_SYNC)) {
			FLAGS |= PYFLAGS_OBJECT_SYNC;
			bytes += 2;
		} else if (!(FLAGS & PYFLAGS_CHECKSUM) && (FLAGS & PYFLAGS_OBJECT_SYNC)) {
			vo_stop = bytes + VO_BYTE_SIZE + 2;
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
					bytes += VO_BYTE_SIZE;
				}
			} else {
				/* error data clipped prematurely */
				error_code = -1;
				goto EXIT;
			}
		} else if (FLAGS & PYFLAGS_CHECKSUM) {
			vo_stop = bytes + VO_BYTE_SIZE;
			curr_vo_byte = vo_object;

			while (bytes < vo_stop) {
				*curr_vo_byte++ = *bytes++;
			}

			tmp = (struct vision_object*) vo_object;

			if (tmp->width * tmp->height > SIG_THRESHOLD) {
				if (tmp->id == SIGNATURE_CAR) {
					dist->x = (double) tmp->x;
					dist->y = (double) tmp->y;
					dist->z = ((double)INIT_AREA_X_DEPTH)/((double)(tmp->width * tmp->height));
					++num_objects;
				} else if (tmp->id == SIGNATURE_A || tmp->id == SIGNATURE_B) {
					*curr_vo_object++ = *tmp;
					++num_objects;
				} else {
					/* invalid signature */
					error_code = -2;
					goto EXIT;
				}
			}

			FLAGS &= ~PYFLAGS_OBJECT_SYNC;
			FLAGS &= ~PYFLAGS_CHECKSUM;
		} else {
			++bytes;
		}
	} while (bytes < stop_address - 1 && num_objects < NUM_OBJECTS);

	if (num_objects < NUM_OBJECTS) {
		*sig = 0;
		goto EXIT;
	}

	if (vo_objects->x > (vo_objects+1)->x) {
		*tmp = *vo_objects;
		*vo_objects = *(vo_objects+1);
		*(vo_objects+1) = *tmp;
	}

	*sig = 1;
	sig_comb = (vo_objects->id << 8) | (vo_objects+1)->id;

	switch (sig_comb) {
		case MAKE_WORD(SIGNATURE_A, SIGNATURE_A):
			*cmd = FORWARD;
			break;
		case MAKE_WORD(SIGNATURE_A, SIGNATURE_B):
			*cmd = RIGHT;
			break;
		case MAKE_WORD(SIGNATURE_B, SIGNATURE_A):
			*cmd = LEFT;
			break;
		case MAKE_WORD(SIGNATURE_B, SIGNATURE_B):
			*cmd = BACKWARD;
			break;
		default:
			*cmd = FORWARD;
	}

	EXIT:
		free(vo_object);
		free(vo_objects);

		return error_code;
}

int8_t parse_words(enum command* cmd, uint8_t* sig, struct vec3* dist, uint16_t* words, uint16_t size) {
	/* Here be vars */
	uint16_t sig_comb;
	uint8_t FLAGS = 0;
	uint16_t checksum;
	uint8_t num_objects = 0;
	int8_t error_code = 1;

	uint16_t* stop_address = words + size;
	uint16_t* vo_stop;
	uint16_t* curr_word;
	uint16_t* curr_vo_word;
	uint16_t* vo_object = (uint16_t*) malloc(sizeof(uint16_t) * VO_SIZE);

	struct vision_object* vo_objects = (struct vision_object*) malloc(sizeof(struct vision_object) * NUM_OBJECTS);
	struct vision_object* curr_vo_object = vo_objects;
	struct vision_object* tmp;

	/* code */

	/* iterate through all of the bytes */
	do {
		if (!(FLAGS & PYFLAGS_FRAME_SYNC) && *words == FRAME_SYNC) {
			FLAGS |= PYFLAGS_FRAME_SYNC;
			++words;
		} else if (!(FLAGS & PYFLAGS_OBJECT_SYNC) && (FLAGS & PYFLAGS_FRAME_SYNC) && *words == OS_SYNC) {
			FLAGS |= PYFLAGS_OBJECT_SYNC;
			++words;
		} else if (!(FLAGS & PYFLAGS_CHECKSUM) && (FLAGS & PYFLAGS_OBJECT_SYNC)) {
			vo_stop = words + VO_SIZE + 1;
			if (vo_stop <= stop_address) {
				checksum = 0;
				for (curr_word=words+1; curr_word < vo_stop; ++curr_word) {
					checksum += *curr_word;
				}
				if (*words == checksum) {
					FLAGS |= PYFLAGS_CHECKSUM;
					++words;
				} else {
					FLAGS &= ~PYFLAGS_OBJECT_SYNC;
					words += VO_SIZE;
				}
			} else {
				/* error data clipped prematurely */
				error_code =  -1;
				goto EXIT;
			}
		} else if (FLAGS & PYFLAGS_CHECKSUM) {
			vo_stop = words + VO_SIZE;
			curr_vo_word = vo_object;

			while (words < vo_stop) {
				*curr_vo_word++ = *words++;
			}

			tmp = (struct vision_object*) vo_object;

			if (tmp->width * tmp->height > SIG_THRESHOLD) {
				if (tmp->id == SIGNATURE_CAR) {
					dist->x = (double) tmp->x;
					dist->y = (double) tmp->y;
					dist->z = ((double)INIT_AREA_X_DEPTH)/((double)(tmp->width * tmp->height));
					++num_objects;
				} else if (tmp->id == SIGNATURE_A || tmp->id == SIGNATURE_B) {
					*curr_vo_object++ = *tmp;
					++num_objects;
				} else {
					/* invalid signature */
					error_code = -2;
					goto EXIT;
				}
			}

			FLAGS &= ~PYFLAGS_OBJECT_SYNC;
			FLAGS &= ~PYFLAGS_CHECKSUM;
		} else {
			++words;
		}

	} while (words < stop_address && num_objects < NUM_OBJECTS);

	if (num_objects < NUM_OBJECTS) {
		*sig = 0;
		goto EXIT;
	}

	if (vo_objects->x > (vo_objects+1)->x) {
		*tmp = *vo_objects;
		*vo_objects = *(vo_objects+1);
		*(vo_objects+1) = *tmp;
	}

	*sig = 1;
	sig_comb = (vo_objects->id << 8) | (vo_objects+1)->id;

	switch (sig_comb) {
		case MAKE_WORD(SIGNATURE_A, SIGNATURE_A):
			*cmd = FORWARD;
			break;
		case MAKE_WORD(SIGNATURE_A, SIGNATURE_B):
			*cmd = RIGHT;
			break;
		case MAKE_WORD(SIGNATURE_B, SIGNATURE_A):
			*cmd = LEFT;
			break;
		case MAKE_WORD(SIGNATURE_B, SIGNATURE_B):
			*cmd = BACKWARD;
			break;
		default:
			*cmd = FORWARD;
	}

	EXIT:
		free(vo_object);
		free(vo_objects);

		return error_code;
}
