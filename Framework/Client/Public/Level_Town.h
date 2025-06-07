#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Town final : public CLevel
{
private:
	CLevel_Town(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Town() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Light(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Room(const _wstring& strLayerTag);

public:
	static CLevel_Town* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END