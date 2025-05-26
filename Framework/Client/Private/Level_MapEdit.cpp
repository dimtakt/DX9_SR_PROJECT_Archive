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
#include "Mountain.h"



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

	/*if (FAILED(Ready_Texture_Info()))
		return E_FAIL;*/

	return S_OK;
}

void CLevel_MapEdit::Update(_float fTimeDelta)
{
	
}

HRESULT CLevel_MapEdit::Render()
{
	SetWindowText(g_hWnd, TEXT("맵 에디터 레벨 입니다"));
	//m_pImgui_Manage->Render();
	this->Imgui_Render();

	return S_OK;
}

HRESULT CLevel_MapEdit::Ready_ImGui(HWND hWnd, LPDIRECT3DDEVICE9 pOut)
{
	MessageBox(0, TEXT("Ready_ImGui 시작"), TEXT("Debug"), MB_OK);


	m_pImgui_Manage = CImgui_Manager::Create(hWnd, pOut);
	if (m_pImgui_Manage == nullptr)
	{
		MSG_BOX(TEXT("Failed to Created : CImgui_Manager"));
		return E_FAIL;
	}
	MessageBox(0, TEXT("Imgui 매니저 생성 성공"), TEXT("Debug"), MB_OK);
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
	// 수동 제작 진행, 제작 및 기능 구현 다되고 시간되면 내부구조 개선 진행 예정
	//나무 
	m_pPreview = static_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT,
		ENUM_CLASS(LEVEL::LEVEL_MAPEDIT),
		TEXT("Prototype_GameObject_Tree")));

	OBJECT_TEXTURE_INFO treeInfo;
	treeInfo.strObjectType = "Tree";
	treeInfo.iTextureCount = 16;
	treeInfo.pTextureCom = static_cast<CTexture*>(m_pPreview->Find_Component(TEXT("Com_Texture")));
	if (treeInfo.pTextureCom)
		treeInfo.pTextureCom->AddRef();   //텍스처 주소 날아가면 안됨, 이미지 프리뷰를 위해 addref

	m_ObjectTextureInfo["Tree"] = treeInfo;
	
	Safe_Release(m_pPreview);

	//산
	m_pPreview = static_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(
		PROTOTYPE::GAMEOBJECT,
		ENUM_CLASS(LEVEL::LEVEL_MAPEDIT),
		TEXT("Prototype_GameObject_Mountain")));

	OBJECT_TEXTURE_INFO MountainInfo;
	MountainInfo.strObjectType = "Mountain";
	MountainInfo.iTextureCount = 1;
	MountainInfo.pTextureCom = static_cast<CTexture*>(m_pPreview->Find_Component(TEXT("Com_Texture")));
	if (MountainInfo.pTextureCom)
		MountainInfo.pTextureCom->AddRef();

	m_ObjectTextureInfo["Mountain"] = MountainInfo;

	Safe_Release(m_pPreview);

	return S_OK;
}


HRESULT CLevel_MapEdit::Delete_Tile_By_Position(D3DXVECTOR3& vTargetPos)
{
	/*CLayer* pLayer = m_pGameInstance->Find_Layer(m_iTile_id, TEXT("Prototype_GameObject_Static_Tile"));*/
	// 오브젝트 삭제 처리 구조조정중
	return S_OK;
}

void CLevel_MapEdit::Imgui_Render()
{
	m_pImgui_Manage->Render_Begin();

	ImGui::Begin("Test Window", nullptr, ImGuiWindowFlags_MenuBar);

	static int iSelectedObjectType = 0;  //오브젝트 타입 구별
	const char* objectList[] = { "Tree", "Mountain" ,"Rock"};

	ImGui::Combo("Object Type", &iSelectedObjectType, objectList, IM_ARRAYSIZE(objectList));

	if (ImGui::Button("Preview"))    //프리뷰 버튼 눌러야 프리뷰 전용 오브젝트 만듦
	{
		if (!m_bPreviewReady)
		{
			if (FAILED(Ready_Texture_Info()))
			{
				MessageBox(nullptr, L"Preview false", L"MapEdit", MB_OK);
			}
			else
			{
				m_bPreviewReady = true;
			}
		}
	}

	ImGui_MenuBar_Render();

	ImGui_Scale_Render();

	ImGui_Rotate_Render();

	ImGui_Transform_Render();  //UI 랜더 및 각변수에 값 저장


	if (m_bPreviewReady) // 프리뷰 켜져야지만 작동
	{

		string strSelectedType = objectList[iSelectedObjectType];     //오브젝트 타입에 따라 텍스처 개수 처리
		int iSelectedIndex; //텍스처 인덱스

		if (ImGui_TextureSelector_Render(strSelectedType, iSelectedIndex))  
		{
			const _tchar* szPrototypeTag = nullptr;
			
			//타입에 따라 태그값 설정
			if (strSelectedType == "Tree")
				szPrototypeTag = TEXT("Prototype_GameObject_Tree");
			else if (strSelectedType == "Rock")
				szPrototypeTag = TEXT("Prototype_GameObject_Rock");
			else if (strSelectedType == "Mountain")
				szPrototypeTag = TEXT("Prototype_GameObject_Mountain");

			if (szPrototypeTag != nullptr)
			{
				// 저장해서 오브젝트 생성
				MAP_OBJECT_DESC tDesc{};
				tDesc.iTextureIndex = iSelectedIndex;
				tDesc.vPos = m_Translates;
				tDesc.vRotate = m_Rotates;
				tDesc.vScale = m_Scales;

				m_pGameInstance->Add_GameObject_ToLayer(
					ENUM_CLASS(LEVEL::LEVEL_MAPEDIT), TEXT("Layer_MapEdit"), ENUM_CLASS(LEVEL::LEVEL_MAPEDIT),
					szPrototypeTag, &tDesc);
			}
		}
	}


	ImGui::SameLine(0.0f, 10.0f);

	if (ImGui::Button("Delete"))
	{
		/*Delete_Tile_By_Position(m_DeletePos);*/
	}

	ImGui::End();
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
				config.path = "../SaveFile/";
				config.flags = ImGuiFileDialogFlags_ReadOnlyFileNameField;

				// 파일 다이얼로그 열기
				// ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", config);
				ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".txt,.cpp,.h", config);
			}
			if (ImGui::MenuItem("Save"))
			{
				IGFD::FileDialogConfig config;
				config.path = "../SaveFile/";
				config.flags = ImGuiFileDialogFlags_ConfirmOverwrite;

				ImGuiFileDialog::Instance()->OpenDialog("SaveFileDlgKey", "Choose File", ".txt,.cpp,.h", config);

			}
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	// 읽기용 로직
	if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
		if (ImGuiFileDialog::Instance()->IsOk()) {
			std::string filePath = ImGuiFileDialog::Instance()->GetFilePathName();
			ImGui::Text("Selected file: %s", filePath.c_str());
		}
		ImGuiFileDialog::Instance()->Close();
	}

	// 저장용 로직
	if (ImGuiFileDialog::Instance()->Display("SaveFileDlgKey"))
	{
		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string savePath = ImGuiFileDialog::Instance()->GetFilePathName();
			ImGui::Text("Saving to: %s", savePath.c_str());

			//ofstream ofs(savePath); ofs << "data";

		}
		ImGuiFileDialog::Instance()->Close();
	}
}

bool CLevel_MapEdit::ImGui_TextureSelector_Render(string& strSelectedType, int& iTextureIndex)
{
	static int iSelectedTexIndex = 0; //텍스처 인덱스 저장용

	auto iter = m_ObjectTextureInfo.find(strSelectedType);
	if (iter == m_ObjectTextureInfo.end())
		return false;

	OBJECT_TEXTURE_INFO& info = iter->second; //구조체 저장

	if (info.pTextureCom == nullptr)
	{
		ImGui::Text("TextureCom is null!");
		return false;
	}

	ImGui::Text("Texture Index:");
	ImGui::SameLine();
	ImGui::SetNextItemWidth(100);
	ImGui::SliderInt("##TextureIndex", &iSelectedTexIndex, 0, info.iTextureCount - 1);

	LPDIRECT3DTEXTURE9 pTex = info.pTextureCom->Get_Textures(iSelectedTexIndex); // 
	if (pTex)
	{
		ImGui::SameLine();
		ImTextureID texID = reinterpret_cast<ImTextureID>(pTex); //IMGUI 이미지 띄우기 용도
		ImGui::Image(texID, ImVec2(64, 64));
	}

	if (ImGui::Button("Create"))
	{
		iTextureIndex = iSelectedTexIndex;  //버튼 클릭될시 외부에 값 전달 
		return true;
	}
	return false;
}

void CLevel_MapEdit::ImGui_TextureId_Render()
{
	ImGui::SameLine();
	ImGui::InputInt("Texture Id", &m_iTexture_id);
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

CLevel_MapEdit* CLevel_MapEdit::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{

	MessageBox(0, TEXT("맵 에디터 생성 시작"), TEXT("Debug"), MB_OK);

	CLevel_MapEdit* pInstance = new CLevel_MapEdit(pGraphic_Device);
	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(TEXT("Failed to Created : CLevel_MapEdit"));
		Safe_Release(pInstance);
	}

	MessageBox(0, TEXT("맵 에디터 생성 성공"), TEXT("Debug"), MB_OK);

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
