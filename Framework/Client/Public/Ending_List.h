#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
#include "Field_Npc_Chat.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEnding_List final : public CUIObject
{
private:
	CEnding_List(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnding_List(const CEnding_List& Prototype);
	virtual							~CEnding_List() = default;

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
	static CEnding_List* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END
