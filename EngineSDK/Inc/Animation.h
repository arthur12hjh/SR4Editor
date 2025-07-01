#pragma once

#include "Texture.h"

NS_BEGIN(Engine)

class ENGINE_DLL CAnimation final : public CTexture
{
private:
	CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device);
	CAnimation(const CAnimation& Prototype);
	virtual ~CAnimation() = default;

public:

	virtual HRESULT Initialize_Prototype(TEXTURE eType, const _tchar* pTextureFilePath, _uint iMotionCount, _bool bLoop);
	virtual HRESULT Initialize(void* pArg) override;

public:
	_uint Get_MotionIndex() { return m_iMotionIndex; }
	_bool Is_Finished() const { return m_bFinished; }
	_bool Is_Playing() const { return m_bPlaying; }
	_bool Is_Rolling() const { return m_bRolling; }

	void Reset() {
		m_iMotionIndex = 0;
		m_fMotionTime = 0.f; 
		m_bPlaying = true;  
		m_bFinished = false; 
	}
	_float Get_AnimationDuration() const { return m_iMotionCount * m_fFrameSpeed; }

public:
	void Update_Animation(_float fTimeDelta);
	void Start_Rolling();
	void Update_Rolling(_float fTimeDelta);

private:
	_uint	m_iMotionCount = {};
	_uint	m_iMotionIndex = {};
	_float	m_fMotionTime = {};
	_float  m_fFrameSpeed = {};

	_float	m_bRollingFrame = {};

	_bool   m_bPlaying = { true };
	_bool   m_bFinished = { false };
	_bool   m_bLoop = { false };

	_bool	m_bRolling = { false };
	

public:
	static CAnimation* Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURE eType, const _tchar* pTextureFilePath, _uint iMotionCount, _bool bLoop = false);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END