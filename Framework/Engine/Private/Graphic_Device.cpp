#include "../Public/Graphic_Device.h"

CGraphic_Device::CGraphic_Device()
	: m_pSDK(nullptr)
	, m_pGraphicDev(nullptr)
{

}


HRESULT CGraphic_Device::Ready_GraphicDev(HWND hWnd, WINMODE eMode, const int iSizeX, const int iSizeY, LPDIRECT3DDEVICE9* ppOut)
{
	m_pSDK = Direct3DCreate9(D3D_SDK_VERSION);
	if (NULL == m_pSDK)
		return E_FAIL;

	D3DCAPS9		DeviceCaps;
	ZeroMemory(&DeviceCaps, sizeof(D3DCAPS9));

	if (FAILED(m_pSDK->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &DeviceCaps)))
	{
		return E_FAIL;
	}

	_ulong	dwFlag = 0;

	if (DeviceCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
		dwFlag |= D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	else
		dwFlag |= D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;

	D3DPRESENT_PARAMETERS		d3dpp;
	ZeroMemory(&d3dpp, sizeof(d3dpp));

	Set_Parameters(d3dpp, hWnd, eMode, iSizeX, iSizeY);

	if (FAILED(m_pSDK->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, dwFlag, &d3dpp, &m_pGraphicDev)))
	{
		return E_FAIL;
	}


	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	{
		return E_FAIL;
	}

	*ppOut = m_pGraphicDev;

	Safe_AddRef(m_pGraphicDev);

	return S_OK;
}
 
void CGraphic_Device::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphicDev->Clear(0,
		NULL,
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		Color,
		1.f,
		0);

	m_pGraphicDev->BeginScene();
	//m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

}

void CGraphic_Device::Render_End(HWND hWnd)
{
	//m_pSprite->End();
	m_pGraphicDev->EndScene();
	m_pGraphicDev->Present(nullptr, nullptr, hWnd, nullptr);
}

void CGraphic_Device::Set_Parameters(D3DPRESENT_PARAMETERS& d3dpp,
	HWND hWnd, WINMODE eMode, const int iSizeX, const int iSizeY)
{
	d3dpp.BackBufferWidth = iSizeX;
	d3dpp.BackBufferHeight = iSizeY;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;

	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;

	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;

	d3dpp.hDeviceWindow = hWnd;

	d3dpp.Windowed = static_cast<BOOL>(eMode);

	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;

	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
}

CGraphic_Device* CGraphic_Device::Create(HWND hWnd, WINMODE eMode, const int iSizeX, const int iSizeY, LPDIRECT3DDEVICE9* ppOut)
{
	CGraphic_Device* pInstance = new CGraphic_Device();

	if (FAILED(pInstance->Ready_GraphicDev(hWnd, eMode, iSizeX, iSizeY, ppOut)))
	{
		MSG_BOX(TEXT("Failed to Created : CGraphic_Device"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGraphic_Device::Free()
{
	Safe_Release(m_pSprite);
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pSDK);
}
