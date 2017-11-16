/*
 * Program to compare ideal and measured distributions
 *
 * Author:		Zander Blasingame
 * Class:		EE 416
 *
 */

#include <stdio.h>
#include "line_scan.h"

int16_t metric(uint16_t* data, int16_t default_val) {
	/* Here be vars */
	uint16_t i;
	uint32_t arg_ev = 0;
	uint32_t net_weight = 0;

	/* Arthmetic */
	for (i=CUTOFF_WINDOW; i<128-CUTOFF_WINDOW; ++i) {
		/* Inversion of data */
		data[i] = MAX_VALUE - data[i];

		arg_ev += i * data[i];
		net_weight += data[i];
	}

	if (net_weight < LINE_THR) {
		/* We dun messed up! */
		return default_val;
	}

	return (int16_t)(arg_ev/net_weight) - CNTR;
}

int16_t metric_lpf(uint16_t* data, int16_t default_val) {
	/* Here be vars */
	uint16_t i, n;
	uint16_t lpf_order;
	uint16_t lpf_data[128];
	uint32_t arg_ev = 0;
	uint32_t net_weight = 0;
	uint16_t thresh = 0;

	/* Low pass filter */
	for (n=CUTOFF_WINDOW; n<128-CUTOFF_WINDOW; ++n) {
		lpf_data[n] = 0;
		lpf_order = n - (LPF_ORDER+1) > 0 ? LPF_ORDER + 1 : n;

		for (i=0; i<lpf_order; ++i) {
			lpf_data[n] += data[n-i];
		}
		/* lpf_data[n] /= LPF_ORDER + 1; */

	}

	/* Find max value */
	for (i=CUTOFF_WINDOW; i<128-CUTOFF_WINDOW; ++i) {
		if (lpf_data[i] > thresh) {
			thresh = lpf_data[i];
		}
	}

	thresh >>= 1;

	/* Threshold data */
	for (i=CUTOFF_WINDOW; i<128-CUTOFF_WINDOW; ++i) {
		if (lpf_data[i] >= thresh) {
			lpf_data[i] = MAX_VALUE;
		} else {
			lpf_data[i] = 0;
		}
	}

	/* Arthmetic */
	for (i=CUTOFF_WINDOW; i<128-CUTOFF_WINDOW; ++i) {
		arg_ev += i * lpf_data[i];
		net_weight += lpf_data[i];
	}

	if (net_weight < LINE_THR) {
		/* We dun messed up! */
		return default_val;
	}

	return (int16_t)(arg_ev/net_weight) - CNTR;
}

int16_t metric_ml(uint16_t* data) {
	const static double weights[129] = {
-1.00410827073e-05,-5.20337906465e-06,2.55101101156e-05,4.1036101616e-05,5.3694117423e-05,6.79836661652e-05,7.37722522915e-05,8.07579229959e-05,8.30978411624e-05,0.00010393335877,0.000111903587027,0.000123089796991,0.000125881619082,0.000139920437426,0.000139842269335,0.00012367961993,0.000125292949405,0.000109731180254,8.29501333873e-05,7.62485491838e-05,6.45897281509e-05,5.73216416145e-05,5.17018215126e-05,3.37029098779e-05,2.31317894044e-05,2.10064019378e-05,2.4022246026e-05,1.54779522162e-05,8.67042333072e-06,1.25890514557e-05,9.65458343738e-06,-1.14219205036e-06,1.07939489226e-05,1.29945460259e-05,2.08637047569e-05,1.39186524577e-05,-8.48500727898e-06,1.50048483689e-05,5.41477222734e-06,1.08094300899e-05,1.43707688686e-05,2.0602575644e-05,1.94024836424e-05,2.15042184078e-05,2.50617047666e-05,2.70097438274e-05,7.04360667363e-06,1.53051330727e-05,1.45426078932e-05,9.25124807537e-06,1.60192101725e-05,5.08146895611e-06,3.96075632832e-06,1.41775232075e-05,2.55113560569e-06,4.89301038932e-07,-2.73187567309e-06,7.52259679068e-06,-7.72293934614e-06,-1.44864491845e-06,-6.95973874263e-06,1.26824301717e-05,1.4590916664e-07,1.13093354732e-05,1.53697032813e-07,-5.17682423568e-06,-8.60658255823e-06,-2.30441645373e-06,2.44768666111e-06,-3.58410006405e-07,7.63233545099e-06,-2.23516393637e-06,3.0515491606e-06,1.99272370193e-06,-1.40037779058e-05,-9.01473021238e-07,-9.4028249183e-07,-1.16342588863e-05,-1.39586655726e-05,-1.29419539256e-05,-1.52570596909e-05,-2.21541959814e-05,-1.57916871366e-05,-1.0913711655e-05,-2.40357591846e-05,-2.43415860849e-05,-1.10710309756e-05,-1.84288105129e-05,-1.20513475425e-05,-1.33598135832e-05,-1.03767629781e-05,-4.35630160881e-06,-9.21464655558e-07,-4.63946746988e-06,-1.20754363038e-05,-6.30300400454e-06,-1.22625288081e-05,-1.98546711177e-05,-1.51221354923e-05,-1.22820510522e-05,-2.58170832916e-05,-1.45227841952e-05,-2.07182739288e-05,-2.99109078745e-05,-4.68555675879e-05,-3.94878517332e-05,-6.64040052136e-05,-6.8850543795e-05,-8.93266241436e-05,-0.000112230216929,-0.000100995232907,-0.000113790076918,-0.00013389249403,-0.000139630888448,-0.000125413669612,-0.000127042734391,-0.000102728988813,-9.89574905132e-05,-9.07031734184e-05,-9.40375364533e-05,-6.65372491217e-05,-7.43917723451e-05,-5.90638158074e-05,-4.58573177269e-05,-3.91754103195e-05,-2.17872171659e-05,-1.47685349739e-05,-1.80241983239e-05,72.6947794945
};

	uint8_t i;
	double centre = 0.0;

	/* Arthmetic */
	for (i=0; i<128; ++i) {
		centre += weights[i] * (double) data[i];
	}
	centre += weights[i];

	printf("%.2f\n", centre);

	return (int16_t)(centre) - CNTR;
}

void get_line_type(enum line_type* line_type, uint16_t* data) {
	uint16_t num_black = 0;
	uint16_t* stop = data + 128;

	do {
		num_black += (*data++ > IS_BLACK_THR) ? 1 : 0;
	} while (data < stop);

	if (num_black > INTER_THR) {
		*line_type = INTERSECTION;
	} else if (num_black > STOP_THR) {
		*line_type = STOP;
	} else if (num_black > LINE_THR) {
		*line_type = LINE;
	} else {
		*line_type = NOLINE;
	}
}

void get_mode(enum line_type* dest, enum line_type* data, uint8_t size) {
	/* Manually define occurance array */
	uint8_t occs[NUM_TYPES] = {0};
	uint8_t i = 0;
	uint8_t max_occ = 0;
	enum line_type* stop = data + size;
	enum line_type mode = NOLINE;

	do {
		++occs[*data++];
	} while (data < stop);

	for (i=0; i<NUM_TYPES; ++i) {
		if (occs[i] > max_occ) {
			mode = (enum line_type) i;
			max_occ = occs[i];
		}
	}

	*dest = mode;
}
