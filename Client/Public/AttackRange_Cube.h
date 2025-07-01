#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTransform;
class CVIBuffer_Cube;
class CStatus;
NS_END


NS_BEGIN(Client)

class CAttackRange_Cube final : public CGameObject
{
private:
	CAttackRange_Cube(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAttackRange_Cube(const CAttackRange_Cube& Prototype);
	virtual ~CAttackRange_Cube() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnCollisionEnter(void* pArg, _float fTimeDelta) override;


public:
	void Set_RangeSize(_float3& vCubePos, _float3& vCubeScale, _float fTimeDelta);
	_bool Get_Visible() { return m_bVisible; }


private:
	CTransform* m_pTransformCom = { nullptr };
	CVIBuffer_Cube* m_pVIBufferCom = { nullptr };

private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();

private:
	_float		m_fTime = 0.f;

	_bool m_bVisible = { true };

public:
	static CAttackRange_Cube* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

