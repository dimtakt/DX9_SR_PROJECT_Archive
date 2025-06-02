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
	void							Open_Ui();		//UI 출력 함수

	void							Player_Offset(_float fX, _float fY, _uint iLineIndex, _uint iMapIdex);
	const _uint						Get_Player_Line() { return m_iPlayerLineIndex; };
	const _uint						Get_Player_Map() { return m_iPlayerMapIndex; };

private:
	LEVEL							m_eLevel = {};
	_bool							m_bRender = {};

	_int							m_iScrollValue = {};
	_int							m_iMouseY = {};

	class CChapMap_PlayerSymbol*	m_pPlayerSymbol = { nullptr};
	_uint							m_iPlayerLineIndex = {};
	_uint							m_iPlayerMapIndex = {};

	_bool							m_bIsNextLevel = { false };

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