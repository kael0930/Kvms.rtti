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

	virtual qint64 pos() const;
	virtual qint64 size() const;
	virtual bool seek(qint64 pos);
	virtual bool atEnd() const;
	virtual bool reset();

	virtual qint64 bytesAvailable() const;
	virtual qint64 bytesToWrite() const;

	qint64 read(char *data, qint64 maxlen);
	KvByteArray read(qint64 maxlen);
	KvByteArray readAll();
	qint64 readLine(char *data, qint64 maxlen);
	KvByteArray readLine(qint64 maxlen = 0);
	virtual bool canReadLine() const;

	qint64 write(const char *data, qint64 len);
	qint64 write(const char *data);
	inline qint64 write(const KvByteArray &data)
	{
		return write(data.constData(), data.size());
	}

//	qint64 peek(char *data, qint64 maxlen);
//	KvByteArray peek(qint64 maxlen);

//	virtual bool waitForReadyRead(int msecs);
//	virtual bool waitForBytesWritten(int msecs);

	void ungetChar(char c);
	bool putChar(char c);
	bool getChar(char *c);

	KvString errorString() const;

protected:
	virtual qint64 readData(char *data, qint64 maxlen) = 0;
	virtual qint64 readLineData(char *data, qint64 maxlen);
	virtual qint64 writeData(const char *data, qint64 len) = 0;

	void setOpenMode(Kv::OpenMode openMode);
	
	void setErrorString(const KvString &errorString);

private:
	KvIODevicePrivate *d;

	KvIODevice(const KvIODevice &);
	KvIODevice& operator=(const KvIODevice &);
};


#endif // KvIODevice_h__
