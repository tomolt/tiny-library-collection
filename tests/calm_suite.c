#include <stdlib.h>
#include <math.h>

#include "sd_cuts.h"

#define CM_IMPLEMENT_HERE
#include "calm.h"

const double EPSILON = 0.0001;

static int cmp_v4(cm_v4 a, cm_v4 b)
{
	float ac[4], bc[4];
	cm_recv_v4(a, ac);
	cm_recv_v4(b, bc);
	for (int i = 0; i < 4; i++) {
		if (fabs(ac[i] - bc[i]) > EPSILON)
			return 0;
	}
	return 1;
}

static int cmp_m16(cm_m16 a, cm_m16 b)
{
	float ac[16], bc[16];
	cm_recv_m16(a, ac);
	cm_recv_m16(b, bc);
	for (int i = 0; i < 16; i++) {
		if (fabs(ac[i] - bc[i]) > EPSILON)
			return 0;
	}
	return 1;
}

void test_calm_mul_m16(void)
{
	sd_push("multiplying two matrices");
	const cm_m16 a = cm_send_m16((float[]){
		 1,  2,  3,  4,
		 5,  6,  7,  8,
		 9, 10, 11, 12,
		13, 14, 15, 16});
	const cm_m16 b = cm_send_m16((float[]){
		17, 18, 19, 20,
		21, 22, 23, 24,
		25, 26, 27, 28,
		29, 30, 31, 32});
	const cm_m16 r = cm_dot_m16(a, b);
	const cm_m16 E = cm_send_m16((float[]){
		538,  612,  686,  760,
		650,  740,  830,  920,
		762,  868,  974, 1080,
		874,  996, 1118, 1240});
	sd_assert(cmp_m16(r, E));
	sd_pop();
}

void test_inverse_m16_success(void)
{
	sd_push("compute correct matrix inverse");
	const cm_m16 m = cm_send_m16((float[]){
		 1,  4,  2,  3,
		 0,  1,  4,  4,
		-1,  0,  1,  0,
		 2,  0,  4,  1});
	const cm_m16 r = cm_inverse_m16(m);
	const cm_m16 E = cm_send_m16((float[]){
		1/65.0, -4/65.0, -38/65.0, 13/65.0,
		20/65.0, -15/65.0, 20/65.0, 0/65.0,
		1/65.0, -4/65.0, 27/65.0, 13/65.0,
		-6/65.0, 24/65.0, -32/65.0, -13/65.0});
	sd_assert(cmp_m16(r, E));
	sd_pop();
}

void test_inverse_m16_failure(void)
{
	sd_push("identify failing matrix inverse");
	const cm_m16 m = cm_send_m16((float[]){
		 1,  5,  0, 13,
		 2,  6,  0, 14,
		 3,  7,  0, 15,
		 4,  8,  0, 16});
	const cm_m16 r = cm_inverse_m16(m);
	const cm_m16 E = cm_identity_m16();
	sd_assert(cmp_m16(r, E));
	sd_pop();
}

void test_calm_look_at(void)
{
	sd_push("build look at matrix");
	const cm_m16 r = cm_lookat_m16(
		cm_new_v3(4.0, 3.0, 3.0),
		cm_new_v3(0.0, 0.0, 0.0),
		cm_new_v3(0.0, 1.0, 0.0));
	const cm_m16 E = cm_send_m16((float[]){
		 0.6, -0.411597,  0.685994,  0.0,
		 0.0,  0.857493,  0.514496,  0.0,
		-0.8, -0.308697,  0.514496,  0.0,
		 0.0,  0.0,      -5.830953,  1.0});
	sd_assert(cmp_m16(r, E));
	sd_pop();
}

void test_calm_qt_from_axis(void)
{
	sd_push("make quaternion from axis");
	const cm_v4 A = cm_norm_v3(cm_new_v3(0.3, 0.4, 0.6));
	const cm_qt Q = cm_new_qt(A, 1.5);
	const cm_qt E = cm_new_v4(0.261825, 0.3491, 0.523649, 0.731689);
	sd_assert(cmp_v4(Q, E));
	sd_pop();
}

void test_calm_m16_from_qt(void)
{
	sd_push("matrix from quaternion");
	const cm_v4 A = cm_norm_v3(cm_new_v3(0.3, 0.4, 0.6));
	const cm_qt Q = cm_new_qt(A, 1.5);
	const cm_m16 r = cm_qt_to_m16(Q);
	const cm_m16 E = cm_send_m16((float[]){
		 0.207841, -0.583491,  0.785073, 0.0,
		 0.949103,  0.314478, -0.017537, 0.0,
		-0.236656,  0.748760,  0.619155, 0.0,
		 0.0,       0.0,       0.0,      1.0});
	sd_assert(cmp_m16(r, E));
	sd_pop();
}

void test_calm_qt_cumulate(void)
{
	sd_push("cumulate quaternions");
	const cm_v4 A1 = cm_norm_v3(cm_new_v3(0.3, 0.4, 0.6));
	const cm_qt Q1 = cm_new_qt(A1, 1.5f);
	const cm_v4 A2 = cm_norm_v3(cm_new_v3(0.9, 0.2, 0.7));
	const cm_qt Q2 = cm_new_qt(A2, 2.93f);
	const cm_qt R  = cm_cum_qt(Q1, Q2);
	const cm_qt E  = cm_new_v4(0.473386, -0.084835, 0.720199, -0.500023);
	sd_assert(cmp_v4(R, E));
	sd_pop();
}

void calm_suite(void)
{
	sd_push("3D Math");
	test_calm_mul_m16();
	test_inverse_m16_success();
	test_inverse_m16_failure();
	test_calm_look_at();
	test_calm_qt_from_axis();
	test_calm_m16_from_qt();
	test_calm_qt_cumulate();
	sd_pop();
}
