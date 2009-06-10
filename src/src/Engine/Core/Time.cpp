#include	"Time.h"

//******************************************************************

Time::Time()
{
	LARGE_INTEGER freq;
	QueryPerformanceFrequency( &freq );
	QueryPerformanceCounter( &m_TimeStart );
	
	m_Freq				= (float)freq.QuadPart;
	m_StartDeltaTime	= m_TimeStart;
	m_DeltaTime			= 0.f;
}

//******************************************************************

Time::~Time()
{
}

//******************************************************************

u32 Time::GetDeltaTimeMs()
{
	return (u32)(GetDeltaTime()*1000.f);
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
	m_DeltaTime = (float)(currentTime.QuadPart-m_StartDeltaTime.QuadPart) / m_Freq;
	m_StartDeltaTime = currentTime;
}
