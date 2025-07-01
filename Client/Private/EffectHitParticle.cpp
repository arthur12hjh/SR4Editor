#include "EffectHitParticle.h"

#include "GameInstance.h"

CEffectHitParticle::CEffectHitParticle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CParticleSystem{ pGraphic_Device }
{
}

CEffectHitParticle::CEffectHitParticle(const CEffectHitParticle& Prototype)
	: CParticleSystem{ Prototype }
{
}

HRESULT CEffectHitParticle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEffectHitParticle::Initialize(void* pArg)
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
		5.f,
		12.f
	));

	return S_OK;
}

void CEffectHitParticle::Priority_Update(_float fTimeDelta)
{
}

void CEffectHitParticle::Update(_float fTimeDelta)
{
	list<CParticleAttribute>::iterator iter;

	for (iter = m_particles.begin(); iter != m_particles.end(); ++iter)
	{
		iter->_position += iter->_velocity * fTimeDelta;

		iter->_age += fTimeDelta;

		if (iter->_age > iter->_lifeTime)
		{
			iter->_isAlive = false;
			ResetParticle(&(*iter));
			m_isDead = true;
		}

	}

}

void CEffectHitParticle::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::PARTICLE, this);
}

HRESULT CEffectHitParticle::Render()
{
	//m_pGraphic_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

	m_pTransformCom->Set_Transform();

	m_pTextureCom->Set_Texture(0);

	__super::Render();

	return S_OK;
}

void CEffectHitParticle::ResetParticle(CParticleAttribute* attribute)
{
	attribute->_isAlive = true;

	m_pGameInstance->GetRandomVector(
		&attribute->_position,
		&m_vec3BoundaryMin,
		&m_vec3BoundaryMax
	);

	attribute->_position.y = m_vec3BoundaryMax.y;

	attribute->_velocity.x = m_pGameInstance->GetRandomFloat(-1.f, 1.f) * 7.f;
	attribute->_velocity.y = m_pGameInstance->GetRandomFloat(-1.f, 1.f) * 7.f;
	attribute->_velocity.z = 0.f;

	attribute->_color = D3DXCOLOR(0.73f, 0.93f, 0.93f, 1.f);

	attribute->_age = 0.f;
	attribute->_lifeTime = m_pGameInstance->GetRandomFloat(0.2f, 0.4f);

}

HRESULT CEffectHitParticle::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Texture_Default_Particle"),
		TEXT("Com_Texture"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
		return E_FAIL;

	return S_OK;
}

CEffectHitParticle* CEffectHitParticle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEffectHitParticle* pInstance = new CEffectHitParticle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEffectHitParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffectHitParticle::Clone(void* pArg)
{
	CEffectHitParticle* pInstance = new CEffectHitParticle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEffectHitParticle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffectHitParticle::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
}
