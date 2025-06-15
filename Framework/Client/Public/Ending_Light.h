#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
#include "Field_Npc_Chat.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEnding_Light final : public CUIObject
{
private:
	CEnding_Light(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnding_Light(const CEnding_Light& Prototype);
	virtual							~CEnding_Light() = default;

public:
	virtual HRESULT					Initialize_Prototype() override;
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

private:
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };

	_uint							m_iTexindex = {};
	_bool							m_bIsReturn = {};

private:
	HRESULT							Ready_Components();
	void							Font_Render();

public:
	static CEnding_Light* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END
