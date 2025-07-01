#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CAnimation;
class CTransform;
class CVIBuffer_Rect;
class CLand;
class CStatus;
NS_END


NS_BEGIN(Client)

class CSilkworm final : public CGameObject
{
private:
	CSilkworm(LPDIRECT3DDEVICE9 pGraphic_Device);
	CSilkworm(const CSilkworm& Prototype);
	virtual ~CSilkworm() = default;

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
	CLand* m_pLandCom = { nullptr };

private:
	void Pattern1();
	void Pattern2();

private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

public:
	static CSilkworm* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

