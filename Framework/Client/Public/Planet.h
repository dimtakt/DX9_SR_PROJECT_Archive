#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CAnimator;
class CTransform;
END

BEGIN(Client)

class CPlanet final : public CGameObject
{
public:
	typedef struct tagPlanetDesc
	{
		CTransform* pTargetTransform;
	}PLANETDESC;
private:
	CPlanet(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlanet(const CPlanet& Prototype);
	virtual ~CPlanet() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void SetUp_RenderState();
	void Reset_RenderState();

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };
	CAnimator* m_pAnimatorCom = { nullptr };
	CTransform* m_pTargetTransform = { nullptr };

	_float m_fAttackFrame = {0.f};

private:
	HRESULT Ready_Components(void* pArg);

public:
	static CPlanet* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END