#pragma once
#include "Sprite.h"
#include "Vector2.h"

class CGameObject
{
protected:
	CSprite m_sprite;
	CVector2 m_vPos;
	CVector2 m_vDir;

public:
	CGameObject();
	CGameObject(CSprite* sprite);
	~CGameObject();

	virtual void Update();
	virtual void Draw(ID2D1HwndRenderTarget* pRT);
};

