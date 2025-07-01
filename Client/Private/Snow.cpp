#include "Snow.h"

#include "GameInstance.h"

CSnow::CSnow(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParticleSystem{ pGraphic_Device }
{
}

CSnow::CSnow(const CSnow& Prototype)
	: CParticleSystem { Prototype }
{
}

HRESULT CSnow::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CSnow::Initialize(void* pArg)
{
	srand((unsigned)time(NULL));

	if (FAILED(Ready_Components()))
		return E_FAIL;

	PARTICLESYSTEM_DESC* pDesc = static_cast<PARTICLESYSTEM_DESC*>(pArg);

	m_fSize = 0.15f;
	m_vertexBufferSize = 2048;
	m_vertexBufferOffset = 0;
	m_vertexBufferBatchSize = 512;
	
	int iNumParticles = pDesc->iNumParticles;
	
	m_vec3BoundaryMin = pDesc->vec3BoundaryMin;
	m_vec3BoundaryMax = pDesc->vec3BoundaryMax;


	for (int i = 0; i < iNumParticles; ++i)
	{
		AddParticle();
	}

	__super::Initialize(pArg);

	m_pTransformCom->Set_State(STATE::POSITION, _float3(
		10.f,
		0.f,
		10.f
	));

	return S_OK;
}

void CSnow::Priority_Update(_float fTimeDelta)
{
}

void CSnow::Update(_float fTimeDelta)
{
	list<CParticleAttribute>::iterator iter;

	for (iter = m_particles.begin(); iter != m_particles.end(); ++iter)
	{
		iter->_position += iter->_velocity * fTimeDelta;

		iter->_age += fTimeDelta;

		if (iter->_position.y < m_vec3BoundaryMin.y)
		{
			iter->_isAlive = false;
			ResetParticle(&(*iter));
		}

	}

}

void CSnow::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::PARTICLE, this);
}

HRESULT CSnow::Render()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTransformCom->Set_Transform();

	m_pTextureCom->Set_Texture(0);

	__super::Render();

	return S_OK;
}

void CSnow::ResetParticle(CParticleAttribute* attribute)
{	
	attribute->_isAlive = true;

	m_pGameInstance->GetRandomVector(
		&attribute->_position,
		&m_vec3BoundaryMin,
		&m_vec3BoundaryMax
	);

	attribute->_position.y = m_vec3BoundaryMax.y;

	attribute->_velocity.x = m_pGameInstance->GetRandomFloat(0.f, 1.f) * -3.f;
	attribute->_velocity.y = m_pGameInstance->GetRandomFloat(0.f, 1.f) * -2.f;
	attribute->_velocity.z = 0.f;

	attribute->_color = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

	attribute->_age = 0.f;
	attribute->_lifeTime = 2.f;

}

HRESULT CSnow::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Snow"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CSnow* CSnow::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CSnow* pInstance = new CSnow(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CSnow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CSnow::Clone(void* pArg)
{
	CSnow* pInstance = new CSnow(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CSnow");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSnow::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}
