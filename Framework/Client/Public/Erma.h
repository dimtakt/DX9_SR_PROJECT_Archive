#pragma once
#include "Monster.h"
#include "Client_Defines.h"


BEGIN(Client)


class CErma_Body;
class CErma_Hand_L;
class CErma_Hand_R;
class CErma_Head;


class CErma final : public CMonster
{
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

	void PlayKeyInputPattern();
	void Set_AllPartsStop(_bool isStop = true);

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

	void ChangeKeyInputPattern() { m_isTriggerKeyPattern = true; }

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


	
	// 로컬 변수들

	_int		m_iPhase						= 0;
	_int		m_iPattern						= 0;

	_bool		m_isEntering					= false;
	_bool		m_isInCombat					= false;

	_int		m_iStackedFrame					= 0;
	_int		m_iPauseLeftFrame				= 0;
	list<int>	m_listKeys						= {};

	_int		m_iPatternRandOffset			= 0;

	_bool		m_isAllStop						= false;
	_bool		m_isTriggerKeyPattern			= false;

public:
	static CErma* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;

};

END