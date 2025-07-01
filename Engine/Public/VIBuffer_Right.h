#pragma once

#include "VIBuffer.h"


NS_BEGIN(Engine)


class ENGINE_DLL CVIBuffer_Right final : public CVIBuffer
{
private:
	CVIBuffer_Right(LPDIRECT3DDEVICE9 pGraphic_Device);
	CVIBuffer_Right(const CVIBuffer_Right& Prototype);
	virtual ~CVIBuffer_Right() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	static CVIBuffer_Right* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END