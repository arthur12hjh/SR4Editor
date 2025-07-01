#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CLand final : public CComponent
{
protected:
	CLand(LPDIRECT3DDEVICE9 pGraphic_Device);
	CLand(const CLand& Prototype);
	virtual ~CLand() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	void Set_Transform(class CTransform* pTransform);
	void Set_TerrainTransform(class CTransform* pTerrainTransform);
	void Set_TerrainVIBuffer(class CVIBuffer* pTerrainVIBuffer);
	
	class CTransform* Get_LandTransform() { return m_pTerrainTransform; }
	class CVIBuffer* Get_LandVIBuffer() { return m_pTerrainVIBuffer; }

	_float Standing_On_Terrain(_float fOffset = 0.f);

protected:
	class CTransform*	m_pTransform = { nullptr };
	class CTransform*	m_pTerrainTransform = { nullptr };
	class CVIBuffer*	m_pTerrainVIBuffer = { nullptr };

public:
	static CLand* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END