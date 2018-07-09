/* calm.h - C Accelerated Linear Maths library
 * 
 * MIT License
 *
 * Copyright (c) 2018 Thomas Oltmann
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * This file was originally distributed as part of Thomas Oltmann's personal
 * tiny library collection. (https://www.github.com/tomolt/tiny-library-collection)
 */

#ifndef CM_CALM_H
#define CM_CALM_H

typedef float cm_f1;

// TEMP
#define CM_BACKEND 0

#define CM_BACKEND_FALLBACK	0
#define CM_BACKEND_SSE		1

/* If the user didn't supply a CM_BACKEND, then choose automatically. */
#ifndef CM_BACKEND
#	if defined(__SSE__) || _M_IX86_FP > 0 || _M_X64 > 0
#		define CM_BACKEND CM_BACKEND_SSE
#	else
#		define CM_BACKEND CM_BACKEND_FALLBACK
#	endif
#endif

#if CM_BACKEND == CM_BACKEND_SSE
#error "SSE SHOULD BE DISABLED RIGHT NOW!"
#	include <xmmintrin.h>
	typedef __m128 cm_f4_;
#	if defined(_MSC_VER)
#		define CM_DEF	_vectorcall
#		define CM_ALIGN16	__declspec(align(16))
#	elif defined(__GNUC__)
#		define CM_DEF
#		define CM_ALIGN16	__attribute__((aligned(16)))
#	elif defined(__clang__)
#		define CM_DEF	__attribute__ ((vectorcall))
#		define CM_ALIGN16	__attribute__((aligned(16)))
#	else
#		define CM_DEF
#		define CM_ALIGN16
#	endif
#else
	typedef struct { cm_f1 c[4]; } cm_f4_;
#	define CM_DEF
#endif

/* M_PI isn't actually officially part of the C standard library.
 * In case it's missing, you can use CM_PI instead. */
#define CM_PI 3.1415926535897932

typedef cm_f4_ cm_v4;

CM_DEF cm_v4  cm_new_v4(cm_f1 a, cm_f1 b, cm_f1 c, cm_f1 d);
CM_DEF cm_v4  cm_send_v4(cm_f1 c[4]);
CM_DEF cm_v4  cm_send1_v4(cm_f1 a);
CM_DEF cm_v4  cm_add_v4(cm_v4 l, cm_v4 r);
CM_DEF cm_v4  cm_sub_v4(cm_v4 l, cm_v4 r);
CM_DEF cm_v4  cm_mul_v4(cm_v4 l, cm_v4 r);
CM_DEF void   cm_recv_v4(cm_v4 f, cm_f1 o[4]);
CM_DEF cm_f1  cm_hsum_v4(cm_v4 f);
CM_DEF cm_v4  cm_scale_v4(cm_v4 f, cm_f1 s);
CM_DEF cm_f1  cm_dot_v4(cm_v4 l, cm_v4 r);
CM_DEF cm_f1  cm_length_v4(cm_v4 v);
CM_DEF cm_v4  cm_norm_v4(cm_v4 v);

typedef cm_f4_ cm_v3;

#define       cm_new_v3(a, b, c) cm_new_v4(a, b, c, 0.0f)
CM_DEF cm_f1  cm_dot_v3(cm_v4 l, cm_v4 r);
CM_DEF cm_f1  cm_length_v3(cm_v4 v);
CM_DEF cm_v4  cm_norm_v3(cm_v4 v);
CM_DEF cm_f1  cm_hsum_v3(cm_v4 f);
CM_DEF cm_v4  cm_cross_v3(cm_v4 l, cm_v4 r);

typedef struct { cm_v4 c[4]; } cm_m16;

CM_DEF cm_m16 cm_identity_m16(void);
CM_DEF cm_m16 cm_new_m16(cm_v4 a, cm_v4 b, cm_v4 c, cm_v4 d);
CM_DEF cm_m16 cm_send_m16(cm_f1 c[16]);
CM_DEF void   cm_recv_m16(cm_m16 m, cm_f1 c[16]);
CM_DEF cm_m16 cm_add_m16(cm_m16 l, cm_m16 r);
CM_DEF cm_m16 cm_sub_m16(cm_m16 l, cm_m16 r);
CM_DEF cm_v4  cm_apply_m16(cm_m16 m, cm_v4 f);
CM_DEF cm_m16 cm_transpose_m16(cm_m16 m);
CM_DEF cm_m16 cm_inverse_m16(cm_m16 m);
CM_DEF cm_m16 cm_dot_m16(cm_m16 l, cm_m16 r);
CM_DEF cm_m16 cm_translate_m16(cm_m16 m, cm_v4 f);
CM_DEF cm_m16 cm_scale_m16(cm_m16 m, cm_v4 v);
CM_DEF cm_m16 cm_orthogonal_m16(cm_f1 left, cm_f1 right, cm_f1 bottom, cm_f1 top, cm_f1 near, cm_f1 far);
CM_DEF cm_m16 cm_frustum_m16(cm_f1 left, cm_f1 right, cm_f1 bottom, cm_f1 top, cm_f1 znear, cm_f1 zfar);
CM_DEF cm_m16 cm_perspective_m16(cm_f1 fovyInDegrees, cm_f1 aspectRatio, cm_f1 znear, cm_f1 zfar);
CM_DEF cm_m16 cm_lookat_m16(cm_v4 position, cm_v4 target, cm_v4 upVector);

/* TODO utilize */
typedef cm_v4 cm_qt;

CM_DEF cm_qt  cm_conj_qt(cm_qt q);
CM_DEF cm_qt  cm_apply_qt(cm_qt q, cm_v4 v);
CM_DEF cm_qt  cm_new_qt(cm_v4 axis, cm_f1 angle);
CM_DEF cm_qt  cm_cum_qt(cm_qt a, cm_qt b);
CM_DEF cm_qt  cm_slerp_qt(cm_qt a, cm_qt b, cm_f1 t);
CM_DEF cm_m16 cm_qt_to_m16(cm_qt q);

#endif

#ifdef CM_IMPLEMENT_HERE

#include <math.h>
#include <string.h> /* For memcpy only. TODO get rid of this dependency */

#if CM_BACKEND == CM_BACKEND_SSE

CM_DEF cm_v4 cm_new_v4(cm_f1 a, cm_f1 b, cm_f1 c, cm_f1 d) {
	return _mm_setr_ps(a, b, c, d);
}

CM_DEF cm_v4 cm_send_v4(cm_f1 c[4]) {
	return _mm_setr_ps(c[0], c[1], c[2], c[3]);
}

CM_DEF cm_v4 cm_send1_v4(cm_f1 a) {
	return _mm_set1_ps(a);
}

CM_DEF cm_v4 cm_add_v4(cm_v4 l, cm_v4 r) {
	return _mm_add_ps(l, r);
}

CM_DEF cm_v4 cm_sub_v4(cm_v4 l, cm_v4 r) {
	return _mm_sub_ps(l, r);
}

CM_DEF cm_v4 cm_mul_v4(cm_v4 l, cm_v4 r) {
	return _mm_mul_ps(l, r);
}

#define cm_spread_v4(f, i) _mm_shuffle_ps(f, f, _MM_SHUFFLE(i, i, i, i))

CM_DEF void cm_recv_v4(cm_v4 f, cm_f1 o[4]) {
	CM_ALIGN16 cm_f1 i[4];
	_mm_store_ps(i, f);
	memcpy(o, i, sizeof(cm_f1[4]));
}

CM_DEF cm_m16 cm_transpose_m16(cm_m16 m) {
	cm_m16 o = m;
	_MM_TRANSPOSE4_PS(o.c[0], o.c[1], o.c[2], o.c[3]);
	return o;
}

CM_DEF cm_v4 cm_cross_v3(cm_v4 l, cm_v4 r) {
	cm_v4 lyzx = _mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 0, 2, 1));
	cm_v4 rzxy = _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 1, 0, 2));
	cm_v4 lzxy = _mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 1, 0, 2));
	cm_v4 ryzx = _mm_shuffle_ps(r, r, _MM_SHUFFLE(3, 0, 2, 1));
	return cm_sub_v4(cm_mul_v4(lyzx, rzxy), cm_mul_v4(ryzx, lzxy));
}

#else // CM_BACKEND == CM_BACKEND_FALLBACK

CM_DEF cm_v4 cm_new_v4(cm_f1 a, cm_f1 b, cm_f1 c, cm_f1 d) {
	return (cm_v4){{a, b, c, d}};
}

CM_DEF cm_v4 cm_send_v4(cm_f1 c[4]) {
	return (cm_v4){{c[0], c[1], c[2], c[3]}}; // TODO clean this up
}

CM_DEF cm_v4 cm_send1_v4(cm_f1 a) {
	return (cm_v4){{a, a, a, a}};
}

CM_DEF void cm_recv_v4(cm_v4 f, cm_f1 o[4]) {
	memcpy(o, f.c, sizeof(cm_f1[4]));
}

CM_DEF cm_v4 cm_add_v4(cm_v4 l, cm_v4 r) {
	return (cm_v4){{l.c[0]+r.c[0], l.c[1]+r.c[1], l.c[2]+r.c[2], l.c[3]+r.c[3]}};
}

CM_DEF cm_v4 cm_sub_v4(cm_v4 l, cm_v4 r) {
	return (cm_v4){{l.c[0]-r.c[0], l.c[1]-r.c[1], l.c[2]-r.c[2], l.c[3]-r.c[3]}};
}

CM_DEF cm_v4 cm_mul_v4(cm_v4 l, cm_v4 r) {
	return (cm_v4){{l.c[0]*r.c[0], l.c[1]*r.c[1], l.c[2]*r.c[2], l.c[3]*r.c[3]}};
}

CM_DEF cm_v4 cm_spread_v4(cm_v4 f, int i) {
	return (cm_v4){{f.c[i], f.c[i], f.c[i], f.c[i]}};
}

CM_DEF cm_v4 cm_cross_v3(cm_v4 l, cm_v4 r) {
	return (cm_v4){{
		l.c[1]*r.c[2] - l.c[2]*r.c[1],
		l.c[2]*r.c[0] - l.c[0]*r.c[2],
		l.c[0]*r.c[1] - l.c[1]*r.c[0]}};
}

CM_DEF cm_m16 cm_transpose_m16(cm_m16 m) {
	cm_m16 o;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
			o.c[j].c[i] = m.c[i].c[j];
	}
	return o;
}

CM_DEF cm_m16 cm_inverse_m16(cm_m16 mat) {
	/* Implementation lifted from OpenTK, but heavily modified for libcalm.
	 * https://svn.code.sf.net/p/opentk/code/trunk/Source/OpenTK/Math/Matrix4.cs
	 * Just to be safe, here's their license:
	 *
	 * Copyright (c) 2006 - 2008 The Open Toolkit library.
	 *
	 * Permission is hereby granted, free of charge, to any person obtaining a copy of
	 * this software and associated documentation files (the "Software"), to deal in
	 * the Software without restriction, including without limitation the rights to
	 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
	 * of the Software, and to permit persons to whom the Software is furnished to do
	 * so, subject to the following conditions:
	 *
	 * The above copyright notice and this permission notice shall be included in all
	 * copies or substantial portions of the Software.
	 *
	 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	 * SOFTWARE.
	 * */
	// TODO test whether this function accidentally
	// overwrites mat outside of its own scope.
	int colIdx[4] = {0}, rowIdx[4] = {0};
	int pivotIdx[4] = {-1, -1, -1, -1};
	int icol = 0, irow = 0;
	for (int i = 0; i < 4; i++) {
		// Find the largest pivot value
		cm_f1 maxPivot = 0.0f;
		for (int j = 0; j < 4; j++) {
			if (pivotIdx[j] != 0) {
				for (int k = 0; k < 4; ++k) {
					if (pivotIdx[k] == -1) {
						cm_f1 absVal = fabs(mat.c[j].c[k]);
						if (absVal > maxPivot) {
							maxPivot = absVal;
							irow = j, icol = k;
						}
					} else if (pivotIdx[k] > 0) {
						return cm_identity_m16();
					}
				}
			}
		}
		pivotIdx[icol]++;
		// Swap rows over so pivot is on diagonal
		if (irow != icol) {
			cm_v4 f = mat.c[irow];
			mat.c[irow] = mat.c[icol];
			mat.c[icol] = f;
		}
		rowIdx[i] = irow, colIdx[i] = icol;
		cm_f1 pivot = mat.c[icol].c[icol];
		// check for singular matrix
		if (pivot == 0.0f) {
			return cm_identity_m16();
		}
		// Scale row so it has a unit diagonal
		mat.c[icol].c[icol] = 1.0f;
		mat.c[icol] = cm_scale_v4(mat.c[icol], 1.0f / pivot);
		// Do elimination of non-diagonal elements
		for (int j = 0; j < 4; ++j) {
			// check this isn't on the diagonal
			if (icol != j) {
				cm_f1 f = mat.c[j].c[icol];
				mat.c[j].c[icol] = 0.0f;
				mat.c[j] = cm_sub_v4(mat.c[j], cm_scale_v4(mat.c[icol], f));
			}
		}
	}
	for (int j = 3; j >= 0; --j) {
		int ir = rowIdx[j], ic = colIdx[j];
		for (int k = 0; k < 4; ++k) {
			cm_f1 f = mat.c[k].c[ir];
			mat.c[k].c[ir] = mat.c[k].c[ic];
			mat.c[k].c[ic] = f;
		}
	}
	return mat;
}

CM_DEF cm_qt cm_cum_qt(cm_qt a, cm_qt b) {
	return (cm_qt) {{
		b.c[3] * a.c[0] + b.c[0] * a.c[3] + b.c[1] * a.c[2] - b.c[2] * a.c[1],
		b.c[3] * a.c[1] + b.c[1] * a.c[3] + b.c[2] * a.c[0] - b.c[0] * a.c[2],
		b.c[3] * a.c[2] + b.c[2] * a.c[3] + b.c[0] * a.c[1] - b.c[1] * a.c[0],
		b.c[3] * a.c[3] - b.c[0] * a.c[0] - b.c[1] * a.c[1] - b.c[2] * a.c[2]}};
}

#endif

/* ~~~~ 4D VECTORS ~~~~ */

CM_DEF cm_f1 cm_hsum_v4(cm_v4 f) {
	cm_f1 c[4];
	cm_recv_v4(f, c);
	return c[0] + c[1] + c[2] + c[3];
}
CM_DEF cm_v4 cm_scale_v4(cm_v4 f, cm_f1 s) {
	cm_v4 b = cm_send1_v4(s);
	return cm_mul_v4(f, b);
}
CM_DEF cm_f1 cm_dot_v4(cm_v4 l, cm_v4 r) {
	return cm_hsum_v4(cm_mul_v4(l, r));
}
CM_DEF cm_f1 cm_length_v4(cm_v4 v) {
	return sqrtf(cm_dot_v4(v, v));
}
CM_DEF cm_v4 cm_norm_v4(cm_v4 v) {
	cm_f1 length = cm_length_v4(v);
	return cm_scale_v4(v, 1.0 / length);
}

/* ~~~~ 3D VECTORS ~~~~ */

CM_DEF cm_f1 cm_dot_v3(cm_v4 l, cm_v4 r) {
	return cm_hsum_v3(cm_mul_v4(l, r));
}
CM_DEF cm_f1 cm_length_v3(cm_v4 v) {
	return sqrtf(cm_dot_v3(v, v));
}
CM_DEF cm_v4 cm_norm_v3(cm_v4 v) {
	cm_f1 length = cm_length_v3(v);
	return cm_scale_v4(v, 1.0 / length);
}
CM_DEF cm_f1 cm_hsum_v3(cm_v4 f) {
	cm_f1 c[4];
	cm_recv_v4(f, c);
	return c[0] + c[1] + c[2];
}

/* ~~~~ 4x4 MATRICES ~~~~ */

CM_DEF cm_m16 cm_identity_m16(void) {
	cm_m16 m;
	m.c[0] = cm_new_v4(1.0, 0.0, 0.0, 0.0);
	m.c[1] = cm_new_v4(0.0, 1.0, 0.0, 0.0);
	m.c[2] = cm_new_v4(0.0, 0.0, 1.0, 0.0);
	m.c[3] = cm_new_v4(0.0, 0.0, 0.0, 1.0);
	return m;
}

CM_DEF cm_m16 cm_new_m16(cm_v4 a, cm_v4 b, cm_v4 c, cm_v4 d) {
	return (cm_m16){{a, b, c, d}};
}

CM_DEF cm_m16 cm_send_m16(cm_f1 f[16])
{
	return (cm_m16){{
		cm_send_v4(&f[ 0]),
		cm_send_v4(&f[ 4]),
		cm_send_v4(&f[ 8]),
		cm_send_v4(&f[12])}};
}


CM_DEF void cm_recv_m16(cm_m16 m, cm_f1 c[16]) {
	/* TODO maybe optimize? */
	cm_recv_v4(m.c[0], &c[0]);
	cm_recv_v4(m.c[1], &c[4]);
	cm_recv_v4(m.c[2], &c[8]);
	cm_recv_v4(m.c[3], &c[12]);
}

CM_DEF cm_m16 cm_add_m16(cm_m16 l, cm_m16 r) {
	cm_m16 o;
	for (int i = 0; i < 4; i++)
		o.c[i] = cm_add_v4(l.c[i], r.c[i]);
	return o;
}

CM_DEF cm_m16 cm_sub_m16(cm_m16 l, cm_m16 r) {
	cm_m16 o;
	for (int i = 0; i < 4; i++)
		o.c[i] = cm_sub_v4(l.c[i], r.c[i]);
	return o;
}

CM_DEF cm_v4 cm_apply_m16(cm_m16 m, cm_v4 f) {
	cm_v4 c0 = cm_mul_v4(m.c[0], cm_spread_v4(f, 0));
	cm_v4 c1 = cm_mul_v4(m.c[1], cm_spread_v4(f, 1));
	cm_v4 c2 = cm_mul_v4(m.c[2], cm_spread_v4(f, 2));
	cm_v4 c3 = cm_mul_v4(m.c[3], cm_spread_v4(f, 3));
	return cm_add_v4(cm_add_v4(c0, c1), cm_add_v4(c2, c3));
}

CM_DEF cm_m16 cm_dot_m16(cm_m16 l, cm_m16 r) {
	cm_m16 o;
	for (int c = 0; c < 4; c++) {
		o.c[c] =                   cm_mul_v4(l.c[0], cm_spread_v4(r.c[c], 0));
		o.c[c] = cm_add_v4(o.c[c], cm_mul_v4(l.c[1], cm_spread_v4(r.c[c], 1)));
		o.c[c] = cm_add_v4(o.c[c], cm_mul_v4(l.c[2], cm_spread_v4(r.c[c], 2)));
		o.c[c] = cm_add_v4(o.c[c], cm_mul_v4(l.c[3], cm_spread_v4(r.c[c], 3)));
	}
	return o;
}

CM_DEF cm_m16 cm_translate_m16(cm_m16 m, cm_v4 f) {
	cm_m16 o = m; /* TODO check whether this really copies the underlying __m128's. */
	o.c[3] = cm_add_v4(m.c[3], f);
	return o;
}

CM_DEF cm_m16 cm_scale_m16(cm_m16 m, cm_v4 v) {
	/* TODO optimize! */
	float f[4];
	cm_recv_v4(v, f);
	cm_m16 o;
	cm_v4 s;
	s = cm_new_v4(f[0], 0.0f, 0.0f, 0.0f);
	o.c[0] = cm_mul_v4(m.c[0], s);
	s = cm_new_v4(0.0f, f[1], 0.0f, 0.0f);
	o.c[1] = cm_mul_v4(m.c[1], s);
	s = cm_new_v4(0.0f, 0.0f, f[2], 0.0f);
	o.c[2] = cm_mul_v4(m.c[2], s);
	o.c[3] = m.c[3];
	return o;
}

CM_DEF cm_m16 cm_orthogonal_m16(cm_f1 left, cm_f1 right, cm_f1 bottom, cm_f1 top, cm_f1 near, cm_f1 far) {
    cm_v4 a = cm_new_v4( 2.0f / (right - left), 0.0f, 0.0f, 0.0f);
	cm_v4 b = cm_new_v4(0.0f, 2.0f / (top - bottom), 0.0f, 0.0f);
    cm_v4 c = cm_new_v4(0.0f, 0.0f, -2.0f / (far - near), 0.0f);
    cm_f1 tx = -(right + left) / (right - left);
    cm_f1 ty = -(top + bottom) / (top - bottom);
    cm_f1 tz = -(far + near) / (far - near);
	cm_v4 t = cm_new_v4(tx, ty, tz, 1.0f);
	return cm_new_m16(a, b, c, t);
}

CM_DEF cm_m16 cm_frustum_m16(cm_f1 left, cm_f1 right, cm_f1 bottom, cm_f1 top, cm_f1 znear, cm_f1 zfar) {
    cm_f1 znear2 = 2.0 * znear;
    cm_f1 width  = right - left;
    cm_f1 height = top - bottom;
    cm_f1 depth  = zfar - znear;
	cm_m16 matrix;
	matrix.c[0] = cm_new_v4(znear2 / width, 0.0f, 0.0f, 0.0f);
	matrix.c[1] = cm_new_v4(0.0f, znear2 / height, 0.0f, 0.0f);
	matrix.c[2] = cm_new_v4((right + left) / width, (top + bottom) / height, (-zfar - znear) / depth, -1.0f);
	matrix.c[3] = cm_new_v4(0.0f, 0.0f, (-znear2 * zfar) / depth, 0.0f);
	return matrix;
}

CM_DEF cm_m16 cm_perspective_m16(cm_f1 fovyInDegrees, cm_f1 aspectRatio, cm_f1 znear, cm_f1 zfar) {
    cm_f1 ymax = znear * tanf(fovyInDegrees * CM_PI / 360.0f);
    cm_f1 xmax = ymax * aspectRatio;
    return cm_frustum_m16(-xmax, xmax, -ymax, ymax, znear, zfar);
}

CM_DEF cm_m16 cm_lookat_m16(cm_v4 position, cm_v4 target, cm_v4 upVector) {
	const cm_v4 f = cm_norm_v3(cm_sub_v4(target, position));
	const cm_v4 s = cm_norm_v3(cm_cross_v3(f, upVector));
	const cm_v4 u = cm_cross_v3(s, f);
	cm_m16 matrix = cm_new_m16(s, u, cm_scale_v4(f, -1.0f), cm_send1_v4(0.0f));
	matrix = cm_transpose_m16(matrix);
	cm_f1 sp = cm_dot_v3(s, position);
	cm_f1 up = cm_dot_v3(u, position);
	cm_f1 fp = cm_dot_v3(f, position);
	matrix.c[3] = cm_new_v4(-sp, -up, fp, 1.0f);
	return matrix;
}

/* ~~~~ QUATERNIONS ~~~~ */

CM_DEF cm_qt cm_conj_qt(cm_qt q) {
	return cm_mul_v4(q, cm_new_v4(-1.0, -1.0, -1.0, 1.0));
}

CM_DEF cm_qt cm_apply_qt(cm_qt q, cm_v4 v) {
	// Optimized quaternion application by Fabian Giesen
	cm_f1 c[4];
	cm_recv_v4(q, c);
	cm_v4 p = *(cm_v4*)&q;
	cm_v4 t = cm_scale_v4(cm_cross_v3(p, v), 2.0f);
	return cm_add_v4(cm_add_v4(v, cm_scale_v4(t, c[3])), cm_cross_v3(p, t));
}

CM_DEF cm_qt cm_new_qt(cm_v4 axis, cm_f1 angle) {
	/* TODO optimize */
	cm_f1 h = angle / 2.0f;
	cm_f1 s = sinf(h);
	cm_f1 c[4];
	cm_recv_v4(axis, c);
	return cm_new_v4(c[0] * s, c[1] * s, c[2] * s, cos(h));
}

#if 0
CM_DEF cm_qt cm_cum_qt(cm_qt a, cm_qt b) {
	/* TODO optimize! */
	cm_f1 ac[4], bc[4];
	cm_recv_v4(a, ac);
	cm_recv_v4(b, bc);
	cm_v4 a3 = cm_new_v4(ac[0], ac[1], ac[2], 0.0f);
	cm_v4 b3 = cm_new_v4(bc[0], bc[1], bc[2], 0.0f);
	cm_f1 ws = ac[3] * bc[3] - cm_dot_v3(a3, b3);
	cm_v4 w = cm_new_v4(0.0f, 0.0f, 0.0f, ws);
	cm_v4 as = cm_scale_v4(a3, bc[3]);
	cm_v4 bs = cm_scale_v4(b3, ac[3]);
	cm_v4 v = cm_add_v4(cm_cross_v3(a3, b3), cm_add_v4(as, bs));
	return cm_add_v4(v, w);
}
#endif

CM_DEF cm_qt cm_slerp_qt(cm_qt a, cm_qt b, cm_f1 t) {
	// Calculate angle between a and b
	cm_f1 cosHalfTheta = cm_dot_v4(a, b);
	// if a = b or a = -b then theta = 0 and we can return a
	if (fabs(cosHalfTheta) >= 1.0)
		return a;
	// Calculate temporary values
	cm_f1 halfTheta = acos(cosHalfTheta);
	cm_f1 sinHalfTheta = sqrt(1.0 - cosHalfTheta * cosHalfTheta);
	// if theta = 180 degrees then result is not fully defined
	// we could rotate around any axis normal to a or b
	if (fabs(sinHalfTheta) < 0.001)
		return cm_add_v4(cm_scale_v4(a, 0.5f), cm_scale_v4(b, 0.5f));
	cm_f1 ratioA = sin((1.0 - t) * halfTheta) / sinHalfTheta;
	cm_f1 ratioB = sin(       t  * halfTheta) / sinHalfTheta;
	// Calculate quaternion
	return cm_add_v4(cm_scale_v4(a, ratioA), cm_scale_v4(b, ratioB));
}

CM_DEF cm_m16 cm_qt_to_m16(cm_qt q) {
	/* TODO optimize? */
	cm_f1 c[4];
	cm_recv_v4(q, c);
	cm_m16 a = cm_new_m16(
		cm_new_v4( c[3], -c[2],  c[1], -c[0]),
		cm_new_v4( c[2],  c[3], -c[0], -c[1]),
		cm_new_v4(-c[1],  c[0],  c[3], -c[2]),
		cm_new_v4( c[0],  c[1],  c[2],  c[3])
	);
	cm_m16 b = cm_new_m16(
		cm_new_v4( c[3], -c[2],  c[1],  c[0]),
		cm_new_v4( c[2],  c[3], -c[0],  c[1]),
		cm_new_v4(-c[1],  c[0],  c[3],  c[2]),
		cm_new_v4(-c[0], -c[1], -c[2],  c[3])
	);
	return cm_dot_m16(a, b);
}

#endif

