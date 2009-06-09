#include	"Time.h"

//******************************************************************

Time::Time()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency( &freq );
	QueryPerformanceCounter( &m_TimeStart );
	
	m_Freq				= (float)freq.QuadPart;
	m_StartDeltaTimeE	= m_TimeStart;
	m_StartDeltaTimeF	= m_TimeStart;
	m_DeltaTimeE		= 0.f;
	m_DeltaTimeF		= 0.f;
}

//******************************************************************

Time::~Time()
{
}

//******************************************************************

u32 Time::GetDeltaTimeEMs()
{
	return (u32)(GetDeltaTimeE()*1000.f);
}

//******************************************************************

u32 Time::GetDeltaTimeFMs()
{
	return (u32)(GetDeltaTimeF()*1000.f);
}

//******************************************************************

u32 Time::GetTimeMs()
{
	return (u32)(GetTime()*1000.f);
}

//******************************************************************

float Time::GetTime()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter( &currentTime );
	return (float)(currentTime.QuadPart-m_TimeStart.QuadPart) / m_Freq;
}

//******************************************************************

void Time::EndE()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter( &currentTime );
	m_DeltaTimeE = (float)(currentTime.QuadPart-m_StartDeltaTimeE.QuadPart) / m_Freq;
	m_StartDeltaTimeE = currentTime;
}

//******************************************************************

void Time::EndF()
{
	LARGE_INTEGER currentTime;
	QueryPerformanceCounter( &currentTime );
	m_DeltaTimeF = (float)(currentTime.QuadPart-m_StartDeltaTimeF.QuadPart) / m_Freq;
	m_StartDeltaTimeF = currentTime;
}
