#include "Engine_Enum.h"
#ifndef Engine_Struct_h__
#define Engine_Struct_h__



namespace Engine
{
	typedef struct tagEngineDesc
	{
		HWND			hWnd;
		WINMODE			eWinMode;
		unsigned int	iWinSizeX, iWinSizeY;
		unsigned int	iNumLevels;
	}ENGINE_DESC;

	typedef struct tagVertexPosTex
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR2		vTexcoord;
	}VTXPOSTEX;
	
	typedef struct tagVertexPosCubeTex
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vTexcoord;
	}VTXCUBETEX;

	typedef struct tagVertexNorTex
	{
		D3DXVECTOR3		vPosition;
		D3DXVECTOR3		vNormal;
		D3DXVECTOR2		vTexcoord;
	}VTXNORTEX;

	typedef struct tagVertexPosColor
	{
		D3DXVECTOR3		vPosition;
		unsigned long	dwColor;
	}VTXPOSCOL;

	typedef struct tagLightDesc
	{
		LIGHT_TYPE eType;
		float padding1[3];              //

		D3DXVECTOR4 vDiffuse;
		D3DXVECTOR4 vSpecular;
		D3DXVECTOR4 vAmbient;

		float fRange;
		float fSpecPower = 32.f;

		int padding2[2];                //
	}LIGHTDESC;

	typedef struct tagLightData
	{
		D3DXVECTOR3 vPosition = { 0.f, 0.f, 0.f };
		D3DXVECTOR3 vDirection = { 0.f, -1.f, 0.f };
		LIGHTDESC desc;
	}LIGHTDATA;

	//////////////////////
	// Event
	typedef struct tagEventData
	{
		virtual ~tagEventData() = default;
	}EVENTDATA;



	//////////////////////
	// Network Struct
	
	typedef struct test
	{
		string	title;
		string	desc;
	}TEST;

	typedef struct tagUser
	{
		int	iId;
		string	sName;
		string	sNickname;
		string	sPassword;
	}USER;
}


#endif // Engine_Struct_h__
