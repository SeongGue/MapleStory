#pragma once
#include "GameObject.h"

enum State{Idle, };

class CPlayer : public CGameObject
{
public:
	CPlayer();
	CPlayer(CSprite* sprite);
	~CPlayer();

	virtual void Update();
	virtual void Draw(ID2D1HwndRenderTarget* pRT);
	void KeyboardMessage(UINT message, WPARAM wParam);
	void KeyDown();
	void KeyUp();
};

