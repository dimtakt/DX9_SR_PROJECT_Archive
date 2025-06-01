#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

BEGIN(Engine)
class CTexture;
class CVIBuffer_Rect;
class CAnimator;
END

BEGIN(Client)

class CStart_Button final : public CUIObject
{
private:
	CStart_Button(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStart_Button(const CStart_Button& Prototype);
	virtual ~CStart_Button() = default;

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
	static CStart_Button* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

END