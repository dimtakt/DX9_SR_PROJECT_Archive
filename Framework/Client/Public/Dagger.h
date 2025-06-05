#pragma once
#include "Client_Defines.h"
#include "GameObject.h"
#include "Collider_OBB.h"
BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimator;
END

BEGIN(Client)

class CDagger final : public CGameObject
{
public:
	typedef struct tagDaggerDesc
	{
		_uint iLayerIndex;
	}DAGGERDESC;
private:
	CDagger(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDagger(const CDagger& Prototype);
	virtual ~CDagger() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

private:
	CVIBuffer_Rect*			m_pVIBufferCom					= { nullptr };
	CTransform*				m_pTransformCom					= { nullptr };
	CTexture*				m_pTextureCom_Normal			= { nullptr };
	CTexture*				m_pTextureCom_FuryReady			= { nullptr };
	CAnimator*				m_pAnimatorCom					= { nullptr };

	_bool					m_isFlippedX					= false;

	CTransform*				m_pTargetTransformCom			= { nullptr };
	DAGGERDESC				m_tDesc = {};

private:
	HRESULT	Ready_Components(void* pArg);
	void SetUp_RenderState();
	void Reset_RenderState();

	_float m_fCurrentAngle = 0;
	_float m_fCurCursorDeg = 0;

private:
	void Follow_Player();
	void Look_At_Cursor();

public:
	static CDagger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END