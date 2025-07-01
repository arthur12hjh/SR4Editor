#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

// 알파값을 갖고 있는 이미지들은 알파소팅이 필요하다. 오로지 그것만을 위해 만든 객?체
class ENGINE_DLL CAlphaObject abstract : public CGameObject
{
protected:
	CAlphaObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAlphaObject(const CAlphaObject& Prototype);
	virtual ~CAlphaObject() = default;

public:
	// 카메라와 나 사이의 거리
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
	// 카메라와 나 사이의 거리를 계산해주는 함수
	void Compute_CamDistance(const _float3& vWorldPos);


public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;
};

NS_END