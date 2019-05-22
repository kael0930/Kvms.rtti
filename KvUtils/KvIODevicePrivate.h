#ifndef KvIODevicePrivate_h__
#define KvIODevicePrivate_h__

#include "KvUtils.h"
#include "KvString.h"
#include "KvNameSpace.h"

#ifndef IODEVICE_BUFFERSIZE
#define IODEVICE_BUFFERSIZE K_INT64_C(16384)
#endif

class KvLinearBuffer
{
	// length of the unread data
	int length;
	// start of the unread data
	char* first;
	// the allocated buffer
	char* data;
	// allocated buffer size
	qint64 capacity;

public:
	KvLinearBuffer(int) : length(0), first(0), data(0), capacity(0) {
	}
	~KvLinearBuffer() {
		delete[] data;
	}

	KV_INLINE const char *get() const { return data; }
	KV_INLINE operator const char*() const { return data; }
	KV_INLINE operator const void*() const { return data; }

	KV_INLINE char get(int index) const {
		assert((unsigned int)index < (unsigned int)length && "StreamString::get(): bad index");
		return data[index];
	}

	KV_INLINE char operator[](int index) const {
		assert((unsigned int)index < (unsigned int)length && "StreamString::operator[](): bad index");
		return data[index];
	}

	void clear(){
		first = data;
		length = 0;
	}
	int size() const{
		return length;
	}
	bool isEmpty() const{
		return length == 0;
	}

	void skip(int n){
		if (n >= length)
		{
			clear();
		}
		else
		{
			length -= n;
			first += n;
		}
	}
	int getChar() {
		if (length == 0)
			return -1;
		int ch = uchar(*first);
		length--;
		first++;
		return ch;
	}
	int read(char* target, int size) {
		int r = kvMin(size, length);
		memcpy(target, first, r);
		length -= r;
		first += r;
		return r;
	}
	int peek(char* target, int size) {
		int r = kvMin(size, length);
		memcpy(target, first, r);
		return r;
	}
	char* reserve(int size) {
		makeSpace(size + length, freeSpaceAtEnd);
		char* writePtr = first + length;
		length += size;
		return writePtr;
	}
	void chop(int size) {
		if (size >= length) {
			clear();
		}
		else {
			length -= size;
		}
	}

	KvByteArray readAll() {
		char* f = first;
		int l = length;
		clear();
		return KvByteArray(f, l);
	}
	int readLine(char* target, int size) {
		int r = kvMin(size, length);
		char* eol = static_cast<char*>(memchr(first, '\n', r));
		if (eol)
			r = 1 + (eol - first);
		memcpy(target, first, r);
		length -= r;
		first += r;
		return int(r);
	}
	bool canReadLine() const {
		return memchr(first, '\n', length);
	}
	void ungetChar(char c) {
		if (first == data) {
			// underflow, the existing valid data needs to move to the end of the (potentially bigger) buffer
			makeSpace(length + 1, freeSpaceAtStart);
		}
		first--;
		length++;
		*first = c;
	}
	void ungetBlock(const char* block, int size) {
		if ((first - data) < size) {
			// underflow, the existing valid data needs to move to the end of the (potentially bigger) buffer
			makeSpace(length + size, freeSpaceAtStart);
		}
		first -= size;
		length += size;
		memcpy(first, block, size);
	}

private:
	enum FreeSpacePos { freeSpaceAtStart, freeSpaceAtEnd };
	void makeSpace(size_t required, FreeSpacePos where) {
		size_t newCapacity = kvMax(capacity, qint64(IODEVICE_BUFFERSIZE));
		while (newCapacity < required)
			newCapacity *= 2;
		int moveOffset = (where == freeSpaceAtEnd) ? 0 : newCapacity - length;
		if (newCapacity > capacity) {
			// allocate more space
			char* newBuf = new char[newCapacity];
			memmove(newBuf + moveOffset, first, length);
			delete[] data;
			data = newBuf;
			capacity = newCapacity;
		}
		else {
			// shift any existing data to make space
			memmove(data + moveOffset, first, length);
		}
		first = data + moveOffset;
	}

};

//template <int Capacity = 1024> class KvStreamString;

class KV_UTILS_EXPORT KvIODevicePrivate
{
public:
	KvIODevicePrivate();
	virtual ~KvIODevicePrivate();

	Kv::OpenMode openMode;
	KvString errorString;

	KvLinearBuffer buffer;

	qint64 pos;
	qint64 devicePos;
	// these three are for fast position updates during read, avoiding isSequential test
	qint64 seqDumpPos;
	qint64 *pPos;
	qint64 *pDevicePos;
	bool baseReadLineDataCalled;
	bool firstRead;
};
#endif // KvIODevicePrivate_h__
