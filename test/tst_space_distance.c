#include <math.h>
#include <stdio.h>
#include "space.h"

static double pts1_arr[] = {
	-73.0,	78.0,	-17.0,
	16.0,	-44.0,	-75.0,
	20.0,	38.0,	22.0,
	57.0,	57.0,	-92.0,
	95.0,	38.0,	73.0,
	77.0,	22.0,	55.0,
	-55.0,	59.0,	-24.0,
	19.0,	73.0,	53.0,
	34.0,	-24.0,	-99.0,
	100.0,	-90.0,	47.0,
	60.0,	11.0,	-23.0,
	-18.0,	86.0,	-54.0,
	75.0,	-69.0,	-7.0,
	-39.0,	12.0,	77.0,
	15.0,	-100.0,	-36.0,
	0.0,	4.0,	51.0,
	-50.0,	90.0,	-52.0,
	-43.0,	-78.0,	85.0,
	8.0,	-51.0,	-65.0
};

static double pts2_arr[] = {
	33.0,	-77.0,	-72.0,
	-54.0,	-85.0,	-4.0,
	40.0,	-22.0,	35.0,
	29.0,	82.0,	6.0,
	7.0,	2.0,	-40.0,
	-41.0,	73.0,	-36.0,
	78.0,	-31.0,	10.0,
	-59.0,	37.0,	37.0,
	100.0,	18.0,	-75.0,
	37.0,	85.0,	25.0,
	-77.0,	-23.0,	-71.0,
	69.0,	17.0,	89.0,
	76.0,	-21.0,	21.0,
	47.0,	-37.0,	-36.0,
	-70.0,	54.0,	-18.0,
	-66.0,	-85.0,	-55.0,
	-28.0,	80.0,	61.0,
	-56.0,	-85.0,	76.0,
	-92.0,	-23.0,	32.0
};

static double ans_arr[] = {
	195.6681,
	107.8054,
	64.5678,
	104.9428,
	147.6787,
	157.4992,
	164.1493,
	87.3842,
	81.8291,
	187.2912,
	149.0939,
	181.0497,
	55.5788,
	150.2198,
	176.8191,
	153.3395,
	115.5552,
	17.2916,
	142.1021
};

static const double delta = 0.0001;

static struct gr_matrix pts1 = {
	.rows = 19,
	.cols = 3,
	.values = pts1_arr
};

static struct gr_matrix pts2 = {
	.rows = 19,
	.cols = 3,
	.values = pts2_arr
};

int main(int argc, char **argv)
{
	int ret = 0;
	struct gr_matrix *distances;

	distances = gr_spc_distance(&pts1, &pts2);
	for (size_t i = 0; i < distances->rows; i++) {
		if (fabs(distances->values[i] - ans_arr[i]) > delta) {
			goto err;
		}
	}
	goto end;
err:
	ret = 1;
	for (size_t i = 0; i < distances->rows; i++) {
		printf("%e\t%e", ans_arr[i], distances->values[i]);
	}
end:
	if (distances != NULL) gr_mtx_delete(distances);
	return ret;
}
