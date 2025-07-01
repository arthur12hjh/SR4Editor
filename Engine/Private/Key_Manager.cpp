#include "Key_Manager.h"

CKey_Manager::CKey_Manager()
{
}

HRESULT CKey_Manager::Initialize()
{
    ZeroMemory(m_bKeyState, sizeof(m_bKeyState));

    return S_OK;
}

_bool CKey_Manager::Key_Pressing(_int _iKey)
{
    if (GetAsyncKeyState(_iKey) & 0x8000)
        return true;

    return false;
}

_bool CKey_Manager::Key_Up(_int _iKey)
{
    if ((m_bKeyState[_iKey]) && !(GetAsyncKeyState(_iKey) & 0x8000))
    {
        m_bKeyState[_iKey] = !m_bKeyState[_iKey];
        return true;
    }
}

_bool CKey_Manager::Key_Down(_int _iKey)
{
    if ((!m_bKeyState[_iKey]) && (GetAsyncKeyState(_iKey) & 0x8000))
    {
        m_bKeyState[_iKey] = !m_bKeyState[_iKey];
        return true;
    }
    return false;
}

_bool CKey_Manager::Key_Released(_int _iKey)
{
    if (!(GetAsyncKeyState(_iKey) & 0x8000))
        return true;

    return false;
}

void CKey_Manager::Update()
{
    for (int i = 0; i < VK_MAX; ++i)
    {
        if ((m_bKeyState[i]) && !(GetAsyncKeyState(i) & 0x8000))
            m_bKeyState[i] = !m_bKeyState[i];

        if ((!m_bKeyState[i]) && (GetAsyncKeyState(i) & 0x8000))
            m_bKeyState[i] = !m_bKeyState[i];
    }
}

CKey_Manager* CKey_Manager::Create()
{   
    CKey_Manager* pInstance = new CKey_Manager();

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX(" Failed to Create : CKey_Manager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CKey_Manager::Free()
{
    __super::Free();

}
