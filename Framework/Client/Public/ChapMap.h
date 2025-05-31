#pragma once
#include "Client_Defines.h"
#include "Client_Defines_Map.h"
#include "Button.h"

BEGIN(Client)

class CChapMap final : public CButton
{
private:
									CChapMap(LPDIRECT3DDEVICE9 pGraphic_Device);
									CChapMap(const CChapMap& Prototype);
	virtual							~CChapMap() = default;

public:
	virtual HRESULT					Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT					Initialize(void* pArg) override;
	virtual void					Priority_Update(_float fTimeDelta) override;
	virtual void					Update(_float fTimeDelta) override;
	virtual void					Late_Update(_float fTimeDelta) override;
	virtual HRESULT					Render() override;

public:
	void							Player_Offset(_float fX, _float fY, _uint iLineIndex);
	const _uint						Get_Player_Line() { return m_iPlayerLineIndex; };
private:
	LEVEL							m_eLevel = {};
	_bool							m_bRender = {};

	_int							m_iScrollValue = {};
	_int							m_iMouseY = {};

	class CChapMap_PlayerSymbol*	m_pPlayerSymbol = { nullptr};
	_uint							m_iPlayerLineIndex = {};

private:
	void							Scroll_Map();

private:
	HRESULT							Ready_Components();

	HRESULT							Ready_ChildPrototype(LEVEL eLevel);
	HRESULT							Ready_Children();
		
public:
	static CChapMap*				Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*			Clone(void* pArg) override;
	virtual void					Free();

};
END