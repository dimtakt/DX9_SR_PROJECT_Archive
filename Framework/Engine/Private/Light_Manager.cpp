#include "Light_Manager.h"

void CLight_Manager::Add_Light(const _wstring& strID, const LIGHTDATA& data)
{
	m_LightMap[strID] = data;
}

void CLight_Manager::Remove_Light(const _wstring& strID)
{
	m_LightMap.erase(strID);
}

const unordered_map<_wstring, LIGHTDATA>& CLight_Manager::Get_Lights() const
{
	return m_LightMap;
}

const LIGHTDATA* CLight_Manager::Get_Light(const _wstring& strID) const
{
	auto it = m_LightMap.find(strID);
	if (it != m_LightMap.end())
		return &it->second;
	return nullptr;
}

void CLight_Manager::Update_LightDirection(const _wstring& strID, const D3DXVECTOR3& vDir)
{
	if (m_LightMap.count(strID))
		m_LightMap[strID].vDirection = vDir;
}

void CLight_Manager::Update_LightPosition(const _wstring& strID, const D3DXVECTOR3& vPos)
{
	if (m_LightMap.count(strID))
		m_LightMap[strID].vPosition = vPos;
}

void CLight_Manager::Apply_ToShader(CShader* pShader, const vector<_wstring>& vecKeys)
{
    int i = 0;
    for (const auto& strID : vecKeys)
    {
        if (i >= 8)
            break;

        auto it = m_LightMap.find(strID);
        if (it == m_LightMap.end())
            continue;

        const LIGHTDATA& light = it->second;

        wstring base = L"gLights[" + std::to_wstring(i) + L"]";

        _float4 vPos, vDir;

        vPos = _float4(light.vPosition, 1.f);
        vDir = _float4(light.vDirection, 1.f);

        pShader->Set_Vector((base + L".vPosition").c_str(), &vPos);
        pShader->Set_Vector((base + L".vDirection").c_str(), &vDir);
        pShader->Set_Vector((base + L".vDiffuse").c_str(), &light.desc.vDiffuse);
        pShader->Set_Vector((base + L".vSpecular").c_str(), &light.desc.vSpecular);
        pShader->Set_Vector((base + L".vAmbient").c_str(), &light.desc.vAmbient);
        pShader->Set_Float((base + L".fSpecPower").c_str(), light.desc.fSpecPower);
        pShader->Set_Float((base + L".fRange").c_str(), light.desc.fRange);
        pShader->Set_Int((base + L".eType").c_str(), static_cast<int>(light.desc.eType));

        ++i;
    }

    pShader->Set_Int(L"gLightCount", i);
}

CLight_Manager* CLight_Manager::Create()
{
	return new CLight_Manager();
}

void CLight_Manager::Free()
{
	__super::Free();
}
