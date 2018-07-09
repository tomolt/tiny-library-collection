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

#ifndef HASHTABLE_H
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

#endif

#ifdef HT_IMPLEMENT_HERE

#include "hashtable.h"

#include <stdlib.h>
#include <string.h>

static double const load_factor = 0.8;

static void memswap(void *a, void *b, size_t size)
{
	char t[size];
	memcpy(t, a, size);
	memcpy(a, b, size);
	memcpy(b, t, size);
}

static uint32_t hash_func(char const *data, short length)
{
	uint32_t hash = 33;
	for (short i = 0; i < length; ++i) {
		hash += data[i];
		hash += hash << 10;
		hash ^= hash >> 6;
	}
	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;
	return hash;
}

struct search_result { bool found; int slot; };

static int fold_slot(struct HT *ht, uint32_t slot)
{ return slot % ht->cap; }

static int distance(struct HT *ht, struct HT_key key, int slot)
{ int d = slot - fold_slot(ht, key.hash); if (d < 0) { d += ht->cap; } return d; }

static int advance(struct HT *ht, int slot)
{ return fold_slot(ht, slot + 1); }

static bool does_match(struct HT_key a, struct HT_key b)
{ return a.hash == b.hash && a.length == b.length && memcmp(a.name, b.name, a.length) == 0; }

static void mark_tombstone(struct HT_key *key)
{ key->name = NULL; }

static bool is_tombstone(struct HT_key key)
{ return key.name == NULL; }

static struct HT_key make_key(char const *name, short length)
{ return (struct HT_key){name, length, hash_func(name, length)}; }

static char *value_at(struct HT *ht, int slot)
{ return &ht->values[slot * ht->eSize]; }

static int evict(struct HT *ht, struct HT_key key, int slot)
{
	return distance(ht, key, slot) > distance(ht, ht->keys[slot], slot) ? slot
		: evict(ht, key, advance(ht, slot));
}

static void insert_at(struct HT *ht, struct HT_key key, void *value, int slot)
{
	memswap(&ht->keys[slot], &key, sizeof(key));
	memswap(value_at(ht, slot), value, ht->eSize);
	if (!is_tombstone(key))
		insert_at(ht, key, value, evict(ht, key, advance(ht, slot)));
}

static struct search_result locate_at(struct HT *ht, struct HT_key key, int slot)
{
	if (does_match(ht->keys[slot], key))
		return (struct search_result){true, slot};
	else if (distance(ht, key, slot) > distance(ht, ht->keys[slot], slot))
		return (struct search_result){false, slot};
	else
		return locate_at(ht, key, advance(ht, slot));
}

static struct search_result locate(struct HT *ht, struct HT_key key)
{ return locate_at(ht, key, fold_slot(ht, key.hash)); }

static void rebuild(struct HT *ht, size_t cap)
{
	struct HT new = htNew(cap, ht->eSize);
	new.fill = ht->fill;
	for (size_t i = 0; i < ht->cap; ++i) {
		struct HT_key key = ht->keys[i];
		if (!is_tombstone(key)) {
			int slot = evict(&new, key, fold_slot(&new, key.hash));
			insert_at(&new, key, value_at(ht, i), slot);
		}
	}
	htFree(ht);
	memcpy(ht, &new, sizeof(new));
}

struct HT htNew(size_t cap, int eSize)
{
	struct HT ht = {cap, eSize, 0, NULL, NULL};
	ht.keys = calloc(cap, sizeof(*ht.keys));
	ht.values = calloc(cap, eSize);
	for (unsigned int i = 0; i < cap; ++i)
		ht.keys[i].hash = i;
	return ht;
}

void htFree(struct HT *ht)
{
	free(ht->keys);
	free(ht->values);
}

void htSet(struct HT *ht, void const *name, short length, void *value)
{
	if ((double)(ht->fill + 1) / (double)ht->cap > load_factor)
		rebuild(ht, ht->cap * 2);
	struct HT_key key = make_key(name, length);
	struct search_result search = locate(ht, key);
	if (search.found) {
		memcpy(value_at(ht, search.slot), value, ht->eSize);
	} else {
		++ht->fill;
		char buf[ht->eSize];
		memcpy(buf, value, ht->eSize);
		insert_at(ht, key, buf, search.slot);
	}
}

void htDel(struct HT *ht, void const *name, short length)
{
	struct search_result search = locate(ht, make_key(name, length));
	if (search.found) {
		mark_tombstone(&ht->keys[search.slot]);
		--ht->fill;
		if ((double)ht->fill / (double)ht->cap < 0.5 * load_factor)
			rebuild(ht, ht->cap / 2);
	}
}

bool htHas(struct HT *ht, void const *name, short length)
{
	return locate(ht, make_key(name, length)).found;
}

void *htGet(struct HT *ht, void const *name, short length)
{
	struct search_result search = locate(ht, make_key(name, length));
	return search.found ? value_at(ht, search.slot) : NULL;
}

#endif
