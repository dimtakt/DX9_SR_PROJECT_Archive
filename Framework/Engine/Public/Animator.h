#pragma once

#include "Component.h"

BEGIN(Engine)

class CTexture;

class ENGINE_DLL CAnimator final : public CComponent
{
public:
	// 각 State가 가질 정보
	typedef struct tagAnimState
	{
		CTexture* pTextureCom;	// 텍스쳐 변화를 애니메이터 내에서 구현을 위함
		_int iFramePerImage;	// 몇프레임 뒤에 다음 이미지로 넘어갈건지
		_bool isExitable;		// 프레임이 끝나지 않아도 다른 State로 넘어갈 수 있는지
	} ANIMSTATE;

	// Animator 최초 생성 시 필요로 하는 정보 (pArg)
	typedef struct tagStartStateDesc
	{
		_wstring strTimerTag; // 애니메이터 다중 사용을 고려하여 타이머 태그를 다르게 구성
	} ANIMSTATE_DESC;
	
private:
	CAnimator(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAnimator(const CAnimator& Prototype);
	virtual ~CAnimator() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	void Update_State();

public:
	HRESULT Add_State(const _wstring strStateTag, ANIMSTATE _state);
	ANIMSTATE* Get_CurState()	{ return m_pCurState; };
	_wstring Get_CurStateTag()	{ return m_strCurStateTag; };

	// bool 인자는, 큰 상태는 동일하나
	// 플레이어가 바라보는 방향만 달라지는 경우처럼 현재 프레임 순서의 유지 필요성이 있는 경우의 예외를 두기 위함
	void Change_State(const _wstring strStatetag, _bool isChangeCurFrame = true);

private:
	ANIMSTATE* Find_State(const _wstring& strStateTag);

private:
	// ANIMSTATE m_tPrevState = {}; // 필요하면 사용?
	ANIMSTATE*	m_pCurState = { nullptr };
	_wstring	m_strCurStateTag = {};
	_wstring	m_strTimerTag = {};

	std::map<const _wstring, ANIMSTATE> m_pStates = {};

	_uint		m_iStackedFrames = {};

public:
	static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END