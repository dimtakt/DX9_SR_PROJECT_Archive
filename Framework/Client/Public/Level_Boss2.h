#pragma once

#include "Client_Defines.h"
#include "Level.h"

BEGIN(Client)

class CLevel_Boss2 final : public CLevel
{
private:
	CLevel_Boss2(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_Boss2() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_UI(const _wstring& strLayerTag);

public:
	static CLevel_Boss2* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};

END