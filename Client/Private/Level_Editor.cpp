#include "Level_Editor.h"
#include "GameInstance.h"
#include "CameraEditor.h"


CLevel_Editor::CLevel_Editor(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
    : CLevel {pGraphic_Device, ENUM_CLASS(eLevelID)}
{
}

HRESULT CLevel_Editor::Initialize()
{
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
    return E_NOTIMPL;
}

HRESULT CLevel_Editor::Ready_Layer_Data(const _wstring& strLayerTag)
{
    return E_NOTIMPL;
}

HRESULT CLevel_Editor::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
        ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Sky"),
        ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
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
    CameraDesc.vAt = _float3(0.f, 0.f, 0.f);
    CameraDesc.fSpeedPerSec = 5.f;
    CameraDesc.fRotationPerSec = D3DXToRadian(90.0f);

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_GameObject_Camera"),
        ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &CameraDesc)))
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
}
