#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "Status.h"

CLevel_GamePlay::CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
	: CLevel { pGraphic_Device, ENUM_CLASS(eLevelID)}
{

}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Events()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Sounds()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Data(TEXT("Layer_Data"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_HpBar(TEXT("Layer_HpBar"))))
		return E_FAIL;	
	
	if (FAILED(Ready_Layer_Inven(TEXT("Layer_Inven"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_HpBar(TEXT("Layer_HpBar"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player_AttackRange_Cube(TEXT("Layer_Player_AttackRange_Cube"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster_AttackRange_Cube(TEXT("Layer_Monster_AttackRange_Cube"))))
		return E_FAIL;

	m_pGameInstance->AddCollisionLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player_AttackRange_Cube"));
	//m_pGameInstance->AddCollisionLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster_AttackRange_Cube"));

	return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	m_fAddTimeDelta += fTimeDelta;

	if ((GetKeyState(VK_LBUTTON) & 0x8000)&& m_fAddTimeDelta > 1.f)
	{
		const _wstring& strLayerTag = TEXT("Layer_Effect");

		CParticleSystem::PARTICLESYSTEM_DESC		hitParticleDesc{};
		hitParticleDesc.iNumParticles = 20;
		hitParticleDesc.vec3BoundaryMin = _float3(0.f, 0.f, 0.f);
		hitParticleDesc.vec3BoundaryMax = _float3(0.f, 0.f, 0.f);

		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_EffectHitParticle"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &hitParticleDesc);

		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_EffectHitRing"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag);

		m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_EffectHitCircle"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag);

		m_fAddTimeDelta = 0.f;
	}
}

HRESULT CLevel_GamePlay::Render()
{
	//SetWindowText(g_hWnd, TEXT("게임플레이레벨이빈다"));

	CGameObject* pPlayer = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"));
	if (pPlayer)
	{
		CStatus* pPlayerStatus = dynamic_cast<CStatus*>(pPlayer->Find_Component(TEXT("Com_Status")));
		if (pPlayerStatus)
		{
			_uint iPlayerMaxHP = pPlayerStatus->Get_MaxHp();
			_uint iPlayerDamage = pPlayerStatus->Get_Damage();
			_uint iPlayerDefense = pPlayerStatus->Get_Defense();

			wchar_t szTitle[256];
			swprintf_s(szTitle, L"플레이어 HP : %.1d, 데미지: %.1d, 방어력 : %.1d", iPlayerMaxHP, iPlayerDamage, iPlayerDefense);
			SetWindowText(g_hWnd, szTitle);
		}
	}
	else
	{
		SetWindowText(g_hWnd, L"게임플레이레벨이빈다");
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Events()
{
	if (FAILED(m_pGameInstance->AddEvent(TEXT("MonsterHitEffect"), nullptr)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
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

HRESULT CLevel_GamePlay::Ready_Sounds()
{
	m_pGameInstance->Manager_PlayBGM(TEXT("BGMTest.mp3"), 1.f);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Data(const _wstring& strLayerTag)
{
	_wstring strFileName = TEXT("../Bin/Resources/Data/test.csv");
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_TestLevelData"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, static_cast<void*>(&strFileName))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Terrain"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Sky"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;

}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _wstring& strLayerTag)
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

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _wstring& strLayerTag)
{
	for (size_t i = 0; i < 1; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Player"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _wstring& strLayerTag)
{
	for (size_t i = 0; i < 10; i++)
	{
		if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Monster"),
			ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
			return E_FAIL;

	}

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_HpBar(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HpFrame"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HpBar"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_HeartSymbol"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Inven(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Inven"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Slot"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _wstring& strLayerTag)
{
	// for (size_t i = 0; i < 50; i++)
	// {
	// 	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Effect"),
	// 		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
	// 		return E_FAIL;
	// }

	CParticleSystem::PARTICLESYSTEM_DESC		ParticleDesc{};
	ParticleDesc.iNumParticles = 1000;
	ParticleDesc.vec3BoundaryMin = _float3(-10.f, -10.f, -10.f);
	ParticleDesc.vec3BoundaryMax = _float3(10.f, 10.f, 10.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Snow"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &ParticleDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::SetEffectEvent()
{
	const _wstring& strLayerTag = TEXT("Layer_Effect");

	CParticleSystem::PARTICLESYSTEM_DESC		hitParticleDesc{};
	hitParticleDesc.iNumParticles = 50;
	hitParticleDesc.vec3BoundaryMin = _float3(0.f, 0.f, 0.f);
	hitParticleDesc.vec3BoundaryMax = _float3(0.f, 0.f, 0.f);

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_EffectHitParticle"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag, &hitParticleDesc)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_EffectHitRing"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_EffectHitCircle"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player_AttackRange_Cube(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AttackRange_Cube"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster_AttackRange_Cube(const _wstring& strLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AttackRange_Cube"),
		ENUM_CLASS(LEVEL::GAMEPLAY), strLayerTag)))
		return E_FAIL;

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pGraphic_Device, eLevelID);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();


}
