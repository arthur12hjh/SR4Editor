#include "Animation.h"

CAnimation::CAnimation(LPDIRECT3DDEVICE9 pGraphic_Device)
    : CTexture { pGraphic_Device }
    
{
}

CAnimation::CAnimation(const CAnimation& Prototype)
    : CTexture{ Prototype }
    , m_iMotionCount{ Prototype.m_iMotionCount }
    , m_iMotionIndex{ Prototype.m_iMotionIndex }
    , m_fMotionTime{ Prototype.m_fMotionTime }
    , m_fFrameSpeed{ Prototype.m_fFrameSpeed }
    , m_bPlaying{ Prototype.m_bPlaying }
    , m_bFinished{ Prototype.m_bFinished }
{
}

HRESULT CAnimation::Initialize_Prototype(TEXTURE eType, const _tchar* pTextureFilePath, _uint iMotionCount, _bool bLoop)
{
    if (FAILED(__super::Initialize_Prototype(eType, pTextureFilePath, iMotionCount)))
        return E_FAIL;

    m_iMotionCount = iMotionCount;
    m_iMotionIndex = 0;
    m_fMotionTime = 0.f;
    m_fFrameSpeed = 5.f / 60.f;
    m_bPlaying = true;
    m_bFinished = false;
    m_bLoop = bLoop;

    return S_OK;
}

HRESULT CAnimation::Initialize(void* pArg)
{

    return S_OK;
}

void CAnimation::Update_Animation(_float fTimeDelta)
{
    m_fMotionTime += fTimeDelta;
    
    if (m_fMotionTime >= m_fFrameSpeed)
    {
        m_fMotionTime -= m_fFrameSpeed;

        ++m_iMotionIndex;
           
        if (m_iMotionIndex >= m_iMotionCount)
        {  
            m_iMotionIndex = 0;
        }
    }
    
}

void CAnimation::Start_Rolling()
{
    m_bRolling = true;
    m_bRollingFrame = 0.f;
}

void CAnimation::Update_Rolling(_float fTimeDelta)
{   
    if (m_iMotionIndex >= m_iMotionCount)
    {
        m_bRolling = false;
    }
}

CAnimation* CAnimation::Create(LPDIRECT3DDEVICE9 pGraphic_Device, TEXTURE eType, const _tchar* pTextureFilePath, _uint iMotionCount, _bool bLoop)
{
    CAnimation* pInstance = new CAnimation(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype(eType, pTextureFilePath, iMotionCount, bLoop)))
    {
        MSG_BOX("Failed to Created : CAnimation");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CAnimation::Clone(void* pArg)
{
    CAnimation* pInstance = new CAnimation(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CAnimation");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAnimation::Free()
{
    __super::Free();

}