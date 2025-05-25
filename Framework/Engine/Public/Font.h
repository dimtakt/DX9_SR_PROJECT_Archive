#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CFont final : public CBase
{
private:
    CFont(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual ~CFont() = default;

public:
    HRESULT Ready_Font(const _wstring& strFontPath, const _wstring& strFontName,
        const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);

    void Render_Font(const _wstring& strText, const _float2* pVec2Pos,
        D3DXCOLOR d3dxColor, DWORD dwFormat = DT_NOCLIP);

public:
    static CFont* Create(LPDIRECT3DDEVICE9 pGraphicDev,
        const _wstring& strFontPath, const _wstring& strFontName,
        const _uint& iWidth, const _uint& iHeight, const _uint& iWeight);
    virtual void Free();

private:
    LPDIRECT3DDEVICE9   m_pGraphic_Device = { nullptr };
    LPD3DXSPRITE        m_pSprite = { nullptr };
    LPD3DXFONT          m_pFont = { nullptr };
    _wstring             m_strFontPath;
};

END
