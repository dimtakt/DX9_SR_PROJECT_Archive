#include "Anim_Manager.h"
#include "Animation.h"

CAnim_Manager::CAnim_Manager()
{
}

HRESULT CAnim_Manager::Insert_Animation(const wstring& strAnimTag, CAnimation* anim)
{
	CAnimation* pAnim = Find_Animation(strAnimTag);

	if (nullptr != pAnim || nullptr == anim)
		return E_FAIL;

	m_mapAnim.insert({ strAnimTag, anim });

	return S_OK;
}

CAnimation* CAnim_Manager::Find_Animation(const wstring& strAnimTag)
{
	auto iter = m_mapAnim.find(strAnimTag);
	return (iter != m_mapAnim.end()) ? iter->second : nullptr;
}

CAnim_Manager* CAnim_Manager::Create()
{
	return new CAnim_Manager();
}

void CAnim_Manager::Free()
{
	__super::Free();
	for (auto& pair : m_mapAnim)
		Safe_Release(pair.second);

	m_mapAnim.clear();
}