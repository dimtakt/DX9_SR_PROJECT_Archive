#pragma once
#include "Monster.h"
#include "Client_Defines.h"
#include "BossHp_Ema.h"
#include "Field_Npc_Chat.h"
BEGIN(Client)


class CErma_Body;
class CErma_Hand_L;
class CErma_Hand_R;
class CErma_Head;


class CErma final : public CMonster
{
public:
	typedef struct tagAZPatternDesc
	{
		_bool		isPatternStart		= false;	// 패턴이 시작되는 1프레임에만 트리거성으로 true가 됩니다.
		vector<int>	vecRequiredInputs	= {};		// 입력해야 하는 키의 벡터입니다.
		vector<int> vecInputs			= {};		// 현재 입력된 키입니다. 입력해야 하는 키가 바뀌거나, 패턴 종료 시 비워집니다.
		_bool		isSuccess			= false;	// 패턴 파훼에 성공한 순간부터 다음 패턴 시작 전까지 true 가 됩니다.
		_float		fLeftTime			= 0;		// 남은 시간을 의미합니다. (초 단위)
	}AZPATTERN_DESC;

private:
	CErma(LPDIRECT3DDEVICE9 pGraphic_Device);
	CErma(const CErma& Prototype);
	virtual ~CErma() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components(void* pArg);
	HRESULT Ready_Object();
	HRESULT Ready_Chat();

	void PlayKeyInputPattern();
	void Set_AllPartsStop(_bool isStop = true);

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

	void ChangeKeyInputPattern() { m_isTriggerKeyPattern = true; }

	AZPATTERN_DESC Get_AZPatternDesc()
	{
		return AZPATTERN_DESC{
			m_isTriggerKeyPattern_Activated,
			m_vecOriginKeys,
			m_vecInputKeys,
			m_isTriggerSuccess,
			m_iPauseLeftFrame / 60.f
		};
	}

private:
	// 각종 컴포넌트들
	CTexture*	m_pTextureCom_Idle				= { nullptr };
	CTexture*	m_pTextureCom_Move				= { nullptr };
	CTexture*	m_pTextureCom_Airborne			= { nullptr };
	CTexture*	m_pTextureCom_Down				= { nullptr };
	CTexture*	m_pTextureCom_Down_End			= { nullptr };
	CTexture*	m_pTextureCom_Stun				= { nullptr };
	CTexture*	m_pTextureCom_Enter_Progress	= { nullptr };
	CTexture*	m_pTextureCom_Enter_End			= { nullptr };
	
	CTransform* m_pTerrainTransformCom			= { nullptr };
	
	CAnimator*	m_pAnimatorCom					= { nullptr };
	

	// 본체가 컨트롤할 것.
	CErma_Body* m_pObj_Body					= { nullptr };
	CErma_Hand_L* m_pObj_Hand_L				= { nullptr };
	CErma_Hand_R* m_pObj_Hand_R				= { nullptr };
	CErma_Head* m_pObj_Head					= { nullptr };

	CBossHp_Ema* m_pBossHp					= { nullptr };
	CField_Npc_Chat* m_pChat				= { nullptr };
	
	
	// 로컬 변수들

	_int		m_iPhase						= 0;
	_int		m_iPattern						= 0;

	_bool		m_isEntering					= false;
	_bool		m_isInCombat					= false;

	_int		m_iStackedFrame					= 0;
	_int		m_iPauseLeftFrame				= 0;
	vector<int>	m_vecKeys						= {};		// 입력해야 하는 키
	vector<int>	m_vecOriginKeys					= {};		// 입력해야 하는 키 (제거되지 않은 원본)
	vector<int>	m_vecInputKeys					= {};		// 입력한 키
	_bool		m_isTriggerSuccess				= false;

	_int		m_iPatternRandOffset			= 0;

	_bool		m_isAllStop						= false;
	_bool		m_isTriggerKeyPattern			= false;
	_bool		m_bStart						= false;
	_int		m_iChatCount					= 0;
	_int		m_iCulChatCount					= 0;


	_bool		m_isTriggerKeyPattern_Activated = false;


public:
	static CErma* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END