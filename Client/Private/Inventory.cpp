#include "Inventory.h"

#include "GameInstance.h"

CInventory::CInventory(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CUIObject{pGraphic_Device}
{
}

CInventory::CInventory(const CInventory& Prototype)
    :CUIObject{Prototype}
{
}

HRESULT CInventory::Initialize_Prototype()
{

    return S_OK;
}

HRESULT CInventory::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    UIOBJECT_DESC	Desc{};

    Desc.fSizeX = 400.0f;
    Desc.fSizeY = 650.0f;
    Desc.fX = 700.0f;
    Desc.fY = 50.0f;

    m_rcUI = { long(Desc.fX - Desc.fSizeX * 0.5f), long(Desc.fY - Desc.fSizeY * 0.5f), long(Desc.fX + Desc.fSizeX * 0.5f), long(Desc.fY + Desc.fSizeY * 0.5f) };

    if (FAILED(__super::Initialize(&Desc)))
        return E_FAIL;

    m_pFontCom->Font_Setting(50, L"인벤토리", 200, 0, 1280, 720);

    return S_OK;
}

void CInventory::Priority_Update(_float TimeDelta)
{
}

void CInventory::Update(_float TimeDelta)
{
    __super::Update_Transform(m_pTransformCom);

    m_pFontCom->Font_Move(m_fX, m_fY);

    if (m_pGameInstance->Key_Down('P'))
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
    		m_iZBuffer = 0;
    		m_pGameInstance->Remove_List(this);
    	}
    }


}

void CInventory::Late_Update(_float TimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::BUFFERUI, this);
}

HRESULT CInventory::Render()
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

        m_pFontCom->Render();

        __super::End();

        m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

    }
   
    return S_OK;
}

HRESULT CInventory::Ready_Components()
{

    /* Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;

    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Top"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    /* Com_Texture */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Textuer_Inven"),
        TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
        return E_FAIL;

    /* Com_Font */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Font"),
        TEXT("Com_Font"), reinterpret_cast<CComponent**>(&m_pFontCom))))
        return E_FAIL;

    return S_OK;
}

CInventory* CInventory::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CInventory* pInstance = new CInventory(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : pGraphic_Device");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CInventory::Clone(void* pArg)
{
    CInventory* pInstance = new CInventory(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Created : CInventory");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CInventory::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pFontCom);
}
