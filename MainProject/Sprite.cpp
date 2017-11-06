#include "stdafx.h"
#include "Sprite.h"


CSprite::CSprite()
{
	m_sprite = NULL;
	m_nWidth = 0;
	m_nHeight = 0;
	m_nAnimType = 0;
	m_pAnimCnt = NULL;
	m_dCurrAnimType = 0;
	m_dCurrFrame = 0;
	m_fAnimWidth = 0.0f;
	m_fAnimHeight = 0.0f;
}

CSprite::~CSprite()
{
}

void CSprite::Draw(ID2D1HwndRenderTarget* pRT, CVector2 pos)
{
	float x = pos.x;
	float y = pos.y;
	D2D1_RECT_F rectangle1 = D2D1::RectF( //�׷��� ��ġ�� ����
		x,
		y,
		x + m_fAnimWidth,
		y + m_fAnimHeight
	);

	float imgLocationX = m_fAnimWidth * m_dCurrFrame;
	float imgLocationY = m_fAnimHeight * m_dCurrAnimType;
	D2D1_RECT_F rectangle2 = D2D1::RectF( //�׷��� �̹��� ����
		imgLocationX,
		imgLocationY,
		imgLocationX + m_fAnimWidth,
		imgLocationY + m_fAnimHeight
	);
	pRT->DrawBitmap(m_sprite, rectangle1, 0.5f, D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, rectangle2);
	m_dCurrFrame += 1;
	m_dCurrFrame %= m_pAnimCnt[m_dCurrAnimType];
}

void CSprite::CalcAnimSize()
{ 
	m_fAnimWidth = m_nWidth / GetMaxAinmCnt();
	m_fAnimHeight = m_nHeight / m_nAnimType;
}

int CSprite::GetMaxAinmCnt()
{
	//�ִϸ��̼� �� �ִ� ������ ������ ã�Ƴ���.
	int maxNum = 0;
	for (int i = 0; i < m_nAnimType; ++i)
	{
		if (maxNum < m_pAnimCnt[i])
			maxNum = m_pAnimCnt[i];
	}
	return maxNum;
}