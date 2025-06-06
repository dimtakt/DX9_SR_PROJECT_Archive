#pragma once
#include "Client_Defines.h"
#include "GameInstance.h"

BEGIN(Client)

class CStat_Manager final : public CBase
{
	DECLARE_SINGLETON(CStat_Manager)
public:
	CStat_Manager();
	~CStat_Manager() = default;
public:
	HRESULT Initialize();
	void Update(_float fTimeDelta);
public:
	const _float* Get_OriginStats() { return m_fOriginStats; }
	const _float* Get_CurStats() { return m_fCurStats; }
	void Cal_Stats(STAT_INFO eStat, float fValue);
	void Set_Stats(STAT_INFO eStat, float fValue);
	void Interaction_Obj_Stat(GAMEOBJ_TYPE eType);
	void Reset_CurStats(); // 원본값으로 복원시켜주는 함수
	_float Get_Damage(DAMAGE eDamage);


private:
	float m_fOriginStats[static_cast<int>(STAT_INFO::STAT_END)] = {};
	float m_fCurStats[static_cast<int>(STAT_INFO::STAT_END)] = {};
	CGameInstance* m_pGameInstance = { nullptr };

public:
	virtual void Free() override;

};

END