#include "KvGlobal.h"

void kv_assert(const char *assertion, const char *file, int line)
{
	kFatal("ASSERT: \"%s\" in file %s, line %d", assertion, file, line);
}

void kv_assert_x(const char *where, const char *what, const char *file, int line)
{
	kFatal("ASSERT failure in %s: \"%s\", file %s, line %d", where, what, file, line);
}

void kFatal(const char *, ...) 
{

}

void kCritical(const char *, ...) 
{

}

void kWarning(const char *, ...) 
{

}

void kDebug(const char *, ...) 
{

}
