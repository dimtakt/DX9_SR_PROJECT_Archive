#pragma once
#include "Animation.h"


BEGIN(Engine)

class CAnim_Player_Fury final : public CAnimation
{
private:
	CAnim_Player_Fury();
	virtual ~CAnim_Player_Fury() = default;

public:
	HRESULT Initialize();

	static CAnim_Player_Fury* Create();
	void Free();

};

END
