#include "stdafx.h"
#include "Player.h"


CPlayer::CPlayer()
{
}

CPlayer::CPlayer(CSprite* sprite)
{
	m_sprite = *sprite;
	m_vPos = CVector2(0.0f, 0.0f);
	m_vDir = CVector2(1.0f, 0.0f);
}

CPlayer::~CPlayer()
{
}

void CPlayer::Update()
{

}

void CPlayer::Draw(ID2D1HwndRenderTarget* pRT)
{
	CGameObject::Draw(pRT);
}

void CPlayer::KeyboardMessage(UINT message, WPARAM wParam)
{
	if (message == WM_KEYDOWN)
		KeyDown();
	if (message == WM_KEYUP)
		KeyUp();
}

void CPlayer::KeyDown()
{
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
		m_vPos.x -= 1.0f;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		m_vPos.x += 1.0f;
}

void CPlayer::KeyUp()
{

}