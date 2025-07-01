#pragma once
#include "UIObject.h"
#include "Client_Defines.h"
#include "Field_Npc_Chat.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CEnding_Mole final : public CUIObject
{
private:
	CEnding_Mole(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEnding_Mole(const CEnding_Mole& Prototype);
	virtual							~CEnding_Mole() = default;

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

	_float							m_fAccTime = {};
	_uint							m_iTexindex = {};
	_bool							m_bIsReturn = {};
	CField_Npc_Chat*				m_pChat;

private:
	HRESULT							Ready_Components();
	HRESULT							Ready_Object();

public:
	static CEnding_Mole* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void					Free() override;
};
END
