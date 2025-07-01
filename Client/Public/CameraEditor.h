#pragma once

#include "Camera.h"

NS_BEGIN(Client)

class CCameraEditor final : public CCamera
{
private:
	CCameraEditor(LPDIRECT3DDEVICE9 pGraphic_Device);
	CCameraEditor(const CCameraEditor& Prototype);
	virtual ~CCameraEditor() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Components(void* pArg) override;

private:
	_float2				m_vOldMouse = {};
	_float				m_fSensor = { 0.2f };

public:
	static CCameraEditor* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END