#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

// ���İ��� ���� �ִ� �̹������� ���ļ����� �ʿ��ϴ�. ������ �װ͸��� ���� ���� ��?ü
class ENGINE_DLL CAlphaObject abstract : public CGameObject
{
protected:
	CAlphaObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAlphaObject(const CAlphaObject& Prototype);
	virtual ~CAlphaObject() = default;

public:
	// ī�޶�� �� ������ �Ÿ�
	_float Get_CamDistance() const {
		return m_fCamDistance;
	}

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);
	virtual void Priority_Update(_float fTimeDelta);
	virtual void Update(_float fTimeDelta);
	virtual void Late_Update(_float fTimeDelta);
	virtual HRESULT Render();

protected:
	_float			m_fCamDistance = {};

protected:
	// ī�޶�� �� ������ �Ÿ��� ������ִ� �Լ�
	void Compute_CamDistance(const _float3& vWorldPos);


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END