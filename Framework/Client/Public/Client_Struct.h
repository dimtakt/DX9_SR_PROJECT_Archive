#pragma once
#include "Engine_Defines.h"

BEGIN(Client)
typedef struct tagMAPObjectDesc
{
	GAMEOBJ_TYPE eType;
	_float3 vScale = { 1.f, 1.f, 1.f };
	_float3 vRotate = { 0.f, 0.f, 0.f };
	_float3 vPos = { 0.f, 0.f, 0.f };
	_int iTextureIndex = { 0 };
}MAP_OBJECT_DESC;

END