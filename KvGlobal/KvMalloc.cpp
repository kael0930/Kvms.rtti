#include "KvPlatformdefs.h"

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdarg.h>
#include <string>
#include <exception>

void kv_check_pointer(const char *n, int l)
{
	printf("In file %s, line %d: Out of memory", n, l);
}

void kvBadAlloc()
{
	KV_THROW(std::bad_alloc());
}

void *kvMalloc(size_t size)
{
	return ::malloc(size);
}

void kvFree(void *ptr)
{
	::free(ptr);
}

void *kvRealloc(void *ptr, size_t size)
{
	return ::realloc(ptr, size);
}


void kvFreeAligned(void *ptr)
{
	if (!ptr)
		return;
	void **ptr2 = static_cast<void **>(ptr);
	free(ptr2[-1]);
}