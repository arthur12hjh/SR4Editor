#include "Level_Editor.h"
#include "GameInstance.h"
#include "ImGui_Manager.h"
#include "CameraEditor.h"


CLevel_Editor::CLevel_Editor(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
    : CLevel {pGraphic_Device, ENUM_CLASS(eLevelID)}
    , m_pImGui_Manager{ CImGui_Manager::GetInstance() }
{
    Safe_AddRef(m_pImGui_Manager);
}

HRESULT CLevel_Editor::Initialize()
{
    if (FAILED(Ready_Lights()))
        return E_FAIL;

    if (FAILED(Ready_Layer_Data(TEXT("Layer_Data"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
        return E_FAIL;

    if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
        return E_FAIL;

    return S_OK;
}

void CLevel_Editor::Update(_float fTimeDelta)
{
}

HRESULT CLevel_Editor::Render()
{

    return S_OK;
}

HRESULT CLevel_Editor::Ready_Lights()
{
    /* 빛의 정보 */
    D3DLIGHT9			LightDesc{};
    LightDesc.Type = D3DLIGHT_DIRECTIONAL;

    LightDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    LightDesc.Ambient = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.f);
    LightDesc.Direction = _float3(1.f, -1.f, 1.f);

    m_pGraphic_Device->SetLight(0, &LightDesc);

    /* 받은 빛을 어떻게 반사할건지?(재질) */
    D3DMATERIAL9		MaterialDesc{};
    MaterialDesc.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    MaterialDesc.Ambient = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);


    m_pGraphic_Device->SetMaterial(&MaterialDesc);

    m_pGraphic_Device->LightEnable(0, true);

    m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

    return S_OK;
}

HRESULT CLevel_Editor::Ready_Layer_Data(const _wstring& strLayerTag)
{
    strTerrainVertexDataFileName = new _wstring(TEXT("../Bin/Resources/Data/TerrainVertexDataTest01.csv"));
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::EDITOR), TEXT("Prototype_GameObject_TerrainVertexData"),
        ENUM_CLASS(LEVEL::EDITOR), strLayerTag, static_cast<void*>(strTerrainVertexDataFileName))))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Editor::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::EDITOR), TEXT("Prototype_GameObject_Terrain"),
        ENUM_CLASS(LEVEL::EDITOR), strLayerTag)))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::EDITOR), TEXT("Prototype_GameObject_Sky"),
        ENUM_CLASS(LEVEL::EDITOR), strLayerTag)))
        return E_FAIL;

    return S_OK;
}

HRESULT CLevel_Editor::Ready_Layer_Camera(const _wstring& strLayerTag)
{
    CCamera::CAMERA_DESC			CameraDesc{};
    CameraDesc.fFov = D3DXToRadian(60.0f);
    CameraDesc.fNear = 0.1f;
    CameraDesc.fFar = 300.f;
    CameraDesc.vEye = _float3(0.f, 1.f, -1.f);
    CameraDesc.vAt = _float3(0.f, -1.f, 1.f);
    CameraDesc.fSpeedPerSec = 5.f;
    CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_CameraEditor"),
        ENUM_CLASS(LEVEL::EDITOR), strLayerTag, &CameraDesc)))
        return E_FAIL;

    return S_OK;
}

CLevel_Editor* CLevel_Editor::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
{
    CLevel_Editor* pInstance = new CLevel_Editor(pGraphic_Device, eLevelID);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : CLevel_Editor");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLevel_Editor::Free()
{
    __super::Free();

    delete strTerrainVertexDataFileName;

    Safe_Release(m_pImGui_Manager);
}
