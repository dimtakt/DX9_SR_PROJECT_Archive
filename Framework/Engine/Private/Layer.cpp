#include "Layer.h"

#include "GameObject.h"

CLayer::CLayer()
{
}

CComponent* CLayer::Find_Component(const _wstring& strComponentTag, _uint iIndex)
{
	auto	iter = m_GameObjects.begin();

	for (size_t i = 0; i < iIndex; i++)
	{
		++iter;
	}

	return (*iter)->Find_Component(strComponentTag);
}

CGameObject* CLayer::Find_GameObject(_uint iIndex)
{
	auto	iter = m_GameObjects.begin();
	for (size_t i = 0; i < iIndex; i++)
	{
		++iter;
	}
	return (*iter);
}

CGameObject* CLayer::Get_LastGameObject()
{
	if (!m_GameObjects.empty())
	{
		return m_GameObjects.back();
	}

	return nullptr;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ) {
		if ((*it)->Get_IsDead()) {
			Safe_Release(*it);
			it = m_GameObjects.erase(it); 
		}
		else {
			if (nullptr != *it)
				(*it)->Priority_Update(fTimeDelta);
			++it;
		}
	}
}

void CLayer::Update(_float fTimeDelta)
{
	std::vector<CGameObject*> vStandbyRemove;

	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject && !pGameObject->Get_IsDead())
			pGameObject->Update(fTimeDelta);
		else if (nullptr != pGameObject && pGameObject->Get_IsDead())
			vStandbyRemove.push_back(pGameObject);
	}

	for (auto& pGameObject : vStandbyRemove)
	{
		auto iter = std::find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject);
		if (iter != m_GameObjects.end())
		{
			m_GameObjects.erase(iter);
			Safe_Release(pGameObject);
		}
	}
}

void CLayer::Late_Update(_float fTimeDelta)
{

	for (auto it = m_GameObjects.begin(); it != m_GameObjects.end(); ) {
		if ((*it)->Get_IsDead()) {
			Safe_Release(*it);
			it = m_GameObjects.erase(it);
		}
		else {
			if (nullptr != *it)
				(*it)->Late_Update(fTimeDelta);
			++it;
		}
	}
}

CLayer* CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();

	__super::Free();
}