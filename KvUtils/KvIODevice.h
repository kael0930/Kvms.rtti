#ifndef KvIODevice_h__
#define KvIODevice_h__

#include "KvUtils.h"
#include "KvString.h"
#include "KvByteArray.h"
#include "KvNameSpace.h"

class KvIODevicePrivate;
class KV_UTILS_EXPORT KvIODevice
{
public:
	explicit KvIODevice();
	virtual ~KvIODevice();

	Kv::OpenMode openMode() const;

	void setTextModeEnabled(bool enabled);
	bool isTextModeEnabled() const;

	bool isOpen() const;
	bool isReadable() const;
	bool isWritable() const;
	virtual bool isSequential() const { return false; }

	virtual bool open(Kv::OpenMode mode);
	virtual void close();

	virtual kint64 pos() const;
	virtual kint64 size() const;
	virtual bool seek(kint64 pos);
	virtual bool atEnd() const;
	virtual bool reset();

	virtual kint64 bytesAvailable() const;
	virtual kint64 bytesToWrite() const;

	kint64 read(char *data, kint64 maxlen);
	KvByteArray read(kint64 maxlen);
	KvByteArray readAll();
	kint64 readLine(char *data, kint64 maxlen);
	KvByteArray readLine(kint64 maxlen = 0);
	virtual bool canReadLine() const;

	kint64 write(const char *data, kint64 len);
	kint64 write(const char *data);
	inline kint64 write(const KvByteArray &data)
	{
		return write(data.constData(), data.size());
	}

//	kint64 peek(char *data, kint64 maxlen);
//	KvByteArray peek(kint64 maxlen);

//	virtual bool waitForReadyRead(int msecs);
//	virtual bool waitForBytesWritten(int msecs);

	void ungetChar(char c);
	bool putChar(char c);
	bool getChar(char *c);

	KvString errorString() const;

protected:
	virtual kint64 readData(char *data, kint64 maxlen) = 0;
	virtual kint64 readLineData(char *data, kint64 maxlen);
	virtual kint64 writeData(const char *data, kint64 len) = 0;

	void setOpenMode(Kv::OpenMode openMode);
	
	void setErrorString(const KvString &errorString);

private:
	KvIODevicePrivate *d;

	KvIODevice(const KvIODevice &);
	KvIODevice& operator=(const KvIODevice &);
};


#endif // KvIODevice_h__
