#include "Font.h"

USING(Engine)

CFont::CFont(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device(pGraphic_Device)
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CFont::Ready_Font(const _wstring& strFontPath, const _wstring& strFontName,
    const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
    m_strFontPath = strFontPath;

    if (AddFontResourceEx(strFontPath.c_str(), FR_PRIVATE, 0) == 0)
    {
        MSG_BOX(L"AddFontResourceEx Failed");
        return E_FAIL;
    }

    D3DXFONT_DESC tFontDesc{};
    tFontDesc.CharSet = HANGEUL_CHARSET;
    tFontDesc.Width = iWidth;
    tFontDesc.Height = iHeight;
    tFontDesc.Weight = iWeight;
    lstrcpy(tFontDesc.FaceName, strFontName.c_str());

    if (FAILED(D3DXCreateFontIndirect(m_pGraphic_Device, &tFontDesc, &m_pFont)))
    {
        MSG_BOX(L"Font Create Failed");
        return E_FAIL;
    }

    if (FAILED(D3DXCreateSprite(m_pGraphic_Device, &m_pSprite)))
    {
        MSG_BOX(L"Sprite Create Failed");
        return E_FAIL;
    }

    return S_OK;
}

void CFont::Render_Font(const _wstring& strText, const _float2* pVec2Pos,
    D3DXCOLOR d3dxColor, DWORD dwFormat)
{
    RECT tRect = { (_long)pVec2Pos->x, (_long)pVec2Pos->y, 0, 0 };

    m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
    m_pFont->DrawTextW(m_pSprite, strText.c_str(), -1, &tRect, dwFormat, d3dxColor);
    m_pSprite->End();
}

CFont* CFont::Create(LPDIRECT3DDEVICE9 pGraphicDev,
    const _wstring& strFontPath, const _wstring& strFontName,
    const _uint& iWidth, const _uint& iHeight, const _uint& iWeight)
{
    CFont* pInstance = new CFont(pGraphicDev);

    if (FAILED(pInstance->Ready_Font(strFontPath, strFontName, iWidth, iHeight, iWeight)))
    {
        Safe_Release(pInstance);
        MSG_BOX(L"CFont Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CFont::Free()
{
    if (!m_strFontPath.empty())
        RemoveFontResourceEx(m_strFontPath.c_str(), FR_PRIVATE, 0);

    Safe_Release(m_pFont);
    Safe_Release(m_pSprite);
    Safe_Release(m_pGraphic_Device);
}
