#pragma once

#include "VIBuffer.h"

NS_BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Top final : public CVIBuffer
{
private:
	CVIBuffer_Top(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Top(const CVIBuffer_Top& Prototype);
	virtual ~CVIBuffer_Top() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual _bool Picking(class CTransform* pTransform, _float3* pOut)  override;

public:
	static CVIBuffer_Top* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END