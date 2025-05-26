#include "AnimatorController.h"

CAnimatorController::CAnimatorController(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CAnimatorController::CAnimatorController(const CAnimatorController& Prototype)
	: CComponent{ Prototype }
{
}

HRESULT CAnimatorController::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAnimatorController::Initialize(void* pArg)
{
	if (nullptr == pArg)
	{
		std::cout << "[CAnimatorController::Initialize] Initialize Failed. Argument was nullptr." << std::endl;
		return E_FAIL;
	}

	ANIMCONTOLLER_DESC* pDesc = static_cast<ANIMCONTOLLER_DESC*>(pArg);
	m_pAnimatorCom = pDesc->pAnimatorCom;

	std::cout << "[CAnimatorController::Initialize] Initialized!" << std::endl;

	return S_OK;
}

void CAnimatorController::Update_Condition()
{
	if (m_pAnimatorCom == nullptr)
		return;

	const _wstring& strCurStateTag = m_pAnimatorCom->Get_CurStateTag();
	auto it = m_Transitions.find(strCurStateTag);

	TRANSITION* pTransition = Find_Transition(strCurStateTag);
	if (pTransition == nullptr) return;

	if (pTransition->pCondition)
		m_pAnimatorCom->Change_State(pTransition->strNextStateTag);
}

HRESULT CAnimatorController::Add_Transition(const _wstring strPrevStateTag, TRANSITION _trans)
{
	if (!m_pAnimatorCom->Check_State(strPrevStateTag) ||
		!m_pAnimatorCom->Check_State(_trans.strNextStateTag))
		return E_FAIL;	// 해당하는 State가 없음

	if (_trans.pCondition == nullptr)
		return E_FAIL;

	m_Transitions.emplace(strPrevStateTag, _trans);

	return S_OK;
}

HRESULT CAnimatorController::Add_Transition(const _wstring strPrevStateTag, const _wstring strNextStateTag, std::function<_bool()> condition)
{
	if (!m_pAnimatorCom->Check_State(strPrevStateTag) ||
		!m_pAnimatorCom->Check_State(strNextStateTag))
		return E_FAIL;	// 해당하는 State가 없음

	if (condition == nullptr)
		return E_FAIL;

	TRANSITION tmpTransition = { strNextStateTag , condition };
	m_Transitions.emplace(strPrevStateTag, tmpTransition);

	return S_OK;
}

CAnimatorController::TRANSITION* CAnimatorController::Find_Transition(const _wstring& strTransitionTag)
{
	auto range = m_Transitions.equal_range(strTransitionTag);
	for (auto it = range.first; it != range.second; ++it)
	{
		if (it->second.pCondition)
			return &(it->second);
	}

	return nullptr;
}

CAnimatorController* CAnimatorController::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAnimatorController* pInstance = new CAnimatorController(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CAnimatorController"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CAnimatorController::Clone(void* pArg)
{
	CAnimatorController* pInstance = new CAnimatorController(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to cloned : CAnimatorController"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAnimatorController::Free()
{
	__super::Free();
}