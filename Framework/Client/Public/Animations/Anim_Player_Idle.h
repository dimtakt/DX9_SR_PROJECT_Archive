#pragma once
#include "Animation.h"


BEGIN(Engine)

class CAnim_Player_Idle final : public CAnimation
{
private:
	CAnim_Player_Idle();
	virtual ~CAnim_Player_Idle() = default;

public:
	HRESULT Initialize();

	static CAnim_Player_Idle* Create();
	void Free();
	
};

END
