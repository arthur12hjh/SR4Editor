#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CKey_Manager final : public CBase
{
private:
	CKey_Manager();
	virtual ~CKey_Manager() = default;

public:
	HRESULT Initialize();

	_bool		Key_Pressing(_int _iKey);

	// 눌렀다가 뗐을 때
	_bool		Key_Up(_int _iKey);
	// 누른 타이밍 딱 한 번 만
	_bool		Key_Down(_int _iKey);

	_bool		Key_Released(_int _iKey);


	void		Update();

private:
	bool		m_bKeyState[VK_MAX] = {};


public:
	static CKey_Manager* Create();
	virtual void Free() override;

};

NS_END