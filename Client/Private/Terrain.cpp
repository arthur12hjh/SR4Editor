#include "Terrain.h"
#include "TerrainVertexData.h"
#include "ImGui_Manager.h"

#include "GameInstance.h"

CTerrain::CTerrain(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
	, m_pImGui_Manager { CImGui_Manager::GetInstance() }
{

}

CTerrain::CTerrain(const CTerrain& Prototype)
	: CGameObject { Prototype }
{

}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pVertexDataObject = static_cast<CTerrainVertexData*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::EDITOR), TEXT("Layer_Data")));

	m_pVIBufferCom->Apply_TerrainData(m_pVertexDataObject->Get_Data());

	m_pVertexDataObject->Set_Terrain(m_pVIBufferCom);

	return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
	SetMapEditor();
}

void CTerrain::Update(_float fTimeDelta)
{
	/*if (GetKeyState(VK_LBUTTON) & 0x8000)
	{
		_float3		vPickPos{};
		if (m_pVIBufferCom->Picking(m_pTransformCom, &vPickPos))
			int a = 10;
	}*/
}


void CTerrain::Late_Update(_float fTimeDelta)
{

	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTransformCom->Set_Transform();

	m_pTextureCom->Set_Texture(0);

	m_pVIBufferCom->Render();

	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	return S_OK;
}

void CTerrain::SetMapEditor()
{
	ImGui::Begin(u8"맵 에디터", NULL, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save", "Ctrl+S"))
				SaveVertexData();
			ImGui::Separator();
			if (ImGui::MenuItem("Open", "Ctrl+O"))
				LoadVertexData();
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	static _float3 position = _float3(500.f, 500.f, 500.f);
	ImGui::InputFloat3(u8"정점 위치", position);
	static int idx[2] = { 0, 0 };
	ImGui::InputInt2(u8"인덱스", idx);

	ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::ImColor(0.7f, 0.7f, 0.7f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, (ImVec4)ImColor::ImColor(0.6f, 0.6f, 0.6f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::ImColor(0.f, 0.f, 0.f));

	if (ImGui::Button(u8"정점 적용"))
	{
		TERRAINDATATYPE_DESC dataTmp;
		dataTmp.iVertexPositionX = idx[0];
		dataTmp.iVertexPositionZ = idx[1];
		dataTmp.vPosition = position;
		ApplyVertexData(dataTmp);
	}
	ImGui::PopStyleColor(3);


	ImGui::End();
}


void CTerrain::SaveVertexData()
{
	m_pVertexDataObject->Save_Data();
}

void CTerrain::LoadVertexData()
{
}

void CTerrain::ApplyVertexData(TERRAINDATATYPE_DESC dataDesc)
{
	m_pVertexDataObject->Set_Data(dataDesc);
}

HRESULT CTerrain::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::EDITOR), TEXT("Prototype_Component_Texture_Terrain"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::EDITOR), TEXT("Prototype_Component_VIBuffer_Terrain"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;	

	return S_OK;
}

CTerrain* CTerrain::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrain* pInstance = new CTerrain(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

}
