#pragma once

#include "AlphaObject.h"
#include "Particle.h"

NS_BEGIN(Engine)

/* 파티클 시스템 */
/* ParticleSystem.h */
class ENGINE_DLL CParticleSystem abstract : public CAlphaObject
{
public:
    typedef struct tagParticleSystemDesc
    {
        int iNumParticles;
        _float3 vec3BoundaryMin;
        _float3 vec3BoundaryMax;
    }PARTICLESYSTEM_DESC;
protected:
    CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device);
    CParticleSystem(const CParticleSystem& Prototype);
    virtual ~CParticleSystem() = default;

public:
    virtual HRESULT Initialize_Prototype();
    virtual HRESULT Initialize(void *pArg);
    virtual void Priority_Update(_float fTimeDelta);
    virtual void Update(_float fTimeDelta);
    virtual void Late_Update(_float fTimeDelta);
    virtual HRESULT Render();

public:
    virtual void Reset();
    // 파티클의 속성을 초기화하는 방법은 시스템에 따라 달라진다
    // 상속받는 하위 객체에서 구현하도록 추상 메서드로 선언하자
    virtual void ResetParticle(CParticleAttribute* attribute) = 0;
    virtual void AddParticle();
    _bool isEmpty();

protected:
    virtual void removeDeadParticles();

protected:
    virtual HRESULT Begin_RenderState();
    virtual HRESULT End_RenderState();

protected:
    // 파티클 매니저의 위치 (파티클 생성 위치)
    _float3                 m_vec3Origin;

    // 파티클 생성 경계 범위
    _float3                 m_vec3BoundaryMin;
    _float3                 m_vec3BoundaryMax;

    // 초당 생성 파티클 입자 수
    _float                   m_fEmitRate;
    // 파티클 크기
    _float                   m_fSize;
    // 파티클 버텍스 버퍼
    LPDIRECT3DVERTEXBUFFER9  m_pVertexBuffer;
    // 파티클 속성 리스트
    list<CParticleAttribute>  m_particles;
    // 한 화면에서 시스템이 가질 수 있는 최대 파티클 수
    _uint                    m_iMaxParticles;

    // 버텍스 버퍼가 보관할 수 있는 파티클 수
    DWORD                    m_vertexBufferSize;
    // 렌더링에 이용되는 칭구들이다
    // 복사를 시작할 다음 파티클의 인덱스
    DWORD                    m_vertexBufferOffset;
    // 한 파티클 드로잉 단계에 정의된 파티클의 수
    DWORD                    m_vertexBufferBatchSize;

public:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;
};

NS_END