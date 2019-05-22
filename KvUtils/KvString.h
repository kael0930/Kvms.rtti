#ifndef KvString_h__
#define KvString_h__

#include "KvUtils.h"

class KV_UTILS_EXPORT KvString
{
public:
	KvString();
	KvString(const KvString &s);
	explicit KvString(const char *s);
	~KvString();

	KvString& operator=(const char *s);
	KvString& operator=(const KvString &s);

	KvString& operator+=(char c);
	KvString& operator+=(const char *s);
	KvString& operator+=(const KvString &s);

	bool operator==(const KvString &s) const;
	KV_INLINE bool operator==(const char *s) const;
	KV_INLINE bool operator!=(const KvString &s) const { return !operator==(s); }
	
	KV_INLINE const char *latin1() const { return data; }

	KV_INLINE const char *get() const { return data; }
	KV_INLINE operator const char*() const { return data; }
	KV_INLINE operator const void*() const { return data; }

	KV_INLINE char &get(int index) {
		assert((unsigned int)index < (unsigned int)length && "String::get(): bad index");
		return data[index];
	}
	KV_INLINE char get(int index) const {
		assert((unsigned int)index < (unsigned int)length && "String::get(): bad index");
		return data[index];
	}

	KV_INLINE char &operator[](int index) {
		assert((unsigned int)index < (unsigned int)length && "String::operator[](): bad index");
		return data[index];
	}
	KV_INLINE char operator[](int index) const {
		assert((unsigned int)index < (unsigned int)length && "String::operator[](): bad index");
		return data[index];
	}

	KV_INLINE int size() const { return length; }
	KV_INLINE int isEmpty() const { return (length == 0); }
	KV_INLINE bool isNull() const { return (length == 0); }

	void resize(int size);
	void allocate(int size);
	void reserve(int size);

	void clear();
	void destroy();

	void copy(const char *s, int len = -1);
	void copy(const KvString &s, int len = -1);

	void append(char c);
	void append(const char *s, int len = -1);
	void append(const KvString &s, int len = -1);

	void append(int pos, char c);
	void append(int pos, const char *s, int len = -1);
	void append(int pos, const KvString &s, int len = -1);

	void remove();
	void remove(int pos, int len = 1);

	void lower();
	void upper();

	static const KvString null;

protected:
	void do_copy(const char *s, int len);
	void do_append(int pos, char c);
	void do_append(int pos, const char *s, int len);
	void do_memcpy(char *dest, const char *src, int size);

protected:
	int length;
	int capacity;
	char *data;

	enum {
		CAPACITY = 4,
		DYNAMIC_FLAG = (int)(1 << (sizeof(int)* 8 - 1)),
		CAPACITY_MASK = ~(int)DYNAMIC_FLAG,
	};

	char stack_data[CAPACITY];
};

inline bool kvStringComparisonHelper(const KvString &s1, const char *s2)
{
	return (s1 == KvString(s2));
}
inline bool KvString::operator==(const char *s) const
{
	return kvStringComparisonHelper(*this, s);
}

inline KV_UTILS_EXPORT bool operator==(const char *s1, const KvString &s2)
{
	return kvStringComparisonHelper(s2, s1);
}

#include <string>
#include <list>

//#define KvString std::string
#define KvStringList std::list<KvString>

#endif // KvString_h__
