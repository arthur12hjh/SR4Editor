#pragma once

#include "Client_Defines.h"
#include "UIObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Right;
class CFont;
NS_END

NS_BEGIN(Client)

class CHpFrame final : public CUIObject
{
private:
	CHpFrame(LPDIRECT3DDEVICE9 pGraphic_Device);
	CHpFrame(const CHpFrame& Prototype);
	virtual ~CHpFrame() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual	void	Priority_Update(_float TimeDelta) override;
	virtual void	Update(_float TimeDelta) override;
	virtual void	Late_Update(_float TimeDelta) override;
	virtual HRESULT Render()				override;

private:
	CTexture* m_pTextureCom = { nullptr };
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Right* m_pVIBufferCom = { nullptr };
	CFont* m_pFontCom = { nullptr };

private:
	HRESULT Ready_Components();

public:
	static CHpFrame* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END