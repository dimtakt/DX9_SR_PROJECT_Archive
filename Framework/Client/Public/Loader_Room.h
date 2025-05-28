#pragma once

#include "Client_Defines.h"
#include "Base.h"

/* 로고, 게임플레이 -> 객체, 리소스들, 사운드, 쉐이더 ->자원준비를 거치는 과정 => 로딩 */

BEGIN(Engine)
class CGameInstance;
END

BEGIN(Client)

class CLoader_Room final : public CBase
{
private:
	CLoader_Room(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLoader_Room() = default;

	/* 세마포어 , 뮤텍스, 크리티컬섹션 */

public:
	HRESULT Initialize(LEVEL eNextLevelID);
	HRESULT Loading();

	_bool isFinished() {
		return m_isFinished;
	}


private:
	LPDIRECT3DDEVICE9				m_pGraphic_Device = { nullptr };
	LEVEL							m_eNextLevelID = { LEVEL::LEVEL_END };
	HANDLE							m_hThread = {};
	_bool							m_isFinished = { false };
	CGameInstance*					m_pGameInstance = { nullptr };
	CRITICAL_SECTION				m_CriticalSection = {};



private:
	HRESULT Loading_For_Stage1_Room();
	


public:
	static CLoader_Room* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eNextLevelID);
	virtual void Free() override;
};

END