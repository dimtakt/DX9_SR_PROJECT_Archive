#pragma once
#include "Client_Defines.h"
#include "Engine_Defines.h"
namespace Client
{
	typedef struct tagStatChangeEvent : public EVENTDATA
	{
		STAT_INFO	eStatType;
		float		fValue;
	}STATCHANGE;

	typedef struct tagRoomChangeEvent : public EVENTDATA
	{
		_float3		vPosition;
	}ROOMCHANGE;

	typedef struct tagActionEvent : public EVENTDATA
	{
		_wstring strActionName;
	};
}