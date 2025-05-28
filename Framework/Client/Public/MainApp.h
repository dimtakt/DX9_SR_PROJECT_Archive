#pragma once

#include "Client_Defines.h"
#include "Client_Defines_Item.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CMainApp final : public CBase 
{
private:
	CMainApp();
	virtual ~CMainApp() = default;

public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
	HRESULT Render();

private:
	CGameInstance*		m_pGameInstance = { nullptr };
	LPDIRECT3DDEVICE9	m_pGraphic_Device = { nullptr };

private:
	HRESULT Ready_Default_Setting();
	HRESULT Ready_Prototype_ForStatic();
	void Ready_Key_Setting();
	HRESULT Ready_Item_Setting();
	HRESULT Start_Level(LEVEL eStartLevelID);
	HRESULT Ready_Manager_Setting();

public:
	static CMainApp* Create();
	virtual void Free() override;
};

END