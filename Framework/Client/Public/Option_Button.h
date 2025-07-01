#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CAnimator;
END

BEGIN(Client)

class COption_Button final : public CUIObject
{
private:
	COption_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	COption_Button(const COption_Button& Prototype);
	virtual ~COption_Button() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;
	HRESULT Render_Font();

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	D3DXCOLOR		m_FontColor = {};
private:
	HRESULT Ready_Components();
	void SetUp_RenderState();
	void Reset_RenderState();

public:
	static COption_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END