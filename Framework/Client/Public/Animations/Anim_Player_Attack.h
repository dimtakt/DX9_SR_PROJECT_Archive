#pragma once
#include "Animation.h"


BEGIN(Engine)

class CAnim_Player_Attack final : public CAnimation
{
private:
	CAnim_Player_Attack();
	virtual ~CAnim_Player_Attack() = default;

public:
	HRESULT Initialize();

	static CAnim_Player_Attack* Create();
	void Free();
	
};

END
