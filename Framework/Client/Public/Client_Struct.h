#pragma once
#include "Engine_Defines.h"

BEGIN(Client)
typedef struct tagMAPObjectDesc
{
	char szType[32];
	_int iTextureIndex = { 0 };
	_float3 vScale = { 1.f, 1.f, 1.f };
	_float3 vRotate = { 0.f, 0.f, 0.f };
	_float3 vPos = { 0.f, 0.f, 0.f };
}MAP_OBJECT_DESC;
END