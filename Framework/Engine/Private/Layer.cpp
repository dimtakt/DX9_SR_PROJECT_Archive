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
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Update(fTimeDelta);
	}
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
	{
		if (nullptr != pGameObject)
			pGameObject->Late_Update(fTimeDelta);
	}
}

CLayer* CLayer::Create()
{
	return new CLayer();
}

void CLayer::Free()
{
	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}
