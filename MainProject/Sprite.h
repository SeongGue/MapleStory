#pragma once
#include "Vector2.h"

class CSprite
{
	ID2D1Bitmap* m_sprite;
	int m_nWidth;
	int m_nHeight;
	int m_nAnimType; //�ִϸ��̼� ���� ����
	int* m_pAnimCnt; //�ִϸ��̼� ���� �� ������ ����
	int m_dCurrAnimType; //���� ������� �ִϸ��̼�
	int m_dCurrFrame; //���� ������
	float m_fAnimWidth;
	float m_fAnimHeight;

public:
	CSprite();
	~CSprite();
	void Draw(ID2D1HwndRenderTarget* pRT, CVector2 pos);
	void SetSprite(ID2D1Bitmap* sprite) { m_sprite = sprite; }
	void SetWidth(int width) { m_nWidth = width; }
	void SetHeight(int height) { m_nHeight = height; }
	void SetAnimType(int animType) { m_nAnimType = animType; }
	void AllocAnimCnt() { m_pAnimCnt = new int[m_nAnimType]; }
	void SetAnimCnt(int index, int num) { m_pAnimCnt[index] = num; }
	void CalcAnimSize();
	int GetMaxAinmCnt();
};

