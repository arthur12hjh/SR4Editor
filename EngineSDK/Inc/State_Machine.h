#pragma once

#include "Component.h"

// 여기서 뭐 클라이언트에 있는 플레이어나 몬스터의 상태(enum) walk_left, Jump_back등 받아서
// enter, exit, keyInput, Update 처리하고 
// 예를들어 공격하는 도중 다른 모션은 금지 처리 등등

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
	void Enter(_uint iStateIndex);	// 상태에 입장
	void Exit(_uint iStateIndex);	// 상태 종료
	void KeyInput();			// 입력값 들어왔을때

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

