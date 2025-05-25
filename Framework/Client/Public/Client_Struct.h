#pragma once
#include "Engine_Defines.h"

BEGIN(Client)
typedef struct tagMAPObjectDesc
{
	_int iIndex = { 0 };
	_float3 vScale = { 1.f, 1.f, 1.f };
	_float3 vRotate = { 0.f, 0.f, 0.f };
	_float3 vPos = { 0.f, 0.f, 0.f };
}MAP_OBJECT_DESC;
END