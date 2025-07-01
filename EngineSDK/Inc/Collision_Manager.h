#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CCollision_Manager final : public CBase
{
public:
	typedef struct tagCollisionComponentDesc
	{
		class CGameObject*		pObj;
		class CTransform*		pTransform;
		class CVIBuffer*		pVIBuffer;
	}COLLISION_DESC;
private:
	CCollision_Manager();
	virtual ~CCollision_Manager() = default;

public:
	HRESULT Initialize();
	void Late_Update(_float fTimeDelta);

	void	Set_Collision_Layer(class CLayer* pLayerSrc, class CLayer* pLayerDst);
	HRESULT CheckPlaneCollision(class CLayer* pLayerSrc, class CLayer* pLayerDst, _float fTimeDelta);
	_bool	PlaneCollisionEnter(CGameObject* pSrc, CGameObject* pDst);
private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	list<pair<class CLayer*, class CLayer*>> m_listCollisionLayer = {};

public:
	static CCollision_Manager* Create();
	virtual void Free();

};

NS_END;