#include "HpFrame.h"

#include "GameInstance.h"

CHpFrame::CHpFrame(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CHpFrame::CHpFrame(const CHpFrame& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CHpFrame::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHpFrame::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	UIOBJECT_DESC	Desc{};

	Desc.fSizeX = 310.0f;
	Desc.fSizeY = 60.0f;
	Desc.fX = 235.0f;
	Desc.fY = 60.0f;

	m_rcUI = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	return S_OK;
}

void CHpFrame::Priority_Update(_float TimeDelta)
{
}

void CHpFrame::Update(_float TimeDelta)
{
	__super::Update_Transform(m_pTransformCom);

	//if (m_pGameInstance->Key_Down('O'))
	//{
	//	m_bOn = true;
	//	if (!m_bActive)
	//	{
	//		m_bActive = true;
	//		m_pGameInstance->Set_UICount();
	//		m_pGameInstance->Set_ZBuffer();
	//		m_iZBuffer = m_pGameInstance->Get_ZBuffer();
	//		m_pGameInstance->Add_List(this);
	//	}
	//	else
	//	{
	//		m_bActive = false;
	//		//m_pGameInstance->Set_UICount(-1);
	//		m_iZBuffer = 0;
	//		m_pGameInstance->Remove_List(this);
	//	}
	//}
}

void CHpFrame::Late_Update(_float TimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CHpFrame::Render()
{
	//if (m_bActive)
	//{
		m_pTransformCom->Set_Transform();

		m_pTextureCom->Set_Texture(0);

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		__super::Begin();

		m_pVIBufferCom->Render();

		__super::End();

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	//}
	return S_OK;
}

HRESULT CHpFrame::Ready_Components()
{
	/*Transform*/
	CTransform::TRANSFORM_DESC TransformDesc{ 5.f, D3DXToRadian(90.f) };

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Textuer_HpFrame"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;



	return S_OK;
}

CHpFrame* CHpFrame::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHpFrame* pInstance = new CHpFrame(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHpFrame::Clone(void* pArg)
{
	CHpFrame* pInstance = new CHpFrame(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CHpFrame");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHpFrame::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);

}