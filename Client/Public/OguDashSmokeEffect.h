#pragma once

#include "Client_Defines.h"
#include "AlphaObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
NS_END

NS_BEGIN(Client)

class COguDashSmokeEffect final : public CAlphaObject
{
private:
	COguDashSmokeEffect(LPDIRECT3DDEVICE9 pGraphic_Device);
	COguDashSmokeEffect(const COguDashSmokeEffect& Prototype);
	virtual ~COguDashSmokeEffect() = default;

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

	_float m_fFrameTime = {};
	_uint  m_iFrameAcc = {};

private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static COguDashSmokeEffect* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END