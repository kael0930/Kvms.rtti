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


	enum ConnectionType {
		AutoConnection,
		DirectConnection,
		QueuedConnection,
		AutoCompatConnection,
		BlockingQueuedConnection,
		UniqueConnection = 0x80
	};
}

#endif // KvNameSpace_h__
