#include "Effect.h"

#include "GameInstance.h"

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAlphaObject{ pGraphic_Device }
{
}

CEffect::CEffect(const CEffect& Prototype)
	: CAlphaObject { Prototype }
{
}

HRESULT CEffect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffect::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(STATE::POSITION, _float3(
		m_pGameInstance->Random(0.f, 20.f),
		5.f,
		m_pGameInstance->Random(0.f, 20.f)
	));

	return S_OK;
}

void CEffect::Priority_Update(_float fTimeDelta)
{
}

void CEffect::Update(_float fTimeDelta)
{
	// 카메라와 나 사이의 거리를 계산해 저장해주자.
	Compute_CamDistance(m_pTransformCom->Get_State(STATE::POSITION));
}

void CEffect::Late_Update(_float fTimeDelta)
{
	// 애니메이션이 90장이므로, 프레임보다 많을 수 있다.
	m_fFrame += 90.f * fTimeDelta;

	if (m_fFrame >= 90.0f)
		m_fFrame = 0.f;

	m_pGameInstance->Add_RenderGroup(RENDER::BLEND, this);
}

HRESULT CEffect::Render()
{
	m_pTransformCom->Set_Transform();

	m_pTextureCom->Set_Texture(static_cast<_uint>(m_fFrame));

	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();
	
	if (FAILED(End_RenderState()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Effect"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CEffect::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffect* pInstance = new CEffect(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect::Clone(void* pArg)
{
	CEffect* pInstance = new CEffect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
