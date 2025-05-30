#pragma once
#include "Animation.h"


BEGIN(Engine)

class CAnim_Player_Attack2 final : public CAnimation
{
private:
	CAnim_Player_Attack2();
	virtual ~CAnim_Player_Attack2() = default;

public:
	HRESULT Initialize();

	static CAnim_Player_Attack2* Create();
	void Free();
	
};

END
