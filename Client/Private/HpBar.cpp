#include "HpBar.h"

#include "GameInstance.h"

CHpBar::CHpBar(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CHpBar::CHpBar(const CHpBar& Prototype)
	:CUIObject{ Prototype }
{
}

HRESULT CHpBar::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHpBar::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	UIOBJECT_DESC	Desc{};

	Desc.fSizeX = 310.0f;
	Desc.fSizeY = 40.0f;
	Desc.fX = 80.0f;
	Desc.fY = 60.0f;

	m_rcUI = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	m_pFontCom->Font_Setting(40, L"Ã¼·Â¹Ù", 300, 60,310,40);

	return S_OK;
}

void CHpBar::Priority_Update(_float fTimeDelta)
{
}

void CHpBar::Update(_float fTimeDelta)
{
	__super::Update_Transform(m_pTransformCom);

	if (m_pGameInstance->Key_Down('U'))
	{
		m_fSizeX -= 310 / 3;
	}
}

void CHpBar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::UI, this);
}

HRESULT CHpBar::Render()
{
	//if (m_bActive)
	//{
		m_pTransformCom->Set_Transform();

		m_pTextureCom->Set_Texture(0);

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	
		__super::Begin();

		m_pVIBufferCom->Render();

		m_pFontCom->Render();

		__super::End();


		m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	//}

	return S_OK;
}

HRESULT CHpBar::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Right"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Textuer_HpBar"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	/* Com_Font */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Font"),
		TEXT("Com_Font"), reinterpret_cast<CComponent**>(&m_pFontCom))))
		return E_FAIL;


	return S_OK;
}

CHpBar* CHpBar::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHpBar* pInstance = new CHpBar(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHpBar::Clone(void* pArg)
{
	CHpBar* pInstance = new CHpBar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CHpBar");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHpBar::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pFontCom);
}
