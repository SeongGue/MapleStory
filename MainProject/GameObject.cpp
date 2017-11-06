#include "stdafx.h"
#include "GameObject.h"


CGameObject::CGameObject()
{
	
}

CGameObject::CGameObject(CSprite* sprite)
{
	m_sprite = *sprite;
	m_vPos = CVector2(0.0f, 0.0f);
	m_vDir = CVector2(1.0f, 0.0f);
}

CGameObject::~CGameObject()
{
}

void CGameObject::Update()
{

}

void CGameObject::Draw(ID2D1HwndRenderTarget* pRT)
{
	m_sprite.Draw(pRT, m_vPos);
}
