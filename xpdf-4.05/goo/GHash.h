//========================================================================
//
// GHash.h
//
// Copyright 2001-2003 Glyph & Cog, LLC
//
//========================================================================

#ifndef GHASH_H
#define GHASH_H

#include <aconf.h>

#include "gtypes.h"

class GString;
struct GHashBucket;
struct GHashIter;

//------------------------------------------------------------------------

class GHash {
public:

  GHash(GBool deleteKeysA = gFalse);
  ~GHash();
  void add(GString *key, void *val);
  void add(GString *key, int val);
  void replace(GString *key, void *val);
  void replace(GString *key, int val);
  void *lookup(GString *key);
  int lookupInt(GString *key);
  void *lookup(const char *key);
  int lookupInt(const char *key);
  void *remove(GString *key);
  int removeInt(GString *key);
  void *remove(const char *key);
  int removeInt(const char *key);
  int getLength() const { return len; }
  void startIter(GHashIter **iter);
  GBool getNext(GHashIter **iter, GString **key, void **val) const;
  GBool getNext(GHashIter **iter, GString **key, int *val) const;
  void killIter(GHashIter **iter);

private:

  void expand();
  GHashBucket *find(GString *key, int *h);
  GHashBucket *find(const char *key, int *h);
  int hash(GString *key) const;
  int hash(const char *key) const;

  GBool deleteKeys;		// set if key strings should be deleted
  int size;			// number of buckets
  int len;			// number of entries
  GHashBucket **tab;
};

#define deleteGHash(hash, T)                       \
  do {                                             \
    GHash *_hash = (hash);                         \
    {                                              \
      GHashIter *_iter;                            \
      GString *_key;                               \
      void *_p;                                    \
      _hash->startIter(&_iter);                    \
      while (_hash->getNext(&_iter, &_key, &_p)) { \
        delete (T*)_p;                             \
      }                                            \
      delete _hash;                                \
    }                                              \
  } while(0)

#endif
