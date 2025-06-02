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
	HRESULT Ready_Static_Setting();
	HRESULT Ready_GameObject_Setting();
	HRESULT Ready_Texture_Setting();
	HRESULT Ready_Model_Setting();
	HRESULT Ready_ETC_Setting();
	HRESULT Ready_Animation_Setting();
	void Ready_Key_Setting();
	void Ready_Font_Setting();
	HRESULT Ready_Item_Setting();
	HRESULT Start_Level(LEVEL eStartLevelID);
	HRESULT Ready_Manager_Setting();
	HRESULT	Ready_UI_Stting();
public:
	static CMainApp* Create();
	virtual void Free() override;
};

END