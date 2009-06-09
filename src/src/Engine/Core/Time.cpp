#include	"Time.h"

//******************************************************************

Time::Time()
{
	QueryPerformanceFrequency( (LARGE_INTEGER*)&m_Freq );
	QueryPerformanceCounter( (LARGE_INTEGER*)&m_StartDeltaTimeF );
	QueryPerformanceCounter( (LARGE_INTEGER*)&m_StartDeltaTimeE );
	m_TimeStart = GetDeltaTimeE();
}

//******************************************************************

Time::~Time()
{
}

//******************************************************************

u32 Time::GetDeltaTimeF()
{
	m_DeltaTimeF = GetDeltaTimeE() - m_StartDeltaTimeF;
	return (u32)m_DeltaTimeF;
}

//******************************************************************

u32 Time::GetDeltaTimeE()
{
	QueryPerformanceCounter( (LARGE_INTEGER*)&m_StartDeltaTimeE );
	return (u32)( m_StartDeltaTimeE * 1000 / m_Freq );
}

//******************************************************************

u32 Time::GetTime()
{
	return (u32)( GetDeltaTimeE() - m_TimeStart );
}

//******************************************************************

float Time::GetDeltaTimeESeconde()
{
	return float(GetDeltaTimeE()) / 1000.f;
}

//******************************************************************

float Time::GetDeltaTimeFSeconde()
{
	return float(GetDeltaTimeF()) / 1000.f;
}

//******************************************************************

float Time::GetTimeSeconde()
{
	return float(GetTime()) / 1000.f;
}

//******************************************************************

void Time::ResetDeltaTimeE()
{
	m_StartDeltaTimeE = 0;
}

//******************************************************************

void Time::ResetDeltaTimeF()
{
	m_DeltaTimeF = 0;
	m_StartDeltaTimeF = GetDeltaTimeE();
}
