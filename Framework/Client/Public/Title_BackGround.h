#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CShader;
END

BEGIN(Client)

class CTitle_BackGround final : public CUIObject
{
private:
	CTitle_BackGround(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTitle_BackGround(const CTitle_BackGround& Prototype);
	virtual ~CTitle_BackGround() = default;

public:
	virtual HRESULT Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	LEVEL m_eLevel = {};
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom_Title_Back = { nullptr };
	CTexture* m_pTextureCom_Title_ReverseTree = { nullptr };
	CShader* m_pShaderCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Ready_ChildPrototype(LEVEL eLevel);
	HRESULT Ready_Children();

public:
	static CTitle_BackGround* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END