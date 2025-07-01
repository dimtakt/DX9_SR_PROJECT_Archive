#include "Event_Manager.h"

CEvent_Manager::CEvent_Manager()
{
}

void CEvent_Manager::Subscribe(_uint iTypeIndex, IEventListener* pListener)
{
	m_Listeners[iTypeIndex].push_back(pListener);
	//Safe_AddRef(pListener);
}

void CEvent_Manager::Unsubscribe(_uint iTypeIndex, IEventListener* pListener)
{
	auto& vec = m_Listeners[iTypeIndex];
	vec.erase(std::remove(vec.begin(), vec.end(), pListener), vec.end());
}

void CEvent_Manager::Broadcast(_uint iTypeIndex, const EVENTDATA* pData)
{
	for (auto listener : m_Listeners[iTypeIndex])
	{
		if (listener)
			listener->OnEvent(iTypeIndex, pData);
	}
}

CEvent_Manager* CEvent_Manager::Create()
{
	return new CEvent_Manager();
}

void CEvent_Manager::Free()
{
	for (auto& pair : m_Listeners)
	{
		for (IEventListener* pListener : pair.second)
		{
			if (pListener) {
				//Safe_Delete(pListener);
				pListener = nullptr;
			}
				
		}
		pair.second.clear();
	}
	m_Listeners.clear();
	__super::Free();

}
