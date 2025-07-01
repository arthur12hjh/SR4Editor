#include "Silkworm.h"

CSilkworm::CSilkworm(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CSilkworm::CSilkworm(const CSilkworm& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CSilkworm::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSilkworm::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CSilkworm::Priority_Update(_float fTimeDelta)
{
}

void CSilkworm::Update(_float fTimeDelta)
{
}

void CSilkworm::Late_Update(_float fTimeDelta)
{
}

HRESULT CSilkworm::Render()
{
	return S_OK;
}

void CSilkworm::Pattern1()
{
}

void CSilkworm::Pattern2()
{
}

HRESULT CSilkworm::Ready_Components()
{
	return S_OK;
}

HRESULT CSilkworm::Begin_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	return S_OK;
}

HRESULT CSilkworm::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CSilkworm* CSilkworm::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSilkworm* pInstance = new CSilkworm(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSilkworm::Clone(void* pArg)
{
	CSilkworm* pInstance = new CSilkworm(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSilkworm");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSilkworm::Free()
{
	__super::Free();
}
