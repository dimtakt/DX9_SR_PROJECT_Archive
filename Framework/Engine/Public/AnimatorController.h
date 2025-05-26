#pragma once

#include "Component.h"
#include "Animator.h"

BEGIN(Engine)

class ENGINE_DLL CAnimatorController final : public CComponent
{
public:
	typedef struct tagTransition
	{
		_wstring strNextStateTag;			// 넘어갈 State 이름
		std::function<_bool()> pCondition;	// 넘어갈 조건
	} TRANSITION;

	typedef struct tagAnimControllerDesc
	{
		CAnimator* pAnimatorCom;
	} ANIMCONTOLLER_DESC;

private:
	CAnimatorController(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAnimatorController(const CAnimatorController& Prototype);
	virtual ~CAnimatorController() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;	// pArg로 애니메이터 받아와야댐
	void Update_Condition();	// 매 프레임마다 전이 조건 검사
	
public:
	HRESULT Add_Transition(const _wstring strPrevStateTag, TRANSITION _trans);
	HRESULT Add_Transition(const _wstring strPrevStateTag, const _wstring strNextStateTag, std::function<_bool()> condition);

private:
	TRANSITION* Find_Transition(const _wstring& strTransitionTag);

private:
	CAnimator* m_pAnimatorCom = { nullptr };

	std::multimap<_wstring, TRANSITION> m_Transitions;
	
	// 변수를 담을 컨테이너 필요. 람다 식의 조건 비교에 필요함.

	//std::map<_wstring, _bool>	m_boolParams = {};
	//std::map<_wstring, _int>	m_intParams = {};
	//std::map<_wstring, _float>	m_floatParams = {};
	//std::map<_wstring, _bool>	m_triggerParams = {};
	

public:
	static CAnimatorController* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

END