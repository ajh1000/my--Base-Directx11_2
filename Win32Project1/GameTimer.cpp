#include "stdafx.h"
#include "GameTimer.h"



CGameTimer::CGameTimer()
{

	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;

	char str[10];

	sprintf(str, "FPS : %d", m_cnt);

	SetWindowText(gameUtil.GetHWND(), str);
}


CGameTimer::~CGameTimer()
{
}

void CGameTimer::update()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;

	// Time difference between this frame and the previous.
	mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;

	// Prepare for next frame.
	mPrevTime = mCurrTime;

	// Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
	// processor goes into a power save mode or we get shuffled to another
	// processor, then mDeltaTime can be negative.
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
	else if (mDeltaTime >= 10.0)
	{
		mDeltaTime = 0.0;
	}

	m_elapsedTime += mDeltaTime;
	m_cnt++;

	if (m_elapsedTime >= 1.f)
	{
		
		char str[100] = {};

		sprintf(str, "FPS : %d", m_cnt);

		BOOL a=SetWindowText(gameUtil.GetHWND(), str);
		

		m_elapsedTime = 0;
		m_cnt = 0;
	}
}

float CGameTimer::getDeltaTime()
{


	return mDeltaTime;
}
