#pragma once

#include "Component.h"

// ���⼭ �� Ŭ���̾�Ʈ�� �ִ� �÷��̾ ������ ����(enum) walk_left, Jump_back�� �޾Ƽ�
// enter, exit, keyInput, Update ó���ϰ� 
// ������� �����ϴ� ���� �ٸ� ����� ���� ó�� ���

NS_BEGIN(Engine);

class ENGINE_DLL CState_Machine final : public CComponent
{
private:
	CState_Machine(LPDIRECT3DDEVICE9 pGraphic_Device);
	CState_Machine(const CState_Machine& Prototype);
	virtual ~CState_Machine() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg) override;
	
public:
	void Enter(_uint iStateIndex);	// ���¿� ����
	void Exit(_uint iStateIndex);	// ���� ����
	void KeyInput();			// �Է°� ��������

	void State_Waiting();
	void State_Find();
	void State_Move();
	void State_Attacking();

	void State_Rolling();
	void State_Jumping();


private:
	_uint m_iStateIndex = {};
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CState_Machine* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;

};

NS_END

