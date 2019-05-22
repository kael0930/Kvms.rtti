#ifndef KvObjectPrivate_h__
#define KvObjectPrivate_h__

#include "KvList.h"
#include "KvString.h"

#include <atomic>

class KvObject;
class ConnectionListVector;

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

	typedef void(*StaticMetaCallFunction)(KvObject *, KvMetaObject::Call, int, void **);
	struct Connection
	{
		KvObject *sender;
		KvObject *receiver;
		StaticMetaCallFunction callFunction;
		// The next pointer for the singly-linked ConnectionList
		Connection *nextConnectionList;
		//senders linked list
		Connection *next;
		Connection **prev;
		std::atomic<int*> argumentTypes;

		ushort method_offset;
		ushort method_relative;
		ushort connectionType : 3; // 0 == auto, 1 == direct, 2 == queued, 4 == blocking
		~Connection();
		int method() const { return method_offset + method_relative; }

	};
	// ConnectionList is a singly-linked list
	struct ConnectionList {
		ConnectionList() : first(0), last(0) {}
		Connection *first;
		Connection *last;
	};

	struct Sender
	{
		KvObject *sender;
		int signal;
		int ref;
	};

	KvObjectPrivate(int version = 0x010000);
	virtual ~KvObjectPrivate();
	void deleteChildren();

	void setParent_helper(KvObject *);

	void addConnection(int signal, Connection *c);
	void cleanConnectionLists();

	static inline Sender *setCurrentSender(KvObject *receiver,
		Sender *sender);
	static inline void resetCurrentSender(KvObject *receiver,
		Sender *currentSender,
		Sender *previousSender);

	static KvObjectPrivate *get(KvObject *o) {
		return o->d_func();
	}

	int signalIndex(const char *signalName) const;
	inline bool isSignalConnected(uint signalIdx) const;

public:
	KvString objectName;
	ExtraData *extraData;    // extra data set by the user
	//QThreadData *threadData; // id of the thread that owns the object

	ConnectionListVector *connectionLists;

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

inline KvObjectPrivate::Sender *KvObjectPrivate::setCurrentSender(KvObject *receiver,
	Sender *sender)
{
	Sender *previousSender = receiver->d_func()->currentSender;
	receiver->d_func()->currentSender = sender;
	return previousSender;
}

inline void KvObjectPrivate::resetCurrentSender(KvObject *receiver,
	Sender *currentSender,
	Sender *previousSender)
{
	// ref is set to zero when this object is deleted during the metacall
	if (currentSender->ref == 1)
		receiver->d_func()->currentSender = previousSender;
	// if we've recursed, we need to tell the caller about the objects deletion
	if (previousSender)
		previousSender->ref = currentSender->ref;
}


#endif // KvObjectPrivate_h__
