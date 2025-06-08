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

class CInteraction_Normal final : public CGameObject
{
private:
	CInteraction_Normal(LPDIRECT3DDEVICE9 pGraphic_Device);
	CInteraction_Normal(const CInteraction_Normal& Prototype);
	virtual ~CInteraction_Normal() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	void SetUp_RenderState();
	void Reset_RenderState();


#pragma region 경험치 뭉치
public:
	HRESULT EXP_Initialize();
	HRESULT EXP_Priority_Update(_float fTimeDelta);
	HRESULT EXP_Update(_float fTimeDelta);
	HRESULT EXP_Late_Update(_float fTimeDelta);
	HRESULT EXP_Render();
	HRESULT EXP_Component();

#pragma endregion

#pragma region 골드상자
public:
	HRESULT Gold_Initialize();
	HRESULT Gold_Priority_Update(_float fTimeDelta);
	HRESULT Gold_Update(_float fTimeDelta);
	HRESULT Gold_Late_Update(_float fTimeDelta);
	HRESULT Gold_Render();
	HRESULT Gold_Component();
#pragma endregion

#pragma region 최대 체력 증가
public:
	HRESULT HP_Initialize();
	HRESULT HP_Priority_Update(_float fTimeDelta);
	HRESULT HP_Update(_float fTimeDelta);
	HRESULT HP_Late_Update(_float fTimeDelta);
	HRESULT HP_Render();
	HRESULT HP_Component();
#pragma endregion

#pragma region 석판
public:
	HRESULT Stone_Tablet_Initialize();
	HRESULT Stone_Tablet_Priority_Update(_float fTimeDelta);
	HRESULT Stone_Tablet_Update(_float fTimeDelta);
	HRESULT Stone_Tablet_Late_Update(_float fTimeDelta);
	HRESULT Stone_Tablet_Render();
	HRESULT Stone_Tablet_Component();
#pragma endregion

#pragma region 아티팩트
public:
	HRESULT Atifact_Initialize();
	HRESULT Atifact_Priority_Update(_float fTimeDelta);
	HRESULT Atifact_Update(_float fTimeDelta);
	HRESULT Atifact_Late_Update(_float fTimeDelta);
	HRESULT Atifact_Render();
	HRESULT Atifact_Component();
#pragma endregion

#pragma region 상인
public:
	HRESULT Merchant_Initialize();
	HRESULT Merchant_Priority_Update(_float fTimeDelta);
	HRESULT Merchant_Update(_float fTimeDelta);
	HRESULT Merchant_Late_Update(_float fTimeDelta);
	HRESULT Merchant_Render();
	HRESULT Merchant_Component();
#pragma endregion

private:
	CVIBuffer_Rect* m_pVIBufferCom_0 = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom_0 = { nullptr };
	CTexture* m_pTextureCom_1 = { nullptr };
	CAnimator* m_pAnimatorCom_0 = { nullptr };

	_bool m_bActive = { TRUE };
	_bool m_bUsing = { FALSE };

private:
	HRESULT Ready_Components();

public:
	virtual void OnCollision(CGameObject* pGameObject) override;

public:
	static CInteraction_Normal* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END