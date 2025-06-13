#pragma once
#include "UIObject.h"
#include "Client_Defines.h"

BEGIN(Client)
class CEvent_ClashPattern final : public CUIObject
{
public:
	enum class CLASH_RESULT { CLASH_PLAYING, CLASH_CLEAR, CLASH_FAIL, CLASH_NONPLAYING};

private:
	typedef struct Circle_Spawn_DB
	{
		_float	m_fDelayTime;
		_int	m_iSpawnX;
		_int	m_iSpawnY;
	public:
		Circle_Spawn_DB(_float fDelayTime, _int iSpawnX, _int iSpawnY) : m_fDelayTime(fDelayTime), m_iSpawnX(iSpawnX), m_iSpawnY(iSpawnY) {}
	};
private:
										CEvent_ClashPattern(LPDIRECT3DDEVICE9 pGraphic_Device);
										CEvent_ClashPattern(const CEvent_ClashPattern& Prototype);
	virtual								~CEvent_ClashPattern() = default;

public:
	CLASH_RESULT						Get_Event_Result() { return m_eCLASH_RESULT; }
	void								Start_Event();

public:
	virtual HRESULT						Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT						Initialize(void* pArg) override;
	virtual void						Priority_Update(_float fTimeDelta) override;
	virtual void						Update(_float fTimeDelta) override;
	virtual void						Late_Update(_float fTimeDelta) override;
	virtual HRESULT						Render() override;

private:
	LEVEL								m_eLevel = {};
	CLASH_RESULT						m_eCLASH_RESULT = { CLASH_RESULT::CLASH_NONPLAYING };

	_bool								m_bIsPlaying = {false};
	_bool								m_bIsSpawn = {false};
	_bool								m_bIsKeyInput = {false};
	_bool								m_bIsGameEnd = {false};

	vector<vector<Circle_Spawn_DB>>		m_vecSpawner;
	vector<Circle_Spawn_DB>				m_vecPattern_1;
	vector<Circle_Spawn_DB>				m_vecPattern_2;
	vector<Circle_Spawn_DB>				m_vecPattern_3;

	_int								m_iSpawnerIndex = {};
	_int								m_iSpawnCount = {};

	_float								m_fAccTime = {};
	_float								m_fEndAccTime = {};

	_int								m_iInputKey_Index = {};
	

private:
	HRESULT								Ready_Components();

	HRESULT								Ready_ChildPrototype(LEVEL eLevel);
	HRESULT								Ready_Children();

	void								Circle_Spawn(_float fTimeDelta);
	void								Patten_Set();
	void								Next_KeyInput();
	void								Result_Check();
	void								Reset_Event();

	void								End_Event();
public:
	static CEvent_ClashPattern*			Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject*				Clone(void* pArg) override;
	virtual void						Free() override;
};
END
