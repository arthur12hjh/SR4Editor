#pragma once

#include "Client_Defines.h"
#include "AlphaObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class CEffectHitRing final : public CAlphaObject
{
private:
	CEffectHitRing(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEffectHitRing(const CEffectHitRing& Prototype);
	virtual ~CEffectHitRing() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Rect* m_pVIBufferCom = { nullptr };

	_float3 m_fSize = {};

private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CEffectHitRing* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END