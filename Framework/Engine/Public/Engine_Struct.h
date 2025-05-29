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

	typedef struct tagVertexPositionNormalTexcoord
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
