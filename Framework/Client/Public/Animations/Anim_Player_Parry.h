#pragma once
#include "Animation.h"


BEGIN(Engine)

class CAnim_Player_Parry final : public CAnimation
{
private:
	CAnim_Player_Parry();
	virtual ~CAnim_Player_Parry() = default;

public:
	HRESULT Initialize();

	static CAnim_Player_Parry* Create();
	void Free();

};

END
