#pragma once
#include <Windows.h>


class CTimer
{
private:
	__int64 m_nPrevCnt;					//���� ������ ����
	double m_dSecondPerCount;			//���� �� ��(�ʴ� ������ ��)
	double m_dDeltaTime;				//�� ������ �� �ð�
	double m_dTotalDeltaTime;			//���� ������Ʈ ������ ��ŸŸ�� �ջ�
	double m_dSecondPerUpdateCnt;		//���� Ƚ�� �� ��

public:
	CTimer();
	~CTimer();
	void Reset();
	void Tick();
	float GetDeltaTime();
	bool IsNextFrameTime();
};

