#include "EffectHitCircle.h"

#include "GameInstance.h"

CEffectHitCircle::CEffectHitCircle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAlphaObject{ pGraphic_Device }
{
}

CEffectHitCircle::CEffectHitCircle(const CEffectHitCircle& Prototype)
	: CAlphaObject { Prototype }
{
}

HRESULT CEffectHitCircle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffectHitCircle::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, _float3(
		10.f,
		5.f,
		12.f
	));

	m_fSize = { 0.5f, 0.5f, 0.5f };

	m_pTransformCom->Set_Scale(m_fSize);

	return S_OK;
}

void CEffectHitCircle::Priority_Update(_float fTimeDelta)
{
}

void CEffectHitCircle::Update(_float fTimeDelta)
{
	// 카메라와 나 사이의 거리를 계산해 저장해주자.
	Compute_CamDistance(m_pTransformCom->Get_State(STATE::POSITION));
}

void CEffectHitCircle::Late_Update(_float fTimeDelta)
{
	m_fSize.x += fTimeDelta * 3;
	m_fSize.y += fTimeDelta * 3;
	m_fSize.z += fTimeDelta * 3;

	m_pTransformCom->Set_Scale(m_fSize);

	if (m_fSize.x >= 0.7f)
	{
		m_fSize = {0.f, 0.f, 0.f};
		m_isDead = true;
	}

	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CEffectHitCircle::Render()
{
	m_pTransformCom->Set_Transform();

	m_pTextureCom->Set_Texture(0);

	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectHitCircle::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Firefly_Particle"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffectHitCircle::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CEffectHitCircle::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CEffectHitCircle* CEffectHitCircle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffectHitCircle* pInstance = new CEffectHitCircle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffectHitCircle::Clone(void* pArg)
{
	CEffectHitCircle* pInstance = new CEffectHitCircle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffectHitCircle::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
