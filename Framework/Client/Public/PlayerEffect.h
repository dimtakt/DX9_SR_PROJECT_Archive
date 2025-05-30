#pragma once
#include "Base.h"
#include "Effect.h"
#include "Client_Defines.h"

BEGIN(Client)

class CPlayerEffect final : public CEffect
{
private:
	Client::CPlayerEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerEffect(const CPlayerEffect& Prototype);
	virtual ~CPlayerEffect() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

private:
	HRESULT Ready_Components();

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CAnimator* m_pAnimatorCom = { nullptr };

public:
	static CGameObject* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg);
	void Free();
};

END