#include "Level_MapEdit.h"
#include "Imgui_Manager.h"
#include "ImGuiFileDialog.h"
#include "ImGuiFileDialogConfig.h"
#include "Camera_Mouse.h"
#include "GameInstance.h"
#include "Layer.h"
#include "GameObject.h"
#include "Client_Struct.h"

#include "json.hpp"
#include "Tree.h"
#include "TerrainBox.h"



CLevel_MapEdit::CLevel_MapEdit(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CLevel(pGraphic_Device)
{
}

HRESULT CLevel_MapEdit::Initialize()
{
	if (FAILED(Ready_ImGui(g_hWnd, m_pGraphic_Device)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Terrain_Texture_Info()))
		return E_FAIL;

	if (FAILED(Ready_Texture_Info()))
		return E_FAIL;

	return S_OK;
}

void CLevel_MapEdit::Update(_float fTimeDelta)
{
	Picking_Check();
}

HRESULT CLevel_MapEdit::Render()
{
	SetWindowText(g_hWnd, TEXT("∏  ø°µ≈Õ ∑π∫ß ¿‘¥œ¥Ÿ"));
	//m_pImgui_Manage->Render();
	this->Imgui_Render();

	return S_OK;
}

HRESULT CLevel_MapEdit::Ready_ImGui(HWND hWnd, LPDIRECT3DDEVICE9 pOut)
{
	MessageBox(0, TEXT("Ready_ImGui Ω√¿€"), TEXT("Debug"), MB_OK);


	m_pImgui_Manage = CImgui_Manager::Create(hWnd, pOut);
	if (m_pImgui_Manage == nullptr)
	{
		MSG_BOX(TEXT("Failed to Created : CImgui_Manager"));
		return E_FAIL;
	}
	MessageBox(0, TEXT("Imgui ∏≈¥œ¿˙ ª˝º∫ º∫∞¯"), TEXT("Debug"), MB_OK);
	return S_OK;
}

HRESULT CLevel_MapEdit::Ready_Layer_Camera(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), strLayerTag,
		ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Prototype_GameObject_Camera_Mouse"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_MapEdit::Ready_Texture_Info()
{
	m_pPreview = static_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT,
		ENUM_CLASS(LEVEL::LEVEL_MAPEDIT),
		TEXT("Prototype_GameObject_Tree")));

	OBJECT_TEXTURE_INFO ObjectInfo;
	ObjectInfo.iTextureCount = 15;
	ObjectInfo.pTextureCom = static_cast<CTexture*>(m_pPreview->Find_Component(TEXT("Com_Texture")));
	if (ObjectInfo.pTextureCom)
		ObjectInfo.pTextureCom->AddRef();

	m_ObjectTextureInfo["Object"] = ObjectInfo;

	Safe_Release(m_pPreview);  // ≈ÿΩ∫√≥∏∏ ∞°¡ÆøÕº≠ ¿˙¿Â«œ∞Ì ªË¡¶

	return S_OK;
}

HRESULT CLevel_MapEdit::Ready_Terrain_Texture_Info()
{
	//¡ˆ«¸
	m_pPreview = static_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT,
		ENUM_CLASS(LEVEL::LEVEL_MAPEDIT),
		TEXT("Prototype_GameObject_TerrainBox")));

	OBJECT_TEXTURE_INFO TerrainBoxInfo;
	TerrainBoxInfo.iTextureCount = 25;
	TerrainBoxInfo.pTextureCom = static_cast<CTexture*>(m_pPreview->Find_Component(TEXT("Com_Texture_Terrain_Top")));
	if (TerrainBoxInfo.pTextureCom)
		TerrainBoxInfo.pTextureCom->AddRef();

	m_ObjectTextureInfo["TerrainBox"] = TerrainBoxInfo;

	Safe_Release(m_pPreview); // ≈ÿΩ∫√≥∏∏ ∞°¡ÆøÕº≠ ¿˙¿Â«œ∞Ì ªË¡¶

	return S_OK;
}

void CLevel_MapEdit::Imgui_Render()
{
	m_pImgui_Manage->Render_Begin();

	ImGui::Begin("Object Editer", nullptr, ImGuiWindowFlags_MenuBar);

	ImGui_MenuBar_Render();	//¿˙¿ÂøÎ πˆ∆∞

	ImGui_Option_Button_Reset();

	ImGui_Object_MenBar();	

	if (m_bPicking)
	{
		ImGui_Picking_Object_MenBar();	//º±≈√µ» ø¿∫Í¡ß∆Æ ¿¸øÎ º≥¡§ ∞™
	}

	ImGui::End();

	ImGui_Terrain_MenBar();  //¡ˆ«¸ ¿¸øÎ UI

	m_pImgui_Manage->Render_End();
}

void CLevel_MapEdit::ImGui_MenuBar_Render()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Open"))
			{
				IGFD::FileDialogConfig config;
				config.path = "../../data/";
				config.flags = ImGuiFileDialogFlags_ReadOnlyFileNameField;

				// ∆ƒ¿œ ¥Ÿ¿ÃæÛ∑Œ±◊ ø≠±‚
				// ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", config);
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".txt,.cpp,.h", config);
			}
			if (ImGui::MenuItem("Save"))
			{
				IGFD::FileDialogConfig config;
				config.path = "../../data/";
				config.flags = ImGuiFileDialogFlags_ConfirmOverwrite;

				ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Choose File", ".txt,.cpp,.h", config);

			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// ¿–±‚øÎ ∑Œ¡˜
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			ifstream ifs(filePath, ios::binary);

			if (ifs.is_open())
			{
				if (!m_pObject.empty())
				{
					for (auto pObj : m_pObject)
					{
						CGameObject* pGameObject = pObj;

						m_pGameInstance->Remove_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"), pGameObject);

						Safe_Release(pGameObject);
					}

					m_pPickingObject = nullptr;
					m_pObjectTransform = nullptr;
					m_bPicking = false;
					m_pObject.clear();
				}

				while (!ifs.eof())
				{
					MAP_OBJECT_DESC Desc = {};

					ifs.read(reinterpret_cast<char*>(&Desc), sizeof(MAP_OBJECT_DESC));	//Íµ¨Ï°∞Ï≤??¨Í∏∞ÎßåÌÅº ?åÏùº ?¥Î? ?ΩÏùå
					
					if (ifs.gcount() == sizeof(MAP_OBJECT_DESC))		//ÎßåÏïΩ Íµ¨Ï°∞Ï≤??¨Í∏∞ÎßåÌÅº ?ΩÏ?Í≤??ÑÎãà?ºÎ©¥ ?Ä?•Ïïà??
						m_pObject_Desc.push_back(Desc);
				}
			}

			ifs.close();

			for (auto& pDesc : m_pObject_Desc)
			{
				if (pDesc.eType == GAMEOBJ_TYPE::OBJECT)			//?§Î∏å?ùÌä∏??Í≤ΩÏö∞ ?§Î∏å?ùÌä∏ ?ùÏÑ±
				{
					MAP_OBJECT_DESC  tSrc{};
					tSrc.iTextureIndex = pDesc.iTextureIndex;
					tSrc.vPos = pDesc.vPos;
					tSrc.vScale = pDesc.vScale;
					tSrc.vRotate = pDesc.vRotate;

					m_pGameInstance->Add_GameObject_ToLayer(
						ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"),
						ENUM_CLASS(LEVEL::LEVEL_MAPEDIT),
						TEXT("Prototype_GameObject_Tree"),
						&tSrc);

					CGameObject* pGameObject = m_pGameInstance->Get_LastGameObject(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"));
					m_pObject.push_back(pGameObject);
				}
				else												//ÏßÄ?ïÏùº Í≤ΩÏö∞ ÏßÄ???ùÏÑ±
				{
					MAP_OBJECT_DESC tSrc{};
					tSrc.iTextureIndex = pDesc.iTextureIndex;
					tSrc.vPos = pDesc.vPos;

					m_pGameInstance->Add_GameObject_ToLayer(
						ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"),
						ENUM_CLASS(LEVEL::LEVEL_MAPEDIT),
						TEXT("Prototype_GameObject_TerrainBox"),
						&tSrc);

					CGameObject* pGameObject = m_pGameInstance->Get_LastGameObject(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"));
					m_pObject.push_back(pGameObject);
				}
			}
		}
		ImGuiFileDialog::Instance()->Close();
	}

	// ¿˙¿ÂøÎ ∑Œ¡˜
	if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string savePath = ImGuiFileDialog::Instance()->GetFilePathName();
			
			ofstream ofs(savePath, ios::binary);

			if (ofs.is_open())
			{
				for (auto& pObj : m_pObject)
				{
					MAP_OBJECT_DESC Desc = {};

					Desc.eType = pObj->Get_ObjType();	//TYPE ?Ä??
					if (Desc.eType == GAMEOBJ_TYPE::OBJECT)	//?§Î∏å?ùÌä∏ ?Ä?•Ïö©
					{
						CTransform* pTransform = static_cast<CTransform*>(pObj->Find_Component(TEXT("Com_Transform")));
						Desc.vPos = pTransform->Get_State(STATE::POSITION); // POSITION ?Ä??
						Desc.vScale = pTransform->Get_Scaled(); //SCALE ?Ä??

						CTexture* pTexture = static_cast<CTexture*>(pObj->Find_Component(TEXT("Com_Texture")));
						Desc.iTextureIndex = pTexture->Get_NumBindTexture(); //TEXTURE ?Ä??
					}
					else //ÏßÄ???Ä?•Ïö©
					{
						CTransform* pTransform = static_cast<CTransform*>(pObj->Find_Component(TEXT("Com_Transform_TerrainBox")));
						Desc.vPos = pTransform->Get_State(STATE::POSITION); // POSITION ?Ä??
						Desc.vScale = pTransform->Get_Scaled(); //SCALE ?Ä??

						CTexture* pTexture = static_cast<CTexture*>(pObj->Find_Component(TEXT("Com_Texture_Terrain_Top")));
						Desc.iTextureIndex = pTexture->Get_NumBindTexture(); //TEXTURE ?Ä??
					}

					m_pObject_Desc.push_back(Desc);
				}

				for (auto& Desc : m_pObject_Desc)
				{
					ofs.write(reinterpret_cast<char*>(&Desc), sizeof(MAP_OBJECT_DESC));
				}


				ofs.close();
			}

		}
		ImGuiFileDialog::Instance()->Close();
	}
}

void CLevel_MapEdit::Picking_Check()
{
	if (!m_pObject.empty() && m_pGameInstance->IsKeyDown(VK_LBUTTON))
		for (CGameObject* pObj : m_pObject)
		{
			if (pObj->Get_ObjType() == GAMEOBJ_TYPE::TERRAIN)
				continue;

			CTransform* pTransform = static_cast<CTransform*>(pObj->Find_Component(TEXT("Com_Transform")));
			CVIBuffer* pVIBuffer = static_cast<CVIBuffer*>(pObj->Find_Component(TEXT("Com_VIBuffer")));

			if (pVIBuffer->Compute_PickedObjectPosition(pTransform->Get_WorldMatrix_Inverse()))
			{
				m_pPickingObject = pObj;
				m_pObjectTransform = pTransform;
				m_bPicking = true;
				m_Scales = m_pObjectTransform->Get_Scaled();
				m_Translates = m_pObjectTransform->Get_State(STATE::POSITION);
				break;
			}
		}
}

void CLevel_MapEdit::ImGui_Object_MenBar()
{
	if (ImGui::CollapsingHeader("Object Settings", ImGuiTreeNodeFlags_DefaultOpen))   //?ÑÏ≤¥ ?πÏÖò
	{
		ImGui_Transform_Render();

		ImGui_Rotate_Render();

		ImGui_Scale_Render();

		static int iObjectTexIndex = 0;

		ImGui::Text("Object Texture Index:");
		ImGui::SetNextItemWidth(250);
		ImGui::SliderInt("Texture", &iObjectTexIndex, 0, 15); // 0~15 ¿Œµ¶Ω∫
		ImGui::SameLine();
		if (ImGui::Button("-"))
			iObjectTexIndex -= 1;

		ImGui::SameLine();
		if (ImGui::Button("+"))
			iObjectTexIndex += 1;

		ImGui_Object_Texture_Redner(iObjectTexIndex);


		if (ImGui::Button("Create Object"))   //πˆ∆∞¿‘∑¬Ω√ º±≈√«— ∞™¿∏∑Œ ª˝º∫
		{
			MAP_OBJECT_DESC  tSrc{};
			tSrc.iTextureIndex = iObjectTexIndex;
			tSrc.vPos = m_Translates;
			tSrc.vScale = m_Scales;
			tSrc.vRotate = m_Rotates;

			m_pGameInstance->Add_GameObject_ToLayer(
				ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"),
				ENUM_CLASS(LEVEL::LEVEL_MAPEDIT),
				TEXT("Prototype_GameObject_Tree"),
				&tSrc);

			CGameObject* pGameObject = m_pGameInstance->Get_LastGameObject(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"));
			m_pObject.push_back(pGameObject);
		}
	}
}

void CLevel_MapEdit::ImGui_Object_Texture_Redner(int iTextureIndex)
{
	auto iter = m_ObjectTextureInfo.find("Object");
	if (iter != m_ObjectTextureInfo.end())
	{
		OBJECT_TEXTURE_INFO& info = iter->second;

		if (info.pTextureCom)
		{
			LPDIRECT3DTEXTURE9 pTex = info.pTextureCom->Get_Textures(iTextureIndex);
			ImGui::Text("Preview:");
			ImGui::SameLine();


			ImTextureID texID = reinterpret_cast<ImTextureID>(pTex);
			ImGui::Image(texID, ImVec2(100, 100));
		}
		else
		{
			ImGui::Text("Texture Component Missing");
		}
	}

}

void CLevel_MapEdit::ImGui_Option_Button_Reset()
{
	if (!m_bPicking)
	{
		if (ImGui::Button("Option Reset"))
		{
			m_iTexture_id = 0;
			m_Scales = { 1.f,1.f, 1.f };
			m_Translates = { 0.f,0.f,0.f };
		}

		ImGui::SameLine(120.f);

		if (ImGui::Button("All Reset"))
		{
			if (!m_pObject.empty())
			{
				for (auto pObj : m_pObject)
				{
					m_pGameInstance->Remove_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"), pObj);

					Safe_Release(pObj);
				}

				m_pPickingObject = nullptr;
				m_pObjectTransform = nullptr;
				m_bPicking = false;
				m_pObject.clear();
			}
		}
	}
}

void CLevel_MapEdit::ImGui_Transform_Render()
{
	ImGui::PushItemWidth(45);
	ImGui::InputFloat("## Translate x", &m_Translates[0]);
	ImGui::SameLine();
	ImGui::InputFloat("## Translate y", &m_Translates[1]);
	ImGui::SameLine();
	ImGui::InputFloat("## Translate z", &m_Translates[2]);
	ImGui::SameLine();
	ImGui::Text("x y z Translate");
}

void CLevel_MapEdit::ImGui_Rotate_Render()
{
	ImGui::InputFloat("## Rotate x", &m_Rotates[0]);
	ImGui::SameLine();
	ImGui::InputFloat("## Rotate y", &m_Rotates[1]);
	ImGui::SameLine();
	ImGui::InputFloat("## Rotate z", &m_Rotates[2]);
	ImGui::SameLine();
	ImGui::Text("x y z Rotate");
}

void CLevel_MapEdit::ImGui_Scale_Render()
{
	ImGui::PushItemWidth(45);
	ImGui::InputFloat("## Scale x", &m_Scales[0]);
	ImGui::SameLine();
	ImGui::InputFloat("## Scale y", &m_Scales[1]);
	ImGui::SameLine();
	ImGui::InputFloat("## Scale z", &m_Scales[2]);
	ImGui::SameLine();
	ImGui::Text("x y z Scale");
}

void CLevel_MapEdit::ImGui_Picking_Object_MenBar()
{
	ImGui_Delete_Object();

	ImGui::SameLine(70.f,0.f);

	ImGui_Picking_UnCheck();

	if (ImGui::CollapsingHeader("Object Option", ImGuiTreeNodeFlags_DefaultOpen))   //?ÑÏ≤¥ ?πÏÖò
	{
		ImGui_Picking_Object_Translates_Option();

		ImGui_Picking_Object_Rotate();

		ImGui_Picking_Object_Scale();
	}

}

void CLevel_MapEdit::ImGui_Delete_Object()
{
	if (ImGui::Button("Delete"))
	{
		if (m_pPickingObject)
		{
			auto objIter = m_pObject.begin();
			while (objIter != m_pObject.end())
			{
				if (*objIter == m_pPickingObject)
				{
					Safe_Release(*objIter);

					objIter = m_pObject.erase(objIter);
					m_pGameInstance->Remove_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"), m_pPickingObject);
					break;
				}
				else
				{
					++objIter;
				}
			}

			m_pPickingObject = nullptr;
			m_pObjectTransform = nullptr;
			m_bPicking = false;
		}
	}
}

void CLevel_MapEdit::ImGui_Picking_Object_Translates_Option()
{
	if (ImGui::TreeNode("Translate Option"))
	{
		ImGui::SetNextItemWidth(200.0f);
		ImGui::SliderFloat("Translate.x", &m_Translates[0], -100.f, 100.f);
		ImGui::SameLine();
		if (ImGui::Button("-##Translate.x"))
			m_Translates[0] -= 0.1f;

		ImGui::SameLine();
		if (ImGui::Button("+##Translate.x"))
			m_Translates[0] += 0.1f;

		ImGui::SetNextItemWidth(200.f);
		ImGui::SliderFloat("Translate.y", &m_Translates[1], -100.f, 100.f);
		ImGui::SameLine();
		if (ImGui::Button("-##Translate.y"))
			m_Translates[1] -= 0.1f;

		ImGui::SameLine();
		if (ImGui::Button("+##Translate.y"))
			m_Translates[1] += 0.1f;

		ImGui::SetNextItemWidth(200.f);
		ImGui::SliderFloat("Translate.z", &m_Translates[2], -100.f, 100.f);
		ImGui::SameLine();
		if (ImGui::Button("-##Translate.z"))
			m_Translates[2] -= 0.1f;

		ImGui::SameLine();
		if (ImGui::Button("+##Translate.z"))
			m_Translates[2] += 0.1f;

		if (m_pObjectTransform)
			m_pObjectTransform->Set_State(STATE::POSITION, m_Translates);

		ImGui::TreePop();
	}

}

void CLevel_MapEdit::ImGui_Picking_Object_Rotate()
{
	if (ImGui::TreeNode("Rotates Option"))
	{

		_float fx = m_Rotates.x;
		_float fy = m_Rotates.y;
		_float fz = m_Rotates.z;

		ImGui::SetNextItemWidth(200.0f);
		ImGui::SliderFloat("Rotate.x", &m_Rotates.x, -300.f, 300.f);
		ImGui::SameLine();
		if (ImGui::Button("-##Rotate.x")) 
			m_Rotates.x -= 1.f;

		ImGui::SameLine();
		if (ImGui::Button("+##Rotate.x"))
			m_Rotates.x += 1.f;

		ImGui::SetNextItemWidth(200.f);
		ImGui::SliderFloat("Rotate.y", &m_Rotates.y, -300.f, 300.f);
		ImGui::SameLine();
		if (ImGui::Button("-##Rotate.y"))
			m_Rotates.y -= 1.f;

		ImGui::SameLine();
		if (ImGui::Button("+##Rotate.y"))
			m_Rotates.y += 1.f;

		ImGui::SetNextItemWidth(200.f);
		ImGui::SliderFloat("Rotate.z", &m_Rotates.z, -300.f, 300.f);
		ImGui::SameLine();
		if (ImGui::Button("-##Rotate.z"))
			m_Rotates.z -= 1.f;

		ImGui::SameLine();
		if (ImGui::Button("+##Rotate.z"))
			m_Rotates.z += 1.f;

		if (m_pObjectTransform)
		{
			if (fx > m_Rotates.x || fx < m_Rotates.x)
			{
				_float fNewX = fx - m_Rotates.x;
				m_pObjectTransform->Add_Rotation(_float3{ 1.f, 0.f, 0.f }, D3DXToRadian(fNewX));
			}
			if (fy > m_Rotates.y || fy < m_Rotates.y)
			{
				_float fNewY = fy - m_Rotates.y;
				m_pObjectTransform->Add_Rotation(_float3{ 0.f, 1.f, 0.f }, D3DXToRadian(fNewY));
			}
			if (fz > m_Rotates.z || fx < m_Rotates.z)
			{
				_float fNewZ = fz - m_Rotates.z;
				m_pObjectTransform->Add_Rotation(_float3{ 0.f, 0.f, 1.f }, D3DXToRadian(fNewZ));
			}
		}
		ImGui::TreePop();
	}
}

void CLevel_MapEdit::ImGui_Picking_Object_Scale()
{
	if (ImGui::TreeNode("Scale Option"))
	{
		ImGui::SetNextItemWidth(200.0f);
		ImGui::SliderFloat("Scale X", &m_Scales.x, 0.f, 100.f);
		ImGui::SameLine();
		if (ImGui::Button("-##Scale X"))
			m_Scales.x -= 0.5f;

		ImGui::SameLine();
		if (ImGui::Button("+##Scale X"))
			m_Scales.x += 0.5f;

		ImGui::SetNextItemWidth(200.f);
		ImGui::SliderFloat("Scale Y", &m_Scales.y, 0.f, 100.f);
		ImGui::SameLine();
		if (ImGui::Button("-##Scale Y"))
			m_Scales.y -= 0.5f;

		ImGui::SameLine();
		if (ImGui::Button("+##Scale Y"))
			m_Scales.y += 0.5f;

		ImGui::SetNextItemWidth(200.f);
		ImGui::SliderFloat("Scale Z", &m_Scales.z, 0.f, 100.f);
		ImGui::SameLine();
		if (ImGui::Button("-##Scale Z"))
			m_Scales.z -= 0.5f;

		ImGui::SameLine();
		if (ImGui::Button("+##Scale Z"))
			m_Scales.z += 0.5f;

		if (m_pObjectTransform)
		{
			m_pObjectTransform->Scaling(m_Scales.x, m_Scales.y, m_Scales.z);
		}

		ImGui::TreePop();
	}
}

void CLevel_MapEdit::ImGui_Picking_UnCheck()
{
	if (ImGui::Button("UnCheck"))
	{
		m_pPickingObject = nullptr;
		m_pObjectTransform = nullptr;
		m_bPicking = false;
	}
}

void CLevel_MapEdit::ImGui_Terrain_MenBar()
{
	ImGui::Begin("Terrain Editor");

	static int iTerrainTexIndex = 0;

	ImGui::Text("Terrain Texture Index:");
	ImGui::SliderInt("TopTexIdx", &iTerrainTexIndex, 0, 25); // 0~3 ¿Œµ¶Ω∫, √ﬂ»ƒ ø¨µø

	// ¿ßƒ° 
	ImGui_Terrain_Transform_Render();

	ImGui_Terrain_Texture_Render(iTerrainTexIndex);


	if (ImGui::Button("Create Terrain"))     //πˆ∆∞¿‘∑¬Ω√ º±≈√«— ∞™¿∏∑Œ ª˝º∫
	{
		MAP_OBJECT_DESC tDesc{};
		tDesc.eType = GAMEOBJ_TYPE::TERRAIN;
		tDesc.iTextureIndex = iTerrainTexIndex;
		tDesc.vPos = m_TrrainTranslate;

		m_pGameInstance->Add_GameObject_ToLayer(
			ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"),
			ENUM_CLASS(LEVEL::LEVEL_MAPEDIT),
			TEXT("Prototype_GameObject_TerrainBox"),
			&tDesc);

		CGameObject* pGameObject = m_pGameInstance->Get_LastGameObject(ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"));
		m_pObject.push_back(pGameObject);
	}

	ImGui::End();
}

void CLevel_MapEdit::ImGui_Terrain_Transform_Render()
{
	ImGui::PushItemWidth(45);
	ImGui::InputFloat("## Translate x", &m_TrrainTranslate[0]);
	ImGui::SameLine();
	ImGui::InputFloat("## Translate y", &m_TrrainTranslate[1]);
	ImGui::SameLine();
	ImGui::InputFloat("## Translate z", &m_TrrainTranslate[2]);
	ImGui::SameLine();
	ImGui::Text("x y z Translate");
}

void CLevel_MapEdit::ImGui_Terrain_Texture_Render(int iTextureIndex)
{
	auto iter = m_ObjectTextureInfo.find("TerrainBox");
	if (iter != m_ObjectTextureInfo.end())
	{
		OBJECT_TEXTURE_INFO& info = iter->second;

		if (info.pTextureCom)
		{
			LPDIRECT3DTEXTURE9 pTex = info.pTextureCom->Get_Textures(iTextureIndex);
			ImGui::Text("Preview:");
			ImGui::SameLine();

			ImTextureID texID = reinterpret_cast<ImTextureID>(pTex);
			ImGui::Image(texID, ImVec2(86, 86));
		}
		else
		{
			ImGui::Text("Texture Component Missing");
		}
	}
}

CLevel_MapEdit* CLevel_MapEdit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{

	MessageBox(0, TEXT("∏  ø°µ≈Õ ª˝º∫ Ω√¿€"), TEXT("Debug"), MB_OK);

	CLevel_MapEdit* pInstance = new CLevel_MapEdit(pGraphic_Device);
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_MapEdit"));
		Safe_Release(pInstance);
	}

	MessageBox(0, TEXT("∏  ø°µ≈Õ ª˝º∫ º∫∞¯"), TEXT("Debug"), MB_OK);

	return pInstance;
}

void CLevel_MapEdit::Free()
{
	if (m_pImgui_Manage)
	{
		delete m_pImgui_Manage;
		m_pImgui_Manage = nullptr;
	}

	__super::Free();

	for (auto& pair : m_ObjectTextureInfo)
	{
		if (pair.second.pTextureCom)
		{
			Safe_Release(pair.second.pTextureCom);
		}
	}
	m_ObjectTextureInfo.clear();
}
