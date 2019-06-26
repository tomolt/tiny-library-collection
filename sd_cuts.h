/* sd_cuts.h - Somewhat Different C Unit Testing System
 * 
 * MIT License
 *
 * Copyright (c) 2018, 2019 Thomas Oltmann
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

#ifndef SD_CUTS_H
#define SD_CUTS_H

#include <setjmp.h>

enum sd_execmodel_ {
	sd_sequential,
	sd_resilient,
	/* sd_parallel NYI */
};

struct sd_branchsaves_ {
	int saved_depth;
	int saved_model;
	void *saved_jmp;
};

extern enum sd_execmodel_ sd_execmodel;

void sd_init(void);

void sd_summarize(void);
void sd_query(int *errors, int *crashes);

void sd_push(char const *format, ...);
void sd_pop(void);

#define sd_branch(code) { \
		struct sd_branchsaves_ s; \
		sigjmp_buf my_jmp; \
		int signal = sigsetjmp(my_jmp, 1); \
		sd_branchbeg_(signal, &my_jmp, &s); \
		if (!signal) { \
			code \
		} \
		sd_branchend_(&s); \
	}

#define SD_EPSILON 0.00001

#define sd_throw(format, ...) sd_throw_(__LINE__, format, __VA_ARGS__)
#define sd_assert(cond) sd_assert_(cond, #cond, __LINE__)
#define sd_assertiq(a, b) sd_assertiq_(a, b, #a "==" #b, __LINE__)
#define sd_assertfq(a, b) sd_assertfq_(a, b, SD_EPSILON, #a "==" #b, __LINE__)
#define sd_assertsq(a, b) sd_assertsq_(a, b, #a "==" #b, __LINE__)
#define sd_asserteq(a, b, e) sd_assertfq_(a, b, e, #a "==" #b, __LINE__)

/* internal functions that have to be visible. */
/* do not call these directly. */
void sd_throw_(int ln, char const *format, ...);
void sd_assert_(int cond, char const *str, int ln);
void sd_assertiq_(long long a, long long b, char const *str, int ln);
void sd_assertfq_(double a, double b, double e, char const *str, int ln);
void sd_assertsq_(char const *a, char const *b, char const *str, int ln);
void sd_branchbeg_(int signal, sigjmp_buf *my_jmp, struct sd_branchsaves_ *s);
void sd_branchend_(struct sd_branchsaves_ *s);

#endif

#ifdef SD_IMPLEMENT_HERE

/* TODO make thread-safe */

#ifdef SD_ASCII_ONLY

# define TEXT_DOTS  ".."
# define TEXT_HIER  "\\ "
# define TEXT_ARROW "<-"
# define TEXT_LINE  "--"

#else

# define TEXT_DOTS  "\u2024\u2024"
# define TEXT_HIER  "\u2514 "
# define TEXT_ARROW "\u2190"
# define TEXT_LINE  "\u2500\u2500"

#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

/* dependencies of the resilient execution model */
#include <signal.h>
#include <float.h>

#define MAX_NAME_LENGTH 200
#define MAX_DEPTH 50

enum sd_execmodel_ sd_execmodel = sd_resilient;

struct sd_T {
	int StackDepth;
	int PrintDepth;
	int ErrorCount;
	int CrashCount;
	sigjmp_buf *CrashJmp;
	char const *Stack[MAX_DEPTH];
};

static struct sd_T sd_T;

static char const *name_of_signal(int signal)
{
	switch (signal) {
		case SIGFPE: return "SIGFPE";
		case SIGILL: return "SIGILL";
		case SIGSEGV: return "SIGSEGV";
		/* the default path should never be taken, */
		/* as only the above signals are actually caught. */
		default: return "unknown signal";
	}
}

static void signal_handler(int signal)
{
	switch (sd_execmodel) {
		case sd_sequential:
			SIG_DFL(signal);
			break;
		case sd_resilient:
			if (sd_T.CrashJmp != NULL) {
				if (signal == SIGFPE) {
					/* source: https://msdn.microsoft.com/en-us/library/xdkz3x12.aspx */
					/* _fpreset(); TODO */
				}
				/* signal will never be 0, so we can pass it */
				/* directly to longjmp without hesitation. */
				/* source: /usr/include/bits/signum-generic.h */
				siglongjmp(*sd_T.CrashJmp, signal);
			} else {
				/* if there is no recovery point, we can't do anything about the signal. */
				/* this situation should not arise during normal operation. */
				SIG_DFL(signal);
			}
			break;
	}
}

static void print_trace(void)
{
	int depth = sd_T.PrintDepth;
	while (depth < sd_T.StackDepth) {
		for (int i = 0; i < depth; ++i)
			fputs(TEXT_DOTS, stdout);
		fputs(TEXT_HIER, stdout);
		puts(sd_T.Stack[depth]);
		++depth;
	}
	sd_T.PrintDepth = sd_T.StackDepth;
}

void sd_init(void)
{
	/* signal() may fail, but there is nothing we can do about it. */
	signal(SIGFPE, signal_handler);
	signal(SIGILL, signal_handler);
	signal(SIGSEGV, signal_handler);
}

void sd_query(int *errors, int *crashes)
{
	if (errors != NULL) {
		*errors = sd_T.ErrorCount;
	}
	if (crashes != NULL) {
		*crashes = sd_T.CrashCount;
	}
}

void sd_summarize(void)
{
	printf(TEXT_LINE " %d failures, %d crashes " TEXT_LINE "\n", sd_T.ErrorCount, sd_T.CrashCount);
}

void sd_push(char const *format, ...)
{
	char *str = malloc(MAX_NAME_LENGTH);
	va_list va;
	va_start(va, format);
	vsnprintf(str, MAX_NAME_LENGTH, format, va);
	va_end(va);
	sd_T.Stack[sd_T.StackDepth++] = str;
}

void sd_pop(void)
{
	free((char *)sd_T.Stack[--sd_T.StackDepth]);
	if (sd_T.PrintDepth > sd_T.StackDepth)
		--sd_T.PrintDepth;
}

void sd_branchbeg_(int signal, sigjmp_buf *my_jmp, struct sd_branchsaves_ *s)
{
	if (signal) {
		++sd_T.CrashCount;
		char const *cause = name_of_signal(signal);
		sd_push("<%s>\t\t" TEXT_ARROW " CRASH\n", cause);
		print_trace();
		sd_pop();
	} else {
		*s = (struct sd_branchsaves_){sd_T.StackDepth, sd_execmodel, (void *)sd_T.CrashJmp};
		sd_T.CrashJmp = my_jmp;
	}
}

void sd_branchend_(struct sd_branchsaves_ *s)
{
	sd_T.CrashJmp = s->saved_jmp;
	/* restore the stack in case of a crash. */
	/* also helps recovering from missing sd_pop()'s, */
	/* though you *really* shouldn't rely on this behaviour. */
	while (sd_T.StackDepth > s->saved_depth)
		sd_pop();
	/* restore the execmodel to allow nesting of branches */
	/* of differing execmodels.*/
	/* you may rely on this behaviour. */
	sd_execmodel = s->saved_model;
}

void sd_throw_(int ln, char const *format, ...)
{
	++sd_T.ErrorCount;
	char *str = malloc(MAX_NAME_LENGTH);
	va_list va;
	va_start(va, format);
	vsnprintf(str, MAX_NAME_LENGTH, format, va);
	va_end(va);
	sd_push("<throw> L%03d: %s\t\t" TEXT_ARROW " FAIL\n", ln, str);
	free(str);
	print_trace();
	sd_pop();
}

void sd_assert_(int cond, char const *str, int ln)
{
	if (!cond) {
		++sd_T.ErrorCount;
		sd_push("<assert> L%03d: %s\t\t" TEXT_ARROW " FAIL\n", ln, str);
		print_trace();
		sd_pop();
	}
}

void sd_assertiq_(long long a, long long b, char const *str, int ln)
{
	sd_assert_(a == b, str, ln);
}

void sd_assertfq_(double a, double b, double e, char const *str, int ln)
{
	double d = a - b;
	if (d < 0.0) d = -d;
	sd_assert_(d <= e, str, ln);
}

void sd_assertsq_(char const *a, char const *b, char const *str, int ln)
{
	sd_assert_(strcmp(a, b) != 0, str, ln);
}

#endif
