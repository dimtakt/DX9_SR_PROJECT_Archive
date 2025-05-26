#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CShader;
END

BEGIN(Client)

class CTitle_Star final : public CUIObject
{
private:
	CTitle_Star(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTitle_Star(const CTitle_Star& Prototype);
	virtual ~CTitle_Star() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom_Title_Star = { nullptr };
	CShader* m_pShaderCom_Title_Star = { nullptr };

private:
	HRESULT Ready_Components();
	void SetUp_RenderState();
	void Reset_RenderState();

public:
	static CTitle_Star* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END