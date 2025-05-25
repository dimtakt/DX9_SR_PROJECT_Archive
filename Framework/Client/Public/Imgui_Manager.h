#pragma once

#include "Client_Defines.h"
#include "Engine_Defines.h"


#ifdef new
#undef new
#endif
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include "imgui_impl_win32.h"


BEGIN(Client)

class CImgui_Manager
{
private:
	CImgui_Manager();
public:
	~CImgui_Manager();
public:
	void Render_Begin();
	void Render();
	void Render_End();
public:
	HRESULT Initialize(HWND hWnd, LPDIRECT3DDEVICE9 pOut);
	static CImgui_Manager* Create(HWND hWnd, LPDIRECT3DDEVICE9 pOut);
};

END