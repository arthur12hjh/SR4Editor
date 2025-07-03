#include "ImGui_Manager.h"
#include "GameInstance.h"


CImGui_Manager::CImGui_Manager(LPDIRECT3DDEVICE9 pGraphic_Device)
	: m_pGameInstance{ CGameInstance::GetInstance() }
	, m_pGraphic_Device{ pGraphic_Device }
{
	Safe_AddRef(m_pGameInstance);
	Safe_AddRef(m_pGraphic_Device);
}

HRESULT CImGui_Manager::Initialize(HWND hWnd)
{
	m_hWnd = hWnd;

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui_ImplWin32_Init(m_hWnd);
	ImGui_ImplDX9_Init(m_pGraphic_Device);

	return S_OK;
}

void CImGui_Manager::Priority_Update(_float fTimeDelta)
{
}

void CImGui_Manager::Update(_float fTimeDelta)
{
}

void CImGui_Manager::Late_Update(_float fTimeDelta)
{
}

HRESULT CImGui_Manager::Render()
{
	return E_NOTIMPL;
}

CImGui_Manager* CImGui_Manager::Create(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd)
{
	CImGui_Manager* pInstance = new CImGui_Manager(pGraphic_Device);

	if (FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX("Failed to Created : CImGui_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pGameInstance);

	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}
