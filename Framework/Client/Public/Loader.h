#pragma once

#include "Client_Defines.h"
#include "Base.h"

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader final : public CBase
{
private:
	CLoader(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader() = default;

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	HRESULT Loading();

	_bool isFinished() {
		return m_isFinished;
	}

	void Show_LoadingText() {
		SetWindowText(g_hWnd, m_szLoadingText);
	}

private:
	LPDIRECT3DDEVICE9				m_pGraphic_Device = { nullptr };
	LEVEL							m_eNextLevelID = { LEVEL::LEVEL_END };
	HANDLE							m_hThread = {};
	_tchar							m_szLoadingText[MAX_PATH] = {};
	_bool							m_isFinished = { false };
	CGameInstance*					m_pGameInstance = { nullptr };
	CRITICAL_SECTION				m_CriticalSection = {};

private:
	HRESULT Loading_For_Logo_Level();
	HRESULT Loading_For_GamePlay_Level();
	HRESULT Loading_For_MapEdit_Level();
	HRESULT Loading_For_Stage1_Level();

public:
	static CLoader* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;
};

END