/* ImGui_Manager.cpp */
#include "ImGui_Manager.h"
#include <fstream>

IMPLEMENT_SINGLETON(CImGui_Manager)

CImGui_Manager::CImGui_Manager()
{
}

HRESULT CImGui_Manager::Initialize(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd)
{
	m_pGraphic_Device = pGraphic_Device;
	m_hWnd = hWnd;

	// ImGui ÃÊ±âÈ­
	IMGUI_CHECKVERSION();
	
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();


	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX9_Init(m_pGraphic_Device);

	LoadFont();

	return S_OK;
}

void CImGui_Manager::Priority_Update(_float fTimeDelta)
{
	// Start the Dear ImGui frame
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CImGui_Manager::Update(_float fTimeDelta)
{
}

void CImGui_Manager::Late_Update(_float fTimeDelta)
{
}

HRESULT CImGui_Manager::Render()
{
	ShowGUI();
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());

	return S_OK;
}

void CImGui_Manager::ShowGUI()
{
	//static _float2 position = _float2(100.f, 100.f);
	//ImGui::SliderFloat2("Position", position, -800.f, 800.f);

}

void CImGui_Manager::LoadFont()
{
	ImGuiIO& io = ImGui::GetIO();
	ImFont* font = nullptr;

	string strFontTag = "../Bin/Resources/Fonts/H2GTRM.TTF";

	ifstream ifile;

	ifile.open(strFontTag);
	if (ifile)
	{
		font = io.Fonts->AddFontFromFileTTF(strFontTag.c_str(), 16.f, NULL, io.Fonts->GetGlyphRangesKorean());
	}
	IM_ASSERT(font != NULL);
}


void CImGui_Manager::Free()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	__super::Free();

	Safe_Release(m_pGraphic_Device);


}
