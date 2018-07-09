/* sd_cuts.h - Somewhat Different C Unit Testing System
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

#ifndef SD_CUTS_H
#define SD_CUTS_H

void sd_init(void);
void sd_summarize(void);
int sd_is_debug(void);

void sd_push(char const *format, ...);
void sd_pop(void);

void sd_branchv(void (*func)(void));
void sd_branchvp(void (*func)(void *), void *ud);

void sd_throw_(char const *str, int ln);
void sd_assert_(int cond, char const *str, int ln);
void sd_assertiq_(long long a, long long b, char const *str, int ln);
void sd_assertfq_(double a, double b, double e, char const *str, int ln);
void sd_assertsq_(char const *a, char const *b, char const *str, int ln);

#define sd_assert(cond) sd_assert_(cond, #cond, __LINE__);

#define SD_EPSILON 0.00001
#define sd_assertiq(a, b) sd_assertiq_(a, b, #a "==" #b, __LINE__);
#define sd_assertfq(a, b) sd_assertfq_(a, b, SD_EPSILON, #a "==" #b, __LINE__);
#define sd_assertsq(a, b) sd_assertsq_(a, b, #a "==" #b, __LINE__);
#define sd_asserteq(a, b, e) sd_assertfq_(a, b, e, #a "==" #b, __LINE__);

#endif

