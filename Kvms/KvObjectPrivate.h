#ifndef KvObjectPrivate_h__
#define KvObjectPrivate_h__

#include "KvGlobal.h"
#include "KvObject.h"
#include "KvList.h"
#include "KvString.h"

#include <atomic>

struct KvSignalSpyCallbackSet
{
	typedef void(*BeginCallback)(KvObject *caller, int method_index, void **argv);
	typedef void(*EndCallback)(KvObject *caller, int method_index);
	BeginCallback signal_begin_callback,
		slot_begin_callback;
	EndCallback signal_end_callback,
		slot_end_callback;
};
void  kv_register_signal_spy_callbacks(const KvSignalSpyCallbackSet &callback_set);

extern KvSignalSpyCallbackSet  kv_signal_spy_callback_set;

class KvObjectPrivate : public KvObjectData
{
	K_DECLARE_PUBLIC(KvObject)

public:
	struct ExtraData
	{
		ExtraData() { }
	};

	struct Connection
	{
		KvObject *sender;
		KvObject *receiver;

		Connection *next;
		Connection *prev;
		std::atomic<int*> argumentTypes;

		ushort method_offset;
		ushort method_relative;
		ushort connectionType : 3; // 0 == auto, 1 == direct, 2 == queued, 4 == blocking
		~Connection();
	};

	struct Sender
	{
		KvObject *sender;
	};

	KvObjectPrivate(int version = 0x010000);
	virtual ~KvObjectPrivate();

	void deleteChildren();

	void setParent_helper(KvObject *);


	static KvObjectPrivate *get(KvObject *o) {
		return o->d_func();
	}

	int signalIndex(const char *signalName) const;
	inline bool isSignalConnected(uint signalIdx) const;

public:
	KvString objectName;
	ExtraData *extraData;    // extra data set by the user
	//QThreadData *threadData; // id of the thread that owns the object

	Connection *senders;     // linked list of connections connected to this object
	Sender *currentSender;   // object currently activating the object
	mutable quint32 connectedSignals[2];

	// preserve binary compatibility with code compiled without Qt 3 support
	// keeping the binary layout stable helps the Qt Creator debugger
	void *unused;
};

inline bool KvObjectPrivate::isSignalConnected(uint signal_index) const
{
	return signal_index >= sizeof(connectedSignals)* 8
		|| (connectedSignals[signal_index >> 5] & (1 << (signal_index & 0x1f))
		|| kv_signal_spy_callback_set.signal_begin_callback
		|| kv_signal_spy_callback_set.signal_end_callback);
}

#endif // KvObjectPrivate_h__
