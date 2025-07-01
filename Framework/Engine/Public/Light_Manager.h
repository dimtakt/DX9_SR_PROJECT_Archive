#pragma once
#include "Base.h"
#include "Shader.h"
BEGIN(Engine)

class CLight_Manager final : public CBase
{
private:
	CLight_Manager() = default;
	virtual ~CLight_Manager() = default;

public:
    void Add_Light(const _wstring& strID, const LIGHTDATA& data);
    void Remove_Light(const _wstring& strID);
    const unordered_map<_wstring, LIGHTDATA>& Get_Lights() const;
    const LIGHTDATA* Get_Light(const _wstring& strID) const;
    void Update_LightDirection(const _wstring& strID, const D3DXVECTOR3& vDir);
    void Update_LightPosition(const _wstring& strID, const D3DXVECTOR3& vPos);
    void Apply_ToShader(CShader* pShader, const vector<_wstring>& vecKeys);

private:
    unordered_map<_wstring, LIGHTDATA> m_LightMap;

public:
	static CLight_Manager* Create();
	virtual void Free() override;
};

END
