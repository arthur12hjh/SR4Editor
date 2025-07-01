#pragma once

#include "Base.h"

NS_BEGIN(Engine)

/* 파티클 구조체 포맷 */
class ENGINE_DLL CParticle final
{
public:
	D3DXVECTOR3 _position = {};         // 파티클의 위치를 저장한다
	D3DCOLOR    _color = {};        // 파티클 색을 저장한다
	static const DWORD FVF;    // FVF값.
};


/* 파티클 속성 포맷 */
class ENGINE_DLL CParticleAttribute final
{
public:
    D3DXVECTOR3     _position = {};     // 월드스페이스 내 파티클 위치
    D3DXVECTOR3     _velocity = {};     // 초당 이동 속도
    D3DXVECTOR3     _acceleration = {}; // 초당 가속 속도
    _float          _lifeTime = {};     // 파티클 생명시간
    _float          _age = {};          // 파티클의 현재 나이
    D3DXCOLOR       _color = {};        // 파티클의 색
    D3DXCOLOR       _colorFade = {};    // 파티클 색이 퇴색하는 방법
    _bool           _isAlive = {true};      // 파티클 생존 여부
};



NS_END