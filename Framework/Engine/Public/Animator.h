#pragma once

#include "Component.h"
#include "Texture.h"
#include "Animation.h"

BEGIN(Engine)

class ENGINE_DLL CAnimator final : public CComponent
{
public:
	// 각 State가 가질 정보
	typedef struct tagAnimState
	{
		CTexture* pTextureCom = nullptr;	// 텍스쳐 변화를 애니메이터 내에서 구현을 위함. 없어도 사용 가능하며, 이 경우 iFramePerImage 프레임 뒤에 상태전이 준비가 됨
		_int iFramePerImage;	// 몇프레임 뒤에 다음 이미지로 넘어갈건지
		_bool isExitable;		// 프레임이 끝나지 않아도 다른 State로 넘어갈 수 있는지
		CAnimation* pAnimation = nullptr;
	} ANIMSTATE;

	// Animator 최초 생성 시 필요로 하는 정보 (pArg)
	typedef struct tagStartStateDesc
	{
		_wstring strTimerTag; // 애니메이터 다중 사용을 고려하여 타이머 태그를 다르게 구성
		CTransform* pParentTransform = nullptr;
		CTransform* pChildTransform = nullptr;
	} ANIMSTATE_DESC;
	
private:
	CAnimator(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAnimator(const CAnimator& Prototype);
	virtual ~CAnimator() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	void Update_State();
	_bool Change_State(const _wstring strStatetag, _bool isChangeCurFrame = true); // bool 인자 : 현재 출력중인 이미지 순서 초기화 할건지

public:
	HRESULT Add_State(const _wstring strStateTag, ANIMSTATE _state);
	ANIMSTATE* Get_CurState()	{ return m_pCurState; };
	_wstring Get_PrevStateTag()	{ return m_strPrevStateTag; };
	_wstring Get_CurStateTag()	{ return m_strCurStateTag; };
	_int Get_CurStackedFrame() { return m_iStackedFrames; };
	_bool Get_IsLastFrame() {
		float result1 = static_cast<float>((m_iStackedFrames + 1) / m_pCurState->iFramePerImage);
		float result2 = static_cast<float>((m_pCurState->pTextureCom) ? m_pCurState->pTextureCom->Get_NumTextures() : 1);
		bool result = (result1 >= result2);
		return result;

		//return (((m_iStackedFrames + 1.0f) / m_pCurState->iFramePerImage - 1.0f) >= m_pCurState->pTextureCom->Get_NumTextures());
	}
	_bool Get_IsReachedFrame(_uint iFrame) {
		// 해당 번째 이미지가 지나기 직전 시점부터 true를 반환합니다.
		return (((m_iStackedFrames + 1) / m_pCurState->iFramePerImage - 1) >= iFrame);
	}
	_bool Check_State(const _wstring& strStateTag) {
		// 해당 State가 존재하는지 여부만 확인
		return Find_State(strStateTag) != nullptr;
	}
	//_bool Get_isChangedState() { return (m_strPrevStateTag == m_strCurStateTag); }

	void Set_ParentTransform(CTransform* pParentTransform) { m_pParentTransform = pParentTransform; };
	void Set_ChildTransform(CTransform* pChildTransform) { m_pChildTransform = pChildTransform; };


private:
	ANIMSTATE* Find_State(const _wstring& strStateTag);
	void Update_Keyframes();

private:
	ANIMSTATE*	m_pPrevState = { nullptr };
	ANIMSTATE*	m_pCurState = { nullptr };
	_wstring	m_strPrevStateTag = {};
	_wstring	m_strCurStateTag = {};
	_wstring	m_strTimerTag = {};

	std::map<const _wstring, ANIMSTATE> m_pStates = {};

	_uint		m_iStackedFrames = {};

	CTransform* m_pParentTransform = { nullptr };	// 애니메이션 - 기준점이 될 트랜스폼 (플레이어같은)
	CTransform* m_pChildTransform = { nullptr };	// 애니메이션 - 실제 움직일 것의 트랜스폼 (무기같은)

public:
	static CAnimator* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

END