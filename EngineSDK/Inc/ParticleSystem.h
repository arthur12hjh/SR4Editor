#pragma once

#include "AlphaObject.h"
#include "Particle.h"

NS_BEGIN(Engine)

/* ��ƼŬ �ý��� */
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
    // ��ƼŬ�� �Ӽ��� �ʱ�ȭ�ϴ� ����� �ý��ۿ� ���� �޶�����
    // ��ӹ޴� ���� ��ü���� �����ϵ��� �߻� �޼���� ��������
    virtual void ResetParticle(CParticleAttribute* attribute) = 0;
    virtual void AddParticle();
    _bool isEmpty();

protected:
    virtual void removeDeadParticles();

protected:
    virtual HRESULT Begin_RenderState();
    virtual HRESULT End_RenderState();

protected:
    // ��ƼŬ �Ŵ����� ��ġ (��ƼŬ ���� ��ġ)
    _float3                 m_vec3Origin;

    // ��ƼŬ ���� ��� ����
    _float3                 m_vec3BoundaryMin;
    _float3                 m_vec3BoundaryMax;

    // �ʴ� ���� ��ƼŬ ���� ��
    _float                   m_fEmitRate;
    // ��ƼŬ ũ��
    _float                   m_fSize;
    // ��ƼŬ ���ؽ� ����
    LPDIRECT3DVERTEXBUFFER9  m_pVertexBuffer;
    // ��ƼŬ �Ӽ� ����Ʈ
    list<CParticleAttribute>  m_particles;
    // �� ȭ�鿡�� �ý����� ���� �� �ִ� �ִ� ��ƼŬ ��
    _uint                    m_iMaxParticles;

    // ���ؽ� ���۰� ������ �� �ִ� ��ƼŬ ��
    DWORD                    m_vertexBufferSize;
    // �������� �̿�Ǵ� Ī�����̴�
    // ���縦 ������ ���� ��ƼŬ�� �ε���
    DWORD                    m_vertexBufferOffset;
    // �� ��ƼŬ ����� �ܰ迡 ���ǵ� ��ƼŬ�� ��
    DWORD                    m_vertexBufferBatchSize;

public:
    virtual CGameObject* Clone(void* pArg) = 0;
    virtual void Free() override;
};

NS_END