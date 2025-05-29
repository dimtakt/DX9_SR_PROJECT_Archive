#pragma once

#include "Base.h"

/* DirectX SDK에서 제공해주는 다양한 함수의 기능을 이용하기위해 초기화하는 과정. */
/* 초기화를 잘 거쳤다라면 IDirect3DDevice9 => LPDIRECT3DDEVICE9을 통해서 전반적인 함수의 호출을 수행해나가면 된다. */

BEGIN(Engine)

class CGraphic_Device : public CBase
{
private:
	explicit CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	HRESULT			Ready_GraphicDev(HWND hWnd, WINMODE eMode, const int iSizeX, const int iSizeY, LPDIRECT3DDEVICE9* ppOut);
	
	void			Render_Begin(D3DXCOLOR Color);
	void			Render_End(HWND hWnd = 0);
		
private:
	void			Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp, HWND hWnd, WINMODE eMode, const int iSizeX, const int iSizeY);

private:
	LPDIRECT3D9				m_pSDK;
	LPDIRECT3DDEVICE9		m_pGraphicDev;
	LPD3DXSPRITE			m_pSprite;

public:
	static CGraphic_Device* Create(HWND hWnd, WINMODE eMode, const int iSizeX, const int iSizeY, LPDIRECT3DDEVICE9* ppOut);
	virtual void	Free();

};

END