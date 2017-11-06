#include "stdafx.h"
#include "Timer.h"


CTimer::CTimer()
{
	__int64 CountPerSecond;
	QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER*>(&CountPerSecond));

	m_nPrevCnt = 0;
	m_dSecondPerCount = 1.0 / static_cast<double>(CountPerSecond);
	m_dDeltaTime = 0.0;
	m_dTotalDeltaTime = 0.0;
	m_dSecondPerUpdateCnt = 0.017f; //1.0 / 60 1초에 60번 실행 0.017f
}

CTimer::~CTimer()
{

}

void CTimer::Reset()
{
	__int64 currCnt;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currCnt));

	m_nPrevCnt = currCnt;
}

void CTimer::Tick()
{
	__int64 currCnt;
	QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER*>(&currCnt));

	m_dDeltaTime = static_cast<double>(currCnt - m_nPrevCnt) * m_dSecondPerCount;
	m_nPrevCnt = currCnt;
}

float CTimer::GetDeltaTime()
{
	return static_cast<float>(m_dDeltaTime);
}

bool CTimer::FixedFPS()
{
	//다음 프레임까지 델타타임을 더한다
	m_dTotalDeltaTime += m_dDeltaTime;

	if (m_dTotalDeltaTime > m_dSecondPerUpdateCnt)
	{
		m_dTotalDeltaTime = 0.0;
		return true;
	}
	return false;
}


