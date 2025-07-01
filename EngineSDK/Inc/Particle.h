#pragma once

#include "Base.h"

NS_BEGIN(Engine)

/* ��ƼŬ ����ü ���� */
class ENGINE_DLL CParticle final
{
public:
	D3DXVECTOR3 _position = {};         // ��ƼŬ�� ��ġ�� �����Ѵ�
	D3DCOLOR    _color = {};        // ��ƼŬ ���� �����Ѵ�
	static const DWORD FVF;    // FVF��.
};


/* ��ƼŬ �Ӽ� ���� */
class ENGINE_DLL CParticleAttribute final
{
public:
    D3DXVECTOR3     _position = {};     // ���彺���̽� �� ��ƼŬ ��ġ
    D3DXVECTOR3     _velocity = {};     // �ʴ� �̵� �ӵ�
    D3DXVECTOR3     _acceleration = {}; // �ʴ� ���� �ӵ�
    _float          _lifeTime = {};     // ��ƼŬ ����ð�
    _float          _age = {};          // ��ƼŬ�� ���� ����
    D3DXCOLOR       _color = {};        // ��ƼŬ�� ��
    D3DXCOLOR       _colorFade = {};    // ��ƼŬ ���� ����ϴ� ���
    _bool           _isAlive = {true};      // ��ƼŬ ���� ����
};



NS_END