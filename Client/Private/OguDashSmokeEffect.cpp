#include "OguDashSmokeEffect.h"

#include "GameInstance.h"

COguDashSmokeEffect::COguDashSmokeEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAlphaObject{ pGraphic_Device }
{
}

COguDashSmokeEffect::COguDashSmokeEffect(const COguDashSmokeEffect& Prototype)
	: CAlphaObject { Prototype }
{
}

HRESULT COguDashSmokeEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT COguDashSmokeEffect::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	CTransform* pTargetTransform = static_cast<CTransform*>(pArg);

	m_pTransformCom->Set_WorldMatrix(*pTargetTransform->Get_WorldMatrixPtr());

	return S_OK;
}

void COguDashSmokeEffect::Priority_Update(_float fTimeDelta)
{
}

void COguDashSmokeEffect::Update(_float fTimeDelta)
{
	// 카메라와 나 사이의 거리를 계산해 저장해주자.
	Compute_CamDistance(m_pTransformCom->Get_State(STATE::POSITION));
}

void COguDashSmokeEffect::Late_Update(_float fTimeDelta)
{
	m_fFrameTime += fTimeDelta;

	if (m_fFrameTime >= 4.f / 60.f)
		++m_iFrameAcc;

	if (m_iFrameAcc >= 7)
	{
		m_iFrameAcc = 6;
		m_isDead = true;
	}

	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT COguDashSmokeEffect::Render()
{
	m_pTransformCom->Set_Transform();

	m_pTextureCom->Set_Texture(m_iFrameAcc);

	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT COguDashSmokeEffect::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Ogu_Dash_Smoke_Effect"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT COguDashSmokeEffect::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT COguDashSmokeEffect::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

COguDashSmokeEffect* COguDashSmokeEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	COguDashSmokeEffect* pInstance = new COguDashSmokeEffect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* COguDashSmokeEffect::Clone(void* pArg)
{
	COguDashSmokeEffect* pInstance = new COguDashSmokeEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void COguDashSmokeEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
