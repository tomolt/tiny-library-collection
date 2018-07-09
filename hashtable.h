/* hashtable.h - A Robin Hood-hashing hashtable implementation
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

#ifdef HASHTABLE_H
#error MULTIPLE INCLUSION
#endif
#define HASHTABLE_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

struct HT_key
{
	char const *name;
	short length;
	uint32_t hash;
};

struct HT
{
	size_t cap;
	int eSize;
	int fill;
	struct HT_key *keys;
	char *values;
};

struct HT htNew(size_t cap, int eSize);
void htFree(struct HT *ht);
void htSet(struct HT *ht, void const *name, short length, void *value);
void htDel(struct HT *ht, void const *name, short length);
bool htHas(struct HT *ht, void const *name, short length);
void *htGet(struct HT *ht, void const *name, short length);

