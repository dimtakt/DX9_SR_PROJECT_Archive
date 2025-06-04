#pragma once
#include "Base.h"

BEGIN(Engine)
class ENGINE_DLL CUIObject_Manager final : public CBase
{
private:
													CUIObject_Manager();
	virtual											~CUIObject_Manager() = default;

public:
	HRESULT											Initialize(_uint iNumLevels);

	HRESULT											Add_UIObject(_uint iLevelIndex, const _wstring& strUITag, class CUIObject* pUIObj);
	void											Update_On(_uint iLevelIndex, const _wstring& strUITag);
	void											Update_Off(_uint iLevelIndex, const _wstring& strUITag);
	void											All_Update_On();
	void											All_Update_Off();

	void											Clear_UiObj(_uint iLevelIndex);

private:
	class CUIObject*								Find_UIObj(_uint iLevelIndex, const _wstring& strUITag);

private:
	class CGameInstance*							 m_pGameInstance = { nullptr };
	_uint											 m_iNumLevels = {};
	map<const _wstring, class CUIObject*>*			m_pUIObjects = {};
public:
	static CUIObject_Manager*						Create(_uint iNumLevels);
	virtual void									Free() override;
	
};

END