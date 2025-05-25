#include "Font_Manager.h"
#include "Font.h"

CFont_Manager::CFont_Manager(LPDIRECT3DDEVICE9 pGraphic_Device)
    :m_pGraphic_Device { pGraphic_Device }
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CFont_Manager::Ready_Font(const _wstring& strFontTag, 
    const _wstring& strFontPath, 
    const _wstring& strFontName, 
    const _uint& iWidth,
    const _uint& iHeight, 
    const _uint& iWeight)
{
    if (Find_Font(strFontTag) != nullptr)
        return E_FAIL;

    CFont* pFont = CFont::Create(m_pGraphic_Device, strFontPath, strFontName, iWidth, iHeight, iWeight);
    NULL_CHECK_RETURN(pFont, E_FAIL);

    m_mapFont.insert({ strFontTag, pFont });

    return S_OK;
}

void CFont_Manager::Render_Font(const wstring& strFontTag,
    const _wstring& strText,
    const _float2* pVec2Pos,
    D3DXCOLOR d3dxColor,
    DWORD dwFormat)
{
    CFont* pFont = Find_Font(strFontTag);
    NULL_CHECK(pFont);

    pFont->Render_Font(strText, pVec2Pos, d3dxColor, dwFormat);
}

CFont* CFont_Manager::Find_Font(const wstring& strFontTag)
{
    auto iter = m_mapFont.find(strFontTag);
    return (iter != m_mapFont.end()) ? iter->second : nullptr;
}

CFont_Manager* CFont_Manager::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    return new CFont_Manager(pGraphic_Device);
}

void CFont_Manager::Free()
{
    __super::Free();
    for (auto& pair : m_mapFont)
        Safe_Release(pair.second);

    m_mapFont.clear();
    Safe_Release(m_pGraphic_Device);
}
