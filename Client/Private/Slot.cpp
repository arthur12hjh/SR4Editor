#include "Slot.h"
#include "GameInstance.h"

CSlot::CSlot(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{pGraphic_Device}
{
}

CSlot::CSlot(const CSlot& Prototype)
	: CUIObject{Prototype}
{
}

HRESULT CSlot::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSlot::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSlot::Priority_Update(_float fTimeDelta)
{
}

void CSlot::Update(_float fTimeDelta)
{
	__super::Update_Transform(m_pTransformCom);

}

void CSlot::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);

}

HRESULT CSlot::Render()
{
	m_pTransformCom->Set_Transform();

	m_pTextureCom->Set_Texture(0);

	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);

	__super::Begin();

	m_pVIBufferCom->Render();

	__super::End();


	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//}

	return S_OK;
}

HRESULT CSlot::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Textuer_HpBar"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;


	return S_OK;
}

CSlot* CSlot::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSlot* pInstance = new CSlot(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSlot::Clone(void* pArg)
{
	CSlot* pInstance = new CSlot(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CSlot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSlot::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
