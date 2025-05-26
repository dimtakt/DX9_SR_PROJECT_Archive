#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CPlayerStats final : public CComponent
{
public:
	typedef struct tagPlayerStatsDesc
	{
		_wstring	strName;
		_int		iLevel;

		_float		fMoveSpeed;
		_float		fAtkSpeed;

		_int		iExp;
		_int		iMaxExp;

		_float		fHp;
		_float		fMaxHp;
		_float		fHpRegen;
		
		_float		fMp;
		_float		fMaxMp;
		_float		fMpRegen;
		
		_float		fDash;		//플레이어 현재 대쉬 개수
		_float		fMaxDash;	//캐릭터 최대 대쉬 개수
		_float		fDashRegen; // 플레이어 대쉬 회복 속도

		_float		fEvade;
		_float		fDef;

		_float		fExpMultiply;
		_float		fGoldMultiply;
	}PLAYERSTAT_DESC;

private:
	CPlayerStats(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayerStats(const CPlayerStats& Prototype);
	virtual ~CPlayerStats() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	PLAYERSTAT_DESC Get_Stats() const{
		return tPlayerStats;
	}

	void Set_Stats(PLAYERSTAT_DESC _stats) {
		tPlayerStats = _stats;
	}

private:
	PLAYERSTAT_DESC tPlayerStats = {};

public:
	static CPlayerStats* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};


END
