#pragma once
#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CAnimator;
END

BEGIN(Client)

class CDagger final : public CGameObject
{
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

private:
	CVIBuffer_Rect*			m_pVIBufferCom					= { nullptr };
	CTransform*				m_pTransformCom					= { nullptr };
	CTexture*				m_pTextureCom					= { nullptr };
	CAnimator*				m_pAnimatorCom					= { nullptr };

	_bool					m_isFlippedX					= false;

	CTransform*				m_pTargetTransformCom			= { nullptr };

private:
	HRESULT	Ready_Components();
	void SetUp_RenderState();
	void Reset_RenderState();

	_float m_fCurrentAngle = 0;

private:
	void Follow_Player();

public:
	static CDagger* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END