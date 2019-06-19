#ifndef SignalSlotTest_h__
#define SignalSlotTest_h__

#include "KvObject.h"

class SignalSlotTest : public KvObject
{
	K_OBJECT

public:
	SignalSlotTest();
	SignalSlotTest(int) {}
	~SignalSlotTest();

K_PROPERTY:
	bool isEnabled() const;
	void setEnabled(bool enabled);

	KvString GetName() const;
	void SetName(const KvString &);

K_SIGNALS:
	void enabledStateChanged(SignalSlotTest *);

public slots:
	void SayHello();
	void CallOneParam(int);
	void CallTwoParam(int, bool);

private:
	bool m_isEnabled = true;
};


#endif // SignalSlotTest_h__
