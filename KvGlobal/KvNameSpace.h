#ifndef KvNameSpace_h__
#define KvNameSpace_h__

namespace Kv{

	enum PropertyFlags  {
		Invalid = 0x00000000,
		Readable = 0x00000001,
		Writable = 0x00000002,
		Resettable = 0x00000004,
		EnumOrFlag = 0x00000008,
		StdCppSet = 0x00000100,
		//     Override = 0x00000200,
		Constant = 0x00000400,
		Final = 0x00000800,
		Designable = 0x00001000,
		ResolveDesignable = 0x00002000,
		Scriptable = 0x00004000,
		ResolveScriptable = 0x00008000,
		Stored = 0x00010000,
		ResolveStored = 0x00020000,
		Editable = 0x00040000,
		ResolveEditable = 0x00080000,
		User = 0x00100000,
		ResolveUser = 0x00200000,
		Notify = 0x00400000,
		Revisioned = 0x00800000
	};

	enum MethodFlags  {
		AccessPrivate = 0x00,
		AccessProtected = 0x01,
		AccessPublic = 0x02,
		AccessMask = 0x03, //mask

		MethodMethod = 0x00,
		MethodSignal = 0x04,
		MethodSlot = 0x08,
		MethodConstructor = 0x0c,
		MethodTypeMask = 0x0c,

		MethodCompatibility = 0x10,
		MethodCloned = 0x20,
		MethodScriptable = 0x40,
		MethodRevisioned = 0x80
	};

	enum MetaObjectFlags {
		DynamicMetaObject = 0x01
	};

	enum ConnectionType {
		AutoConnection,
		DirectConnection,
		QueuedConnection,
		AutoCompatConnection,
		BlockingQueuedConnection,
		UniqueConnection = 0x80
	};

	enum OpenMode
	{
		NotOpen = 0x0000,
		ReadOnly = 0x0001,
		WriteOnly = 0x0002,
		ReadWrite = ReadOnly | WriteOnly,
		Append = 0x0004,
		Truncate = 0x0008,
		Text = 0x0010,
		Unbuffered = 0x0020
	};
}

#endif // KvNameSpace_h__
