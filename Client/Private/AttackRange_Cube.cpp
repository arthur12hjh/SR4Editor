#include "AttackRange_Cube.h"
#include "GameInstance.h"

CAttackRange_Cube::CAttackRange_Cube(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CGameObject { pGraphic_Device }
{
}

CAttackRange_Cube::CAttackRange_Cube(const CAttackRange_Cube& Prototype)
    : CGameObject { Prototype }
{
}

HRESULT CAttackRange_Cube::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAttackRange_Cube::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_bVisible = true;

    return S_OK;
}

void CAttackRange_Cube::Priority_Update(_float fTimeDelta)
{
}

void CAttackRange_Cube::Update(_float fTimeDelta)
{
   
}

void CAttackRange_Cube::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CAttackRange_Cube::Render()
{
    if (!m_bVisible)
        return S_OK;

    m_pTransformCom->Set_Transform();

    if (FAILED(Begin_RenderState()))
        return E_FAIL;

    m_pVIBufferCom->Render();

    if (FAILED(End_RenderState()))
        return E_FAIL;

    return S_OK;
}

void CAttackRange_Cube::OnCollisionEnter(void* pArg, _float fTimeDelta)
{
 
}


void CAttackRange_Cube::Set_RangeSize(_float3& vCubePos, _float3& vCubeScale, _float fTimeDelta)
{
    m_pTransformCom->Set_State(STATE::POSITION, vCubePos);

    m_pTransformCom->Set_Scale(vCubeScale);

    m_bVisible = true;   

    m_fTime += fTimeDelta;

    if (1.f < m_fTime)
    {
        m_bVisible = false;
        m_fTime = 0.f;
    }

}

HRESULT CAttackRange_Cube::Ready_Components()
{
    /* Com_Transform */
    CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
        TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
        return E_FAIL;


    /* Com_VIBuffer */
    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_VIBuffer_Cube"),
        TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
        return E_FAIL;

    return S_OK;
}

HRESULT CAttackRange_Cube::Begin_RenderState()
{
    //m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
   
    //m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);



    return S_OK;
}

HRESULT CAttackRange_Cube::End_RenderState()
{
    //m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

    return S_OK;
}

CAttackRange_Cube* CAttackRange_Cube::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CAttackRange_Cube* pInstance = new CAttackRange_Cube(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : pGraphic_Device");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CAttackRange_Cube::Clone(void* pArg)
{
    CAttackRange_Cube* pInstance = new CAttackRange_Cube(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CAttackRange_Cube");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttackRange_Cube::Free()
{
    __super::Free();

    Safe_Release(m_pTransformCom);
    Safe_Release(m_pVIBufferCom);
}
