#include "HeartSymbol.h"

#include "GameInstance.h"

CHeartSymbol::CHeartSymbol(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CUIObject{ pGraphic_Device }
{
}

CHeartSymbol::CHeartSymbol(const CHeartSymbol& Prototype)
	: CUIObject{ Prototype }
{
}

HRESULT CHeartSymbol::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CHeartSymbol::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	UIOBJECT_DESC	Desc{};

	Desc.fSizeX = 60.0f;
	Desc.fSizeY = 60.0f;
	Desc.fX = 55.0f;
	Desc.fY = 55.0f;

	m_rcUI = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;


	return S_OK;
}

void CHeartSymbol::Priority_Update(_float TimeDelta)
{
}

void CHeartSymbol::Update(_float TimeDleta)
{
	__super::Update_Transform(m_pTransformCom);

	if (m_pGameInstance->Key_Down('I'))
	{
		m_bOn = true;
		if (!m_bActive)
		{
			m_bActive = true;
			m_pGameInstance->Set_ZBuffer();
			m_pGameInstance->Set_UICount();
			m_iZBuffer = m_pGameInstance->Get_ZBuffer();
			m_pGameInstance->Add_List(this);
		}
		else
		{
			m_bActive = false;
			//m_pGameInstance->Set_ZBuffer(-1);
			m_iZBuffer = 0;
			m_pGameInstance->Remove_List(this);
		}
	}
}

void CHeartSymbol::Late_Update(_float TimeDleta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::BUFFERUI, this);

}

HRESULT CHeartSymbol::Render()
{
	if (m_bActive)
	{
		m_pTransformCom->Set_Transform();

		m_pTextureCom->Set_Texture(0);

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		__super::Begin();

		m_pVIBufferCom->Render();

		__super::End();

		m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	}
	return S_OK;
}

HRESULT CHeartSymbol::Ready_Components()
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
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Textuer_HeartSymbol"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CHeartSymbol* CHeartSymbol::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CHeartSymbol* pInstance = new CHeartSymbol(pGraphic_Device);
	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}
	return pInstance;
}

CGameObject* CHeartSymbol::Clone(void* pArg)
{
	CHeartSymbol* pInstance = new CHeartSymbol(*this);
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CHeartSymbol");
		Safe_Release(pInstance);
	}
	return pInstance;
}

void CHeartSymbol::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
}
