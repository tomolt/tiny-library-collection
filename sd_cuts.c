/* sd_cuts.c - Somewhat Different C Unit Testing System
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

#include "sd_cuts.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define MAX_NAME_LENGTH 200
#define MAX_DEPTH 50

static char const *Stack[MAX_DEPTH];
static int StackDepth;
static int PrintDepth;
static int ErrorCount;

static void print_trace(void)
{
	int depth = PrintDepth;
	while (depth < StackDepth) {
		for (int i = 0; i < depth; ++i)
			fputs("  ", stdout);
		fputs("\\ ", stdout);
		puts(Stack[depth]);
		++depth;
	}
	PrintDepth = StackDepth;
}

void sd_init(void)
{
	
}

void sd_summarize(void)
{
	printf("-- %d failures --\n", ErrorCount);
}

int sd_is_debug(void)
{
#ifdef SD_DEBUG
	return 1;
#else
	return 0;
#endif
}

void sd_push(char const *format, ...)
{
	va_list va;
	char *str = malloc(MAX_NAME_LENGTH);
	va_start(va, format);
	vsnprintf(str, MAX_NAME_LENGTH, format, va);
	va_end(va);
	Stack[StackDepth++] = str;
}

void sd_pop(void)
{
	free((char *)Stack[--StackDepth]);
	if (PrintDepth > StackDepth)
		--PrintDepth;
}

void sd_branchv(void (*func)(void))
{
	func();
}

void sd_branchvp(void (*func)(void *), void *ud)
{
	func(ud);
}

void sd_throw_(char const *str, int ln)
{
	++ErrorCount;
	sd_push("<assert> L%03d: %s\t\t<- FAIL\n", ln, str);
	print_trace();
	sd_pop();
}

void sd_assert_(int cond, char const *str, int ln)
{
	if (!cond) sd_throw_(str, ln);
}

void sd_assertiq_(long long a, long long b, char const *str, int ln)
{
	if (a != b) sd_throw_(str, ln);
}

void sd_assertfq_(double a, double b, double e, char const *str, int ln)
{
	double d = a - b;
	if (d < 0.0) d = -d;
	if (d > e) sd_throw_(str, ln);
}

void sd_assertsq_(char const *a, char const *b, char const *str, int ln)
{
	if (strcmp(a, b) != 0) sd_throw_(str, ln);
}

