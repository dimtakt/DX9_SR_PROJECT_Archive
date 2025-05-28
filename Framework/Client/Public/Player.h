#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "TerrainBox.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CPlayerStats;
class CAnimator;
END

BEGIN(Client)

class CPlayer final : public CGameObject
{
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	void Change_TerrainBox(CTerrainBox* pTerrainBox)
	{
		m_pTerrainBox = pTerrainBox;
	}

private:
	CVIBuffer_Rect*			m_pVIBufferCom					= { nullptr };
	CTransform*				m_pTransformCom					= { nullptr };

	CTexture*				m_pTextureCom_Roll				= { nullptr };
	CTexture*				m_pTextureCom_Air				= { nullptr };
	CTexture*				m_pTextureCom_Down				= { nullptr };
	CTexture*				m_pTextureCom_Idle_Lower		= { nullptr };
	CTexture*				m_pTextureCom_Idle_Upper		= { nullptr };
	CTexture*				m_pTextureCom_Move_Lower		= { nullptr };
	CTexture*				m_pTextureCom_Move_Upper		= { nullptr };
	CTexture*				m_pTextureCom_Attack_Lower		= { nullptr };
	CTexture*				m_pTextureCom_Attack_Upper		= { nullptr };
	CTexture*				m_pTextureCom_GreatSwordHeavyAttack_Lower	= { nullptr };
	CTexture*				m_pTextureCom_GreatSwordHeavyAttack_Upper	= { nullptr };
	CTexture*				m_pTextureCom_WhirlWind_Ready	= { nullptr };
	CTexture*				m_pTextureCom_WhirlWind_Cycle	= { nullptr };

	CPlayerStats*			m_pPlayerStatsCom				= { nullptr };
	CAnimator*				m_pAnimatorCom					= { nullptr };

	_bool					m_isFlippedX					= false;

	CTerrainBox*			m_pTerrainBox					= { nullptr };
	
private:
	HRESULT Ready_Components();
	void SetUp_RenderState();
	void Reset_RenderState();

public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END
