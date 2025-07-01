#include "ParticleSystem.h"


CParticleSystem::CParticleSystem(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CAlphaObject { pGraphic_Device }
{
}

CParticleSystem::CParticleSystem(const CParticleSystem& Prototype)
	: CAlphaObject{ Prototype }
    //, m_vec3Origin { Prototype.m_vec3Origin }
    //, m_vec3BoundaryMin {Prototype.m_vec3BoundaryMin }
    //, m_vec3BoundaryMax{ Prototype.m_vec3BoundaryMax }
    //, m_fEmitRate {Prototype.m_fEmitRate}
    //, m_fSize {Prototype.m_fSize}
    , m_pVertexBuffer{ Prototype.m_pVertexBuffer }
    //, m_particles {Prototype.m_particles}
    //, m_iMaxParticles {Prototype.m_iMaxParticles}
    //, m_vertexBufferSize {Prototype.m_vertexBufferSize}
    //, m_vertexBufferOffset {Prototype.m_vertexBufferOffset}
    //, m_vertexBufferBatchSize {Prototype.m_vertexBufferBatchSize}
{
}

HRESULT CParticleSystem::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CParticleSystem::Initialize(void* pArg)
{

	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(
		m_vertexBufferSize * sizeof(CParticle),
		D3DUSAGE_DYNAMIC | D3DUSAGE_POINTS | D3DUSAGE_WRITEONLY,
		CParticle::FVF,
		D3DPOOL_DEFAULT,
		&m_pVertexBuffer,
		0)))
		return E_FAIL;

	return S_OK;
}

void CParticleSystem::Priority_Update(_float fTimeDelta)
{
}

void CParticleSystem::Update(_float fTimeDelta)
{
}

void CParticleSystem::Late_Update(_float fTimeDelta)
{
}

HRESULT CParticleSystem::Render()
{
    if (!m_particles.empty())
    {
        // 먼저 렌더 상태를 지정하자
        if (FAILED(Begin_RenderState()))
            return E_FAIL;

        m_pGraphic_Device->SetFVF(CParticle::FVF);
        m_pGraphic_Device->SetStreamSource(0, m_pVertexBuffer, 0, sizeof(CParticle));

        // 할당할 버텍스 버퍼의 오프셋과 크기를 지정해주자
        // 버텍스 버퍼의 크기를 벗어날 경우 오프셋을 0으로 초기화해준다
        if (m_vertexBufferOffset >= m_vertexBufferSize)
            m_vertexBufferOffset = 0;

        // 그래픽카드가 그릴 버텍스 버퍼 세그먼트
        CParticle* pParticles = {};

        m_pVertexBuffer->Lock(
            m_vertexBufferOffset * sizeof(CParticle),
            m_vertexBufferBatchSize * sizeof(CParticle),
            (void**)&pParticles,
            m_vertexBufferOffset ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);

        DWORD iNumParticlesInBatch = 0;

        // 버텍스 버퍼의 오프셋과 크기를 알아냈으니 복사를 시작하자
        list<CParticleAttribute>::iterator iter;
        for (iter = m_particles.begin(); iter != m_particles.end(); ++iter)
        {
            if (iter->_isAlive)
            {
                // 일단 전 프레임에서 생존한 파티클을
                // 다음 버텍스 버퍼 세그먼트로 복사하자
                pParticles->_position = iter->_position;
                pParticles->_color = iter->_color;
                ++pParticles;
                ++iNumParticlesInBatch;

                // 이번 프레임의 복사작업을 마쳤다면 파티클을 그리자
                if (iNumParticlesInBatch == m_vertexBufferBatchSize)
                {
                    m_pVertexBuffer->Unlock();

                    m_pGraphic_Device->DrawPrimitive(
                        D3DPT_POINTLIST,
                        m_vertexBufferOffset,
                        m_vertexBufferBatchSize);

                    // 그래픽카드와 동시에 작업할 것이기에
                    // 다음 프레임의 파티클을 채우자
                    m_vertexBufferOffset += m_vertexBufferBatchSize;

                    // 할당할 버텍스 버퍼의 오프셋과 크기를 지정해주자
                  // 버텍스 버퍼의 크기를 벗어날 경우 오프셋을 0으로 초기화해준다
                    if (m_vertexBufferOffset >= m_vertexBufferSize)
                        m_vertexBufferOffset = 0;


                    m_pVertexBuffer->Lock(
                        m_vertexBufferOffset * sizeof(CParticle),
                        m_vertexBufferBatchSize * sizeof(CParticle),
                        (void**)&pParticles,
                        m_vertexBufferOffset ? D3DLOCK_NOOVERWRITE :
                        D3DLOCK_DISCARD);

                    iNumParticlesInBatch = 0;
                }
            }
        }

        m_pVertexBuffer->Unlock();

        // 간헐적으로 배치크기보다 파티클 수가 적어
        // 드로잉이 안되기도 하기에
        // 예비역으로 남겨주자
        // 근데 이러면 그래픽카드가 버텍스 버퍼 배치 단계에선 쉬게 된다.
        if (iNumParticlesInBatch)
        {
            m_pGraphic_Device->DrawPrimitive(
                D3DPT_POINTLIST,
                m_vertexBufferOffset,
                iNumParticlesInBatch);
        }

        m_vertexBufferOffset += m_vertexBufferBatchSize;

        if (FAILED(End_RenderState()))
            return E_FAIL;
    }

    return S_OK;
}

void CParticleSystem::Reset()
{
    list<CParticleAttribute>::iterator iter;

    for (iter = m_particles.begin(); iter != m_particles.end(); ++iter)
    {
        // ResetParticle 함수는 추상 메소드이기떄문에
        // 무조건 자식 클래스에서 재정의 해줘야한다.
        ResetParticle(&(*iter));
    }
}

void CParticleSystem::AddParticle()
{
    CParticleAttribute _attribute = {};
    ResetParticle(&_attribute);

    m_particles.push_back(_attribute);
}

_bool CParticleSystem::isEmpty()
{
    if (m_particles.empty())
        return true;

    return false;
}

void CParticleSystem::removeDeadParticles()
{
    list<CParticleAttribute>::iterator iter;

    iter = m_particles.begin();

    while (iter != m_particles.end())
    {
        if (iter->_isAlive == false)
        {
            iter = m_particles.erase(iter);
        }
        else
        {
            iter++;
        }
    }
}

HRESULT CParticleSystem::Begin_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, false);
    m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE, TypetoDW(m_fSize));
    m_pGraphic_Device->SetRenderState(D3DRS_POINTSIZE_MIN, TypetoDW(0.f));

    // POINTSCALE_A,B,C를 통해 거리에 따른 파티클 크기를 제어하자
    m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_A, TypetoDW(0.f));
    m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_B, TypetoDW(0.f));
    m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALE_C, TypetoDW(1.f));

    // 알파 블렌딩을 적용시키자
    // 텍스쳐의 알파채널을 받아와 블렌딩을 해준다
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphic_Device->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

    return S_OK;
}

HRESULT CParticleSystem::End_RenderState()
{
    m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

    m_pGraphic_Device->SetRenderState(D3DRS_POINTSPRITEENABLE, false);
    m_pGraphic_Device->SetRenderState(D3DRS_POINTSCALEENABLE, false);
    m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, false);


    m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

    return S_OK;
}

void CParticleSystem::Free()
{
    __super::Free();

}
