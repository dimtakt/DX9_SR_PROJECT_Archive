#pragma once
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL IEventListener
{
public:
    virtual void OnEvent(_uint iTypeindex, const EVENTDATA* pData) = 0;
    virtual ~IEventListener() = default;
};

END