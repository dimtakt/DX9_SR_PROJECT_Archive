#pragma once

#include "GameInstance.h"

BEGIN(Engine)

class CKey_Manager final : public CBase
{
private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	HRESULT Initialize();
	void Update(float fDeltaTime);                    // 매 프레임 호출

public:
	void AddTrackingKey(int iKey);                         // 추가됨
	bool IsKeyDown(int iKey) const;   // 이번 프레임에 눌림
	bool IsKeyUp(int iKey) const;     // 이번 프레임에 떼짐
	bool IsKeyHold(int iKey) const;   // 계속 눌림
	float GetKeyHoldTime(int iKey) const; // 눌린 시간 (선택사항)


private:
	unordered_map<int, bool> m_mPrevState;
	unordered_map<int, bool> m_mCurrState;
	unordered_map<int, float> m_mKeyHoldTime;
	vector<int> m_vTrackingKeys;

public:
	static CKey_Manager* Create();
	virtual void Free() override;

};

END