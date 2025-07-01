#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Right;
NS_END

NS_BEGIN(Client)

class CHeartSymbol final : public CUIObject
{
private:
	CHeartSymbol(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHeartSymbol(const CHeartSymbol& Prototype);
	virtual ~CHeartSymbol() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void	Priority_Update(_float TimeDelta) override;
	virtual void	Update(_float TimeDleta) override;
	virtual void	Late_Update(_float TimeDleta) override;
	virtual HRESULT Render()				override;

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Right* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CHeartSymbol* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END