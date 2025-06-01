#pragma once
#include "EventListener.h"
#include "Base.h"

BEGIN(Engine)

class CEvent_Manager final : public CBase
{
private:
	CEvent_Manager();
	~CEvent_Manager() = default;
public:
	void Subscribe(_uint iTypeIndex, IEventListener* pListener);
	void Unsubscribe(_uint iTypeIndex, IEventListener* pListener);
	void Broadcast(_uint iTypeIndex, const EVENTDATA* pData);

private:
	unordered_map<_uint, vector<IEventListener*>> m_Listeners;

public:
	static CEvent_Manager* Create();
	virtual void Free() override;

};

END