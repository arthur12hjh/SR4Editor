#pragma once

#include "Client_Defines.h"
#include "ParticleSystem.h"

NS_BEGIN(Engine)
class CTransform;
class CTexture;
NS_END


NS_BEGIN(Client)

class CEffectHitParticle final : public CParticleSystem
{
private:
	CEffectHitParticle(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffectHitParticle(const CEffectHitParticle& Prototype);
	virtual ~CEffectHitParticle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	virtual void ResetParticle(class CParticleAttribute* attribute) override;

private:
	CTransform* m_pTransformCom = { nullptr };
	CTexture* m_pTextureCom = { nullptr };


private:
	HRESULT Ready_Components();

public:
	static CEffectHitParticle* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END