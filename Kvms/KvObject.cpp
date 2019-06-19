#include "KvObject.h"
#include "KvObjectPrivate.h"
#include "KvMetaObjectPrivate.h"

static int DIRECT_CONNECTION_ONLY = 0;

//KV_CORE_EXPORT QBasicAtomicPointer<KvSignalSpyCallbackSet> qt_signal_spy_callback_set = Q_BASIC_ATOMIC_INITIALIZER(nullptr);
KvSignalSpyCallbackSet  kv_signal_spy_callback_set;
void kv_register_signal_spy_callbacks(KvSignalSpyCallbackSet *callback_set)
{
	//kv_signal_spy_callback_set.store(callback_set);
}

/*!\internal
For a given metaobject, compute the signal offset, and the method offset (including signals)
*/
static void computeOffsets(const KvMetaObject *metaobject, int *signalOffset, int *methodOffset)
{
	*signalOffset = *methodOffset = 0;
	const KvMetaObject *m = metaobject->d.superdata;
	while (m) {
		const KvMetaObjectPrivate *d = KvMetaObjectPrivate::get(m);
		*methodOffset += d->methodCount;
		*signalOffset += (d->revision >= 4) ? d->signalCount : d->methodCount;
		/*Before Qt 4.6 (revision 4), the signalCount information was not generated by moc.
		so for compatibility we consider all the method as slot for old moc output*/
		m = m->d.superdata;
	}
}

KvObject::KvObject(KvObject *parent /*= 0*/)
: d_ptr(new KvObjectPrivate)
{
	d_ptr->q_ptr = this;
	if (parent)
	{
		setParent(parent);
	}
	//kv_addObject(this);
}

KvObject::~KvObject()
{
	K_D(KvObject)

		d->wasDeleted = true;
	d->blockSig = 0; // unblock signals so we always emit destroyed()
}

bool KvObject::event(KvEvent *)
{
	return false;
}

bool KvObject::eventFilter(KvObject *, KvEvent *)
{
	return false;
}

KvString KvObject::objectName() const
{
	K_D(KvObject)

		return d->objectName;
}

void KvObject::setObjectName(const KvString &name)
{
	K_D(KvObject)

		d->objectName = name;
}

void KvObject::setParent(KvObject *parent)
{
	K_D(KvObject)

		d->setParent_helper(parent);
}

bool KvObject::setProperty(const char *name, const KvVariant &value)
{
	const KvMetaObject *meta = metaObject();
	if (!name || !meta)
	{
		return false;
	}

	int id = meta->indexOfProperty(name);
	if (id < 0)
	{
		//自定义、额外动态属性; TODO
	}

	KvMetaProperty p = meta->property(id);
	if (!p.isWritable())
	{
		printf("%s::setProperty: Property \"%s\" invalid,"
			" read-only or does not exist", metaObject()->className(), name);
	}

	return p.write(this, value);
}

KvVariant KvObject::property(const char *name)
{
	const KvMetaObject *mo = metaObject();
	if (!name || !mo)
	{
		return KvVariant();
	}

	int id = mo->indexOfProperty(name);
	if (id < 0)
	{
		//用户属性;
	}
	KvMetaProperty p = mo->property(id);
	if (!p.isReadable())
	{
		printf("%s::property: Property \"%s\" invalid or does not exist",
			metaObject()->className(), name);
	}
	return p.read(this);
}

void KvObject::installEventFilter(KvObject *)
{

}

void KvObject::removeEventFilter(KvObject *)
{

}

/*****************************************************************************
Signals and slots
*****************************************************************************/


const char *kvFlagLocation(const char *method)
{
	//QThreadData::current()->flaggedSignatures.store(method);
	return method;
}

static int extract_code(const char *member)
{
	// extract code, ensure QMETHOD_CODE <= code <= QSIGNAL_CODE
	return (((int)(*member) - '0') & 0x3);
}

static const char * extract_location(const char *member)
{
	//if (QThreadData::current()->flaggedSignatures.contains(member)) 
	{
		// signature includes location information after the first null-terminator
		const char *location = member + kvstrlen(member) + 1;
		if (*location != '\0')
			return location;
	}
	return 0;
}

static bool check_signal_macro(const KvObject *sender, const char *signal,
	const char *func, const char *op)
{
	int sigcode = extract_code(signal);
	if (sigcode != KSIGNAL_CODE)
	{
		if (sigcode == KSLOT_CODE)
			printf("Object::%s: Attempt to %s non-signal %s::%s",
			func, op, sender->metaObject()->className(), signal + 1);
		else
			printf("Object::%s: Use the SIGNAL macro to %s %s::%s",
			func, op, sender->metaObject()->className(), signal);
		return false;
	}
	return true;
}

static bool check_method_code(int code, const KvObject *object,
	const char *method, const char *func)
{
	if (code != KSLOT_CODE && code != KSIGNAL_CODE) {
		printf("Object::%s: Use the SLOT or SIGNAL macro to "
			"%s %s::%s", func, func, object->metaObject()->className(), method);
		return false;
	}
	return true;
}

static void err_method_notfound(const KvObject *object,
	const char *method, const char *func)
{
	const char *type = "method";
	switch (extract_code(method)) {
	case KSLOT_CODE:   type = "slot";   break;
	case KSIGNAL_CODE: type = "signal"; break;
	}
	const char *loc = extract_location(method);
	if (strchr(method, ')') == 0)                // common typing mistake
		printf("Object::%s: Parentheses expected, %s %s::%s%s%s",
		func, type, object->metaObject()->className(), method + 1,
		loc ? " in " : "", loc ? loc : "");
	else
		printf("Object::%s: No such %s %s::%s%s%s",
		func, type, object->metaObject()->className(), method + 1,
		loc ? " in " : "", loc ? loc : "");

}

static void err_info_about_objects(const char * func,
	const KvObject * sender,
	const KvObject * receiver)
{
	KvString a = sender ? sender->objectName() : KvString();
	KvString b = receiver ? receiver->objectName() : KvString();
	if (!a.isEmpty())
		printf("Object::%s:  (sender name:   '%s')", func, a.get());
	if (!b.isEmpty())
		printf("Object::%s:  (receiver name: '%s')", func, b.get());
}

bool KvObject::connect(const KvObject * sender, const char *signal,
	const KvObject *receiver, const char *method,
	Kv::ConnectionType type)
{
#if 0
	{
		const void *cbdata[] = { sender, signal, receiver, method, &type };
		if (KvInternal::activateCallbacks(KvInternal::ConnectCallback, (void **)cbdata))
			return true;
	}
#endif

	if (sender == 0 || receiver == 0 || signal == 0 || method == 0)
	{
		printf("KvObject::connect: Cannot connect %s::%s to %s::%s",
			sender ? sender->metaObject()->className() : "(null)",
			(signal && *signal) ? signal + 1 : "(null)",
			receiver ? receiver->metaObject()->className() : "(null)",
			(method && *method) ? method + 1 : "(null)");
		return false;
	}
	KvByteArray tmp_signal_name;

	if (!check_signal_macro(sender, signal, "connect", "bind"))
		return false;
	const KvMetaObject *smeta = sender->metaObject();
	const char *signal_arg = signal;
	++signal; //skip code

	KvArgumentTypeArray  signalTypes;
	KvByteArray signalName = KvMetaObjectPrivate::decodeMethodSignature(signal, signalTypes);
	int signal_index = KvMetaObjectPrivate::indexOfSignalRelative(&smeta, signalName, false);
	if (signal_index < 0) {
		// check for normalized signatures
		tmp_signal_name = KvMetaObject::normalizedSignature(signal - 1);
		signal = tmp_signal_name.constData() + 1;

		smeta = sender->metaObject();
		signal_index = KvMetaObjectPrivate::indexOfSignalRelative(&smeta, signal, false);
	}
	if (signal_index < 0) {
		// re-use tmp_signal_name and signal from above
		smeta = sender->metaObject();
		signal_index = KvMetaObjectPrivate::indexOfSignalRelative(&smeta, signal, true);
	}
	if (signal_index < 0) {
		err_method_notfound(sender, signal_arg, "connect");
		err_info_about_objects("connect", sender, receiver);
		return false;
	}

	signal_index = KvMetaObjectPrivate::originalClone(smeta, signal_index);
	int signalOffset, methodOffset;
	computeOffsets(smeta, &signalOffset, &methodOffset);
	int signal_absolute_index = signal_index + methodOffset;
	signal_index += signalOffset;

	KvByteArray tmp_method_name;
	int membcode = extract_code(method);

	if (!check_method_code(membcode, receiver, method, "connect"))
		return false;
	const char *method_arg = method;
	++method; // skip code

	const KvMetaObject *rmeta = receiver->metaObject();
	int method_index_relative = -1;
	switch (membcode) {
	case KSLOT_CODE:
		method_index_relative = KvMetaObjectPrivate::indexOfSlotRelative(&rmeta, method, false);
		break;
	case KSIGNAL_CODE:
		method_index_relative = KvMetaObjectPrivate::indexOfSignalRelative(&rmeta, method, false);
		break;
	}

	if (method_index_relative < 0) {
		// check for normalized methods
		tmp_method_name = KvMetaObject::normalizedSignature(method);
		method = tmp_method_name.constData();

		// rmeta may have been modified above
		rmeta = receiver->metaObject();
		switch (membcode) {
		case KSLOT_CODE:
			method_index_relative = KvMetaObjectPrivate::indexOfSlotRelative(&rmeta, method, false);
			if (method_index_relative < 0)
				method_index_relative = KvMetaObjectPrivate::indexOfSlotRelative(&rmeta, method, true);
			break;
		case KSIGNAL_CODE:
			method_index_relative = KvMetaObjectPrivate::indexOfSignalRelative(&rmeta, method, false);
			if (method_index_relative < 0)
				method_index_relative = KvMetaObjectPrivate::indexOfSignalRelative(&rmeta, method, true);
			break;
		}
	}

	if (method_index_relative < 0) {
		err_method_notfound(receiver, method_arg, "connect");
		err_info_about_objects("connect", sender, receiver);
		return false;
	}

	if (!KvMetaObject::checkConnectArgs(signal, method)) {
		printf("QObject::connect: Incompatible sender/receiver arguments"
			"\n        %s::%s --> %s::%s",
			sender->metaObject()->className(), signal,
			receiver->metaObject()->className(), method);
		return false;
	}

	int *types = 0;
	if ((type == Kv::QueuedConnection)
		/*&& !(types = queuedConnectionTypes(smeta->method(signal_absolute_index).parameterTypes()))*/)
		return false;

	if (!KvMetaObjectPrivate::connect(sender, signal_index, receiver, method_index_relative, rmeta, type, types))
		return false;
	const_cast<KvObject*>(sender)->connectNotify(signal - 1);
	return true;
}

bool KvObject::disconnect(const KvObject *sender, const KvMetaMethod &signal,
	const KvObject *receiver, const KvMetaMethod &member)
{
	return true;
}

bool KvObject::disconnect(const KvObject *sender, const char *signal,
	const KvObject *receiver, const char *member)
{
	return true;
}

KvObject * KvObject::sender() const
{
	K_D(KvObject)

		//locker TODO
	if (!d->currentSender)
		return 0;
	for (KvObjectPrivate::Connection * c = d->senders; c; c->next)
	{
		if (c->sender == d->currentSender->sender)
		{
			return d->currentSender->sender;
		}
	}

	return 0;
}

void KvObject::deleteLater()
{
	printf("delete later...\n");
	//QCoreApplication::postEvent(this, new QEvent(QEvent::DeferredDelete));
}

void kv_findChildren_helper(const KvObject *parent, const KvString &name, /*const QRegExp *re,*/ const KvMetaObject &mo, KvList<void *> *list)
{

}

KvObject * kv_findChild_helper(const KvObject *parent, const KvString &name, const KvMetaObject &mo)
{
	if (!parent)
		return 0;

	const ObjectList &children = parent->children();
	KvObject *obj = 0;
	ObjectList::const_iterator it = children.begin();
	for (; it != children.end(); ++it)
	{
		obj = (*it);
		if (mo.cast(obj) && (!name.isEmpty() || obj->objectName() == name))
		{
			return obj;
		}
	}

	for (; it != children.end(); ++it)
	{
		obj = kv_findChild_helper((*it), name, mo);
		if (obj)
		{
			return obj;
		}
	}

	return 0;
}

//////////////////////////////////////////////////////////////////////////

KvObjectData::~KvObjectData() {}

KvObjectPrivate::Connection::~Connection()
{
	if (argumentTypes != &DIRECT_CONNECTION_ONLY)
		delete[] static_cast<int *>(argumentTypes);
}

class ConnectionListVector : public KvVector<KvObjectPrivate::ConnectionList>
{
public:
	bool orphaned; //the QObject owner of this vector has been destroyed while the vector was inUse
	bool dirty; //some Connection have been disconnected (their receiver is 0) but not removed from the list yet
	int inUse; //number of functions that are currently accessing this object or its connections

	KvObjectPrivate::ConnectionList allsignals;

	ConnectionListVector()
		: KvVector<KvObjectPrivate::ConnectionList>(), orphaned(false), dirty(false), inUse(0)
	{ }

	KvObjectPrivate::ConnectionList &operator[](int at)
	{
		if (at < 0)
			return allsignals;
		return KvVector<KvObjectPrivate::ConnectionList>::operator[](at);
	}
};

KvObjectPrivate::KvObjectPrivate(int version)
: /*threadData(0),*/ connectionLists(0), senders(0), currentSender(0)/*, currentChildBeingDeleted(0)*/
{
	if (version != 0x010000)
		printf("Cannot mix incompatible Qt library (version 0x%x) with this library (version 0x%x)",
		version, 0x010000);

	//ObjectData initialization
	q_ptr = 0;
	parent = 0;                                 // no parent yet. It is set by setParent()
	isWidget = false;                           // assume not a widget object
	pendTimer = false;                          // no timers yet
	blockSig = false;                           // not blocking signals
	wasDeleted = false;                         // double-delete catcher
	//sendChildEvents = true;                     // if we should send ChildInsert and ChildRemove events to parent
	//receiveChildEvents = true;
	//postedEvents = 0;
	extraData = 0;
	connectedSignals[0] = connectedSignals[1] = 0;
	//inThreadChangeEvent = false;

	metaObject = 0;
	//hasGuards = false;
}

KvObjectPrivate::~KvObjectPrivate()
{

}

void KvObjectPrivate::deleteChildren()
{

}

void KvObjectPrivate::setParent_helper(KvObject *)
{

}

int KvObjectPrivate::signalIndex(const char *signalName) const
{
	K_Q(KvObject)

		const KvMetaObject *base = q->metaObject();
	int relative_index = KvMetaObjectPrivate::indexOfSignalRelative(&base, signalName, false);
	if (relative_index < 0)
		relative_index = KvMetaObjectPrivate::indexOfSignalRelative(&base, signalName, true);
	if (relative_index < 0)
		return relative_index;
	relative_index = KvMetaObjectPrivate::originalClone(base, relative_index);
	int signalOffset, methodOffset;
	computeOffsets(base, &signalOffset, &methodOffset);
	return relative_index + signalOffset;
}

void KvObjectPrivate::addConnection(int signal, Connection *c)
{
	if (!connectionLists)
		connectionLists = new ConnectionListVector();
	if (signal >= connectionLists->size())
		connectionLists->resize(signal + 1);

	ConnectionList &connectionList = (*connectionLists)[signal];
	if (connectionList.last) {
		connectionList.last->nextConnectionList = c;
	}
	else {
		connectionList.first = c;
	}
	connectionList.last = c;

	cleanConnectionLists();
}

void KvObjectPrivate::cleanConnectionLists()
{

}

//////////////////////////////////////////////////////////////////////////

void KvMetaObject::activate(KvObject *sender, const KvMetaObject *m, int local_signal_index, void **argv)
{
	int signalOffset;
	int methodOffset;
	computeOffsets(m, &signalOffset, &methodOffset);

	int signal_index = signalOffset + local_signal_index;

	if (!sender->d_func()->isSignalConnected(signal_index))
		return; // nothing connected to these signals, and no spy

	if (sender->d_func()->blockSig)
		return;

	int signal_absolute_index = methodOffset + local_signal_index;

	void *empty_argv[] = { 0 };
	if (kv_signal_spy_callback_set.signal_begin_callback != 0) {
		kv_signal_spy_callback_set.signal_begin_callback(sender, signal_absolute_index,
			argv ? argv : empty_argv);
	}

	//Qt::HANDLE currentThreadId = QThread::currentThreadId();

	//QMutexLocker locker(signalSlotLock(sender));
	ConnectionListVector *connectionLists = sender->d_func()->connectionLists;
	if (!connectionLists) {
		//locker.unlock();
		if (kv_signal_spy_callback_set.signal_end_callback != 0)
			kv_signal_spy_callback_set.signal_end_callback(sender, signal_absolute_index);
		return;
	}
	++connectionLists->inUse;

	const KvObjectPrivate::ConnectionList *list;
	if (signal_index < connectionLists->size())
		list = &connectionLists->at(signal_index);
	else
		list = &connectionLists->allsignals;

	do {
		KvObjectPrivate::Connection *c = list->first;
		if (!c) continue;
		// We need to check against last here to ensure that signals added
		// during the signal emission are not emitted in this emission.
		KvObjectPrivate::Connection *last = list->last;

		do {
			if (!c->receiver)
				continue;

			KvObject * const receiver = c->receiver;
			const bool receiverInSameThread = true;// currentThreadId == receiver->d_func()->threadData->threadId;

			// determine if this connection should be sent immediately or
			// put into the event queue
			if ((c->connectionType == Kv::AutoConnection && !receiverInSameThread)
				|| (c->connectionType == Kv::QueuedConnection))
			{
				printf("no support in mult thread...\n");
				break;
			}
			else if (c->connectionType == Kv::BlockingQueuedConnection)
			{
				printf("no support in queued connection...\n");
				break;
			}

			KvObjectPrivate::Sender currentSender;
			KvObjectPrivate::Sender *previousSender = 0;
			if (receiverInSameThread) {
				currentSender.sender = sender;
				currentSender.signal = signal_absolute_index;
				currentSender.ref = 1;
				previousSender = KvObjectPrivate::setCurrentSender(receiver, &currentSender);
			}
			const KvObjectPrivate::StaticMetaCallFunction callFunction = c->callFunction;
			const int method_relative = c->method_relative;
			if (callFunction && c->method_offset <= receiver->metaObject()->methodOffset())
			{
				//we compare the vtable to make sure we are not in the destructor of the object.
				//locker.unlock();
				if (kv_signal_spy_callback_set.slot_begin_callback != 0)
					kv_signal_spy_callback_set.slot_begin_callback(receiver, c->method(), argv ? argv : empty_argv);

#ifdef KV_NO_EXCEPTIONS
				callFunction(receiver, KvMetaObject::InvokeMetaMethod, method_relative, argv ? argv : empty_argv);
#else
				KV_TRY
				{
					callFunction(receiver, KvMetaObject::InvokeMetaMethod, method_relative, argv ? argv : empty_argv);
				} 
				KV_CATCH(...) 
				{
					//locker.relock();
					if (receiverInSameThread)
						KvObjectPrivate::resetCurrentSender(receiver, &currentSender, previousSender);

					--connectionLists->inUse;
					assert(connectionLists->inUse >= 0);
					if (connectionLists->orphaned && !connectionLists->inUse)
						delete connectionLists;
					KV_RETHROW;
				}
#endif
				if (kv_signal_spy_callback_set.slot_end_callback != 0)
					kv_signal_spy_callback_set.slot_end_callback(receiver, c->method());
				//locker.relock();
			}
			else
			{
				const int method = method_relative + c->method_offset;
				//locker.unlock();

				if (kv_signal_spy_callback_set.slot_begin_callback != 0) {
					kv_signal_spy_callback_set.slot_begin_callback(receiver,
						method,
						argv ? argv : empty_argv);
				}

#ifdef KV_NO_EXCEPTIONS
				metacall(receiver, KvMetaObject::InvokeMetaMethod, method, argv ? argv : empty_argv);
#else
				KV_TRY
				{
					metacall(receiver, KvMetaObject::InvokeMetaMethod, method, argv ? argv : empty_argv);
				} 
				KV_CATCH(...) 
				{
					//locker.relock();
					if (receiverInSameThread)
						KvObjectPrivate::resetCurrentSender(receiver, &currentSender, previousSender);

					--connectionLists->inUse;
					assert(connectionLists->inUse >= 0);
					if (connectionLists->orphaned && !connectionLists->inUse)
						delete connectionLists;
					KV_RETHROW;
				}
#endif

				if (kv_signal_spy_callback_set.slot_end_callback != 0)
					kv_signal_spy_callback_set.slot_end_callback(receiver, method);

				//locker.relock();
			}

			if (receiverInSameThread)
				KvObjectPrivate::resetCurrentSender(receiver, &currentSender, previousSender);

			if (connectionLists->orphaned)
				break;
		} while (c != last && (c = c->nextConnectionList) != 0);

		if (connectionLists->orphaned)
			break;
	} while (list != &connectionLists->allsignals &&
		//start over for all signals;
		((list = &connectionLists->allsignals), true));

	--connectionLists->inUse;
	assert(connectionLists->inUse >= 0);
	if (connectionLists->orphaned) {
		if (!connectionLists->inUse)
			delete connectionLists;
	}
	else if (connectionLists->dirty) {
		sender->d_func()->cleanConnectionLists();
	}

	//locker.unlock();

	if (kv_signal_spy_callback_set.signal_end_callback != 0)
		kv_signal_spy_callback_set.signal_end_callback(sender, signal_absolute_index);

}

void KvMetaObjectPrivate::memberIndexes(const KvObject *obj, const KvMetaMethod &member, int *signalIndex, int *methodIndex)
{

}

bool KvMetaObjectPrivate::connect(const KvObject *sender, int signal_index,
	const KvObject *receiver, int method_index,
	const KvMetaObject *rmeta /*= 0*/, int type /*= 0*/, int *types /*= 0*/)
{
	KvObject *s = const_cast<KvObject *>(sender);
	KvObject *r = const_cast<KvObject *>(receiver);

	int method_offset = rmeta ? rmeta->methodOffset() : 0;
	KvObjectPrivate::StaticMetaCallFunction callFunction =
		(rmeta && KvMetaObjectPrivate::get(rmeta)->revision >= 6 && rmeta->d.extradata)
		? reinterpret_cast<const KvMetaObjectExtraData *>(rmeta->d.extradata)->static_metacall : 0;

	//QOrderedMutexLocker locker;

	if (type & Kv::UniqueConnection) {
		ConnectionListVector *connectionLists = KvObjectPrivate::get(s)->connectionLists;
		if (connectionLists && connectionLists->size() > signal_index) {
			const KvObjectPrivate::Connection *c2 =
				(*connectionLists)[signal_index].first;

			int method_index_absolute = method_index + method_offset;

			while (c2) {
				if (c2->receiver == receiver && c2->method() == method_index_absolute)
					return false;
				c2 = c2->nextConnectionList;
			}
		}
		type &= Kv::UniqueConnection - 1;
	}

	KvObjectPrivate::Connection *c = new KvObjectPrivate::Connection;
	c->sender = s;
	c->receiver = r;
	c->method_relative = method_index;
	c->method_offset = method_offset;
	c->connectionType = type;
	c->argumentTypes = types;
	c->nextConnectionList = 0;
	c->callFunction = callFunction;

	KV_TRY{
		KvObjectPrivate::get(s)->addConnection(signal_index, c);
	} KV_CATCH(...) {
		delete c;
		KV_RETHROW;
	}

	c->prev = &(KvObjectPrivate::get(r)->senders);
	c->next = *c->prev;
	*c->prev = c;
	if (c->next)
		c->next->prev = &c->next;

	KvObjectPrivate *const sender_d = KvObjectPrivate::get(s);
	if (signal_index < 0) {
		sender_d->connectedSignals[0] = sender_d->connectedSignals[1] = ~0;
	}
	else if (signal_index < (int)sizeof(sender_d->connectedSignals) * 8) {
		sender_d->connectedSignals[signal_index >> 5] |= (1 << (signal_index & 0x1f));
	}

	return true;
}

bool KvMetaObjectPrivate::disconnect(const KvObject *sender, int signal_index,
	const KvObject *receiver, int method_index, DisconnectType /*= DisconnectAll*/)
{
	return true;
}

bool KvMetaObjectPrivate::disconnectHelper(KvObjectPrivate::Connection *c, const KvObject *receiver,
	int method_index, /*QMutex *senderMutex, */DisconnectType)
{
	return true;
}


///
//////////////////////////////////////////////////////////////////////////

struct kv_meta_stringdata_KvObject_t {
	KvByteArrayData data[4];
	char stringdata0[45];
};

static const kv_meta_stringdata_KvObject_t kv_meta_stringdata_KvObject = {
	{
		KV_META_LITERAL(KvObject,0, 0, 8), // "QObject"
		KV_META_LITERAL(KvObject, 1, 9, 9), // "destroyed"
		KV_META_LITERAL(KvObject, 2, 19, 10), // "objectName"
		KV_META_LITERAL(KvObject, 3, 30, 11), // "deleteLater"
	},
	"KvObject\0destroyed\0objectName\0deleteLater"
};

static const uint kv_meta_data_KvObject[] = {

	// content:
	7,       // revision
	0,       // classname
	0, 0, // classinfo
	3, 14, // methods
	1, 27, // properties
	0, 0, // enums/sets
	2, 58, // constructors
	0,       // flags
	3,       // signalCount

	// signals: name, argc, parameters, tag, flags
	1, 1, 39, 2, 0x06 /* Public */,
	1, 0, 42, 2, 0x26 /* Public | MethodCloned */,

	// slots: name, argc, parameters, tag, flags
	3, 0, 46, 2, 0x0a /* Public */,

	// signals: parameters
	KvMetaType::Void, KvMetaType::QObjectStar, 2,
	KvMetaType::Void,

	// slots: parameters
	KvMetaType::Void,

	// constructors: parameters
	0x80000000 | 2, KvMetaType::QObjectStar, 7,
	0x80000000 | 2,

	// properties: name, type, flags
	2, KvMetaType::KvString, 0x00495103,

	// properties: notify_signal_id
	2,

	// constructors: name, argc, parameters, tag, flags
	0, 1, 50, 2, 0x0e /* Public */,
	0, 0, 53, 2, 0x2e /* Public | MethodCloned */,

	0        // eod
};

void KvObject::kv_static_metacall(KvObject *_o, KvMetaObject::Call _c, int _id, void **_a)
{
	if (_c == KvMetaObject::CreateInstance) {
		switch (_id) {
		case 0: { KvObject *_r = new KvObject((*reinterpret_cast<KvObject*(*)>(_a[1])));
			if (_a[0]) *reinterpret_cast<KvObject**>(_a[0]) = _r; } break;
		case 1: { KvObject *_r = new KvObject();
			if (_a[0]) *reinterpret_cast<KvObject**>(_a[0]) = _r; } break;
		}
	}
	else if (_c == KvMetaObject::InvokeMetaMethod) {
		assert(staticMetaObject.cast(_o));
		KvObject *_t = static_cast<KvObject *>(_o);
		switch (_id) {
		case 0: _t->destroyed((*reinterpret_cast<KvObject*(*)>(_a[1]))); break;
		case 1: _t->destroyed(); break;
		case 2: _t->deleteLater(); break;
		default:;
		}
	}
}

const KvMetaObjectExtraData KvObject::staticMetaObjectExtraData = {
	0, kv_static_metacall
};

const KvMetaObject KvObject::staticMetaObject = {
	{ 0, kv_meta_stringdata_KvObject.data,
	kv_meta_data_KvObject, &staticMetaObjectExtraData }
};

#ifdef K_NO_DATA_RELOCATION
const KvMetaObject &KvObject::getStaticMetaObject() { return staticMetaObject; }
#endif //K_NO_DATA_RELOCATION

const KvMetaObject *KvObject::metaObject() const
{
	return KvObject::d_ptr->metaObject ? KvObject::d_ptr->metaObject : &staticMetaObject;
}

void *KvObject::kv_metacast(const char *_clname)
{
	if (!_clname) return 0;
	if (!strcmp(_clname, kv_meta_stringdata_KvObject.stringdata0))
		return static_cast<void*>(const_cast<KvObject*>(this));
	return 0;
}

int KvObject::kv_metacall(KvMetaObject::Call _c, int _id, void **_a)
{
	if (_id < 0)
		return _id;
	if (_c == KvMetaObject::InvokeMetaMethod) {
		if (_id < 4)
			kv_static_metacall(this, _c, _id, _a);
		_id -= 4;
	}
	else if (_c == KvMetaObject::ReadProperty) {
		void *_v = _a[0];
		switch (_id) {
		case 0: *reinterpret_cast<KvString*>(_v) = objectName(); break;
		}
		_id -= 1;
	}
	else if (_c == KvMetaObject::WriteProperty) {
		void *_v = _a[0];
		switch (_id) {
		case 0: setObjectName(*reinterpret_cast<KvString*>(_v)); break;
		}
		_id -= 1;
	}
	else if (_c == KvMetaObject::ResetProperty) {
		_id -= 1;
	}
	else if (_c == KvMetaObject::QueryPropertyDesignable) {
		_id -= 1;
	}
	else if (_c == KvMetaObject::QueryPropertyScriptable) {
		_id -= 1;
	}
	else if (_c == KvMetaObject::QueryPropertyStored) {
		_id -= 1;
	}
	else if (_c == KvMetaObject::QueryPropertyEditable) {
		_id -= 1;
	}
	else if (_c == KvMetaObject::QueryPropertyUser) {
		_id -= 1;
	}
	return _id;
}

// SIGNAL 0
void KvObject::destroyed(KvObject * _t1)
{
	void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
	KvMetaObject::activate(this, &staticMetaObject, 0, _a);
}

//////////////////////////////////////////////////////////////////////////
///
