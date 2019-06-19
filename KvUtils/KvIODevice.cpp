#include "KvIODevice.h"
#include "KvIODevicePrivate.h"

#define CHECK_MAXLEN(function, returnType) \
do { \
if (maxSize < 0) { \
		printf("IODevice::"#function": Called with maxSize < 0"); \
		return returnType; \
} \
} while (0)

#define CHECK_WRITABLE(function, returnType) \
do { \
	if ((d->openMode & Kv::WriteOnly) == 0) { \
	if (d->openMode == Kv::NotOpen) \
			return returnType; \
			printf("IODevice::"#function": ReadOnly device"); \
			return returnType; \
	} \
} while (0)

#define CHECK_READABLE(function, returnType) \
do { \
	if ((d->openMode & Kv::ReadOnly) == 0) {\
	if (d->openMode == Kv::NotOpen) \
				return returnType; \
				printf("IODevice::"#function": WriteOnly device"); \
				return returnType; \
		} \
} while (0)


KvIODevicePrivate::KvIODevicePrivate()
	: openMode(Kv::NotOpen), buffer(IODEVICE_BUFFERSIZE)
	, pos(0), devicePos(0), seqDumpPos(0)
	, pPos(&pos), pDevicePos(&devicePos)
	, baseReadLineDataCalled(false)
	, firstRead(true)
{

}

KvIODevicePrivate::~KvIODevicePrivate()
{

}

KvIODevice::KvIODevice()
:d(new KvIODevicePrivate())
{
}


KvIODevice::~KvIODevice()
{
	delete d;
}

Kv::OpenMode KvIODevice::openMode() const
{
	return d->openMode;
}

void KvIODevice::setTextModeEnabled(bool enabled)
{

}

bool KvIODevice::isTextModeEnabled() const
{
	return false;
}

bool KvIODevice::isOpen() const
{
	return d->openMode != Kv::NotOpen;
}

bool KvIODevice::isReadable() const
{
	return (openMode() & Kv::ReadOnly) != 0;
}

bool KvIODevice::isWritable() const
{
	return (openMode() & Kv::WriteOnly) != 0;
}

bool KvIODevice::open(Kv::OpenMode mode)
{
	d->openMode = mode;
	d->pos = (mode & Kv::Append) ? size() : kint64(0);
	d->buffer.clear();
	d->firstRead = true;
	return true;
}

void KvIODevice::close()
{
	if (d->openMode == Kv::NotOpen)
	{
		return;
	}

	d->openMode = Kv::NotOpen;
	d->errorString.clear();
	d->pos = 0;
	d->buffer.clear();
	d->firstRead = true;
}

kint64 KvIODevice::pos() const
{
	return d->pos;
}

kint64 KvIODevice::size() const
{
	return isSequential() ? bytesAvailable() : kint64(0);
}

bool KvIODevice::seek(kint64 pos)
{
	if (d->openMode == Kv::NotOpen || pos < 0)
	{
		return false;
	}

	kint64 offset = pos - d->pos;
	if (!isSequential())
	{
		d->pos = pos;
		d->devicePos = pos;
	}

	if (offset < 0 || offset >= kint64(d->buffer.size()))
	{
		d->buffer.clear();
	}
	else if (!d->buffer.isEmpty())
	{
		d->buffer.skip(int(offset));
	}

	return true;
}

bool KvIODevice::atEnd() const
{
	return d->openMode == Kv::NotOpen 
		|| (d->buffer.isEmpty() && bytesAvailable() == 0);
}

bool KvIODevice::reset()
{
	return seek(0);
}

kint64 KvIODevice::bytesAvailable() const
{
	if (!isSequential())
	{
		return kvMax(size() - d->pos, kint64(0));
	}
	return d->buffer.size();
}

kint64 KvIODevice::bytesToWrite() const
{
	return kint64(0);
}

KvByteArray KvIODevice::read(kint64 maxSize)
{
	KvByteArray result;

	CHECK_MAXLEN(read, result);

	if (maxSize != kint64(int(maxSize))) 
	{
		printf("QIODevice::read: maxSize argument exceeds QByteArray size limit");
		maxSize = INT_MAX;
	}

	kint64 readBytes = 0;
	if (maxSize)
	{
		result.resize(int(maxSize));
		if (!result.size())
		{
			kint64 readResult;
			do 
			{
				result.resize(int(kvMin(maxSize, result.size() + IODEVICE_BUFFERSIZE)));
				readResult = read(result.data() + readBytes, result.size() - readBytes);
				if (readResult > 0 || readBytes == 0)
				{
					readBytes += readResult;
				}
			} while (readResult == IODEVICE_BUFFERSIZE);
		}
		else
		{
			readBytes = read(result.data(), result.size());
		}
	}

	if (readBytes <= 0)
	{
		result.clear();
	}
	else
	{
		result.resize(int(readBytes));
	}

	return result;
}

kint64 KvIODevice::read(char *data, kint64 maxSize)
{
	if (maxSize == 1)
	{
		int chint;
		while ((chint = d->buffer.getChar() != -1))
		{
			++(*d->pPos);

			char c = char(uchar(chint));
			if ( c == '\r' && (d->openMode & Kv::Text) )
			{
				continue;
			}
			*data = c;
			return kint64(1);
		}
	}

	CHECK_MAXLEN(read, kint64(-1));
	kint64 readSoFar = 0;
	bool moreToRead = true;
	do 
	{
		printf("IODevice::read: Function no implemention\n"); 
		break;
	} while (moreToRead);

	return readSoFar;
}

KvByteArray KvIODevice::readAll()
{
	KvByteArray result;
	kint64 readBytes = 0;

	// flush internal read buffer
	if (!(d->openMode & Kv::Text) && !d->buffer.isEmpty()) 
	{
		result = d->buffer.readAll();
		readBytes = result.size();
		d->pos += readBytes;
	}

	kint64 theSize;
	if (isSequential() || (theSize = size()) == 0) 
	{
		// Size is unknown, read incrementally.
		kint64 readResult;
		do {
			result.resize(result.size() + IODEVICE_BUFFERSIZE);
			readResult = read(result.data() + readBytes, result.size() - readBytes);
			if (readResult > 0 || readBytes == 0)
				readBytes += readResult;
		} while (readResult > 0);
	}
	else 
	{
		// Read it all in one go.
		// If resize fails, don't read anything.
		result.resize(int(readBytes + theSize - d->pos));
		readBytes += read(result.data() + readBytes, result.size() - readBytes);
	}

	if (readBytes <= 0)
		result.clear();
	else
		result.resize(int(readBytes));

	return result;
}

kint64 KvIODevice::readLine(char *data, kint64 maxSize)
{
	if (maxSize < 2) {
		printf("IODevice::readLine: Called with maxSize < 2");
		return kint64(-1);
	}

	// Leave room for a '\0'
	--maxSize;

	kint64 readSoFar = 0;
	const bool sequential = isSequential();
	if (!d->buffer.isEmpty())
	{
		readSoFar = d->buffer.readLine(data, maxSize);
		if (!sequential)
			d->pos += readSoFar;

		if (readSoFar && data[readSoFar - 1] == '\n') 
		{
			if (d->openMode & Kv::Text) 
			{
				// QRingBuffer::readLine() isn't Text aware.
				if (readSoFar > 1 && data[readSoFar - 2] == '\r') 
				{
					--readSoFar;
					data[readSoFar - 1] = '\n';
				}
			}
			data[readSoFar] = '\0';
			return readSoFar;
		}
	}

	if (d->pos != d->devicePos && !sequential && !seek(d->pos))
		return kint64(-1);
	d->baseReadLineDataCalled = false;
	kint64 readBytes = readLineData(data + readSoFar, maxSize - readSoFar);
	if (readBytes < 0) 
	{
		data[readSoFar] = '\0';
		return readSoFar ? readSoFar : -1;
	}
	readSoFar += readBytes;
	if (!d->baseReadLineDataCalled && !sequential) 
	{
		d->pos += readBytes;
		// If the base implementation was not called, then we must
		// assume the device position is invalid and force a seek.
		d->devicePos = kint64(-1);
	}
	data[readSoFar] = '\0';

	if (d->openMode & Kv::Text) 
	{
		if (readSoFar > 1 && data[readSoFar - 1] == '\n' && data[readSoFar - 2] == '\r') {
			data[readSoFar - 2] = '\n';
			data[readSoFar - 1] = '\0';
			--readSoFar;
		}
	}

	return readSoFar;
}

bool KvIODevice::canReadLine() const
{
	return d->buffer.canReadLine();
}

kint64 KvIODevice::write(const char *data, kint64 maxSize)
{
	CHECK_WRITABLE(write, kint64(-1));
	CHECK_MAXLEN(write, kint64(-1));

	const bool sequential = isSequential();
	if (d->pos != d->devicePos && !sequential && !seek(d->pos));
	{
		return kint64(-1);
	}

	if (d->openMode & Kv::Text)
	{
		kint64 writtenSoFar = 0;

		return writtenSoFar;
	}

	kint64 written = writeData(data, maxSize);
	if (written > 0)
	{
		if (!isSequential())
		{
			d->pos += written;
			d->devicePos += written;
		}
		if (!d->buffer.isEmpty() && !isSequential())
		{
			d->buffer.skip(written);
		}
	}
	return written;
}

kint64 KvIODevice::write(const char *data)
{
	return write(data, kvstrlen(data));
}

KvString KvIODevice::errorString() const
{
	return d->errorString;
}

kint64 KvIODevice::readLineData(char *data, kint64 maxSize)
{
	kint64 readSoFar = 0;
	char c = 0;
	int lastReadReturn = 0;
	d->baseReadLineDataCalled = true;

	while (readSoFar < maxSize && (lastReadReturn = read(&c, 1)) == 1) {
		*data++ = c;
		++readSoFar;
		if (c == '\n')
			break;
	}

	if (lastReadReturn != 1 && readSoFar == 0)
		return isSequential() ? lastReadReturn : -1;
	return readSoFar;
}

void KvIODevice::setOpenMode(Kv::OpenMode openMode)
{
	d->openMode = openMode;
	d->firstRead = true;
	if (!isReadable())
	{
		d->buffer.clear();
	}
}

void KvIODevice::setErrorString(const KvString &errorString)
{
	d->errorString = errorString;
}