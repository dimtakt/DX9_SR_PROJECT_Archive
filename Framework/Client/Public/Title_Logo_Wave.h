#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CShader;
class CVIBuffer_Rect;
class CAnimator;
END

BEGIN(Client)

class CTitle_Logo_Wave final : public CUIObject
{
private:
	CTitle_Logo_Wave(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTitle_Logo_Wave(const CTitle_Logo_Wave& Prototype);
	virtual ~CTitle_Logo_Wave() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom_Title_Logo_Wave = { nullptr };
	CShader* m_pShaderCom_Title_Logo_Wave = { nullptr };

private:
	HRESULT Ready_Components();
	void SetUp_RenderState();
	void Reset_RenderState();

public:
	static CTitle_Logo_Wave* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END