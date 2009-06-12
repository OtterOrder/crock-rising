#include	"Time.h"

////******************************************************************
//
//Time::Time()
//{
//	LARGE_INTEGER freq;
//	QueryPerformanceFrequency( &freq );
//	QueryPerformanceCounter( &m_TimeStart );
//	
//	m_Freq				= (float)freq.QuadPart;
//	m_StartDeltaTime	= m_TimeStart;
//	m_DeltaTime			= 0.f;
//}
//
////******************************************************************
//
//Time::~Time()
//{
//}
//
////******************************************************************
//
//u32 Time::GetDeltaTimeMs()
//{
//	return (u32)(GetDeltaTime()*1000.f);
//}
//
////******************************************************************
//
//u32 Time::GetTimeMs()
//{
//	return (u32)(GetTime()*1000.f);
//}
//
////******************************************************************
//
//float Time::GetTime()
//{
//	LARGE_INTEGER currentTime;
//	QueryPerformanceCounter( &currentTime );
//	return (float)(currentTime.QuadPart-m_TimeStart.QuadPart) / m_Freq;
//}
//
////******************************************************************
//
//void Time::EndE()
//{
//	LARGE_INTEGER currentTime;
//	QueryPerformanceCounter( &currentTime );
//	m_DeltaTime = (float)(currentTime.QuadPart-m_StartDeltaTime.QuadPart) / m_Freq;
//	m_StartDeltaTime = currentTime;
//}

Time::Time()
{
	QueryPerformanceFrequency( (LARGE_INTEGER*)&freq );
	QueryPerformanceCounter( (LARGE_INTEGER*)&startDeltaTimeF );
	QueryPerformanceCounter( (LARGE_INTEGER*)&startDeltaTimeE );
	timeStart = getDeltaTimeE();
}

Time::~Time()
{
}

unsigned int Time::getDeltaTimeF()
{
	deltaTimeF = getDeltaTimeE() - startDeltaTimeF;
	return (unsigned int)(deltaTimeF);
}

unsigned int Time::getDeltaTimeE()
{
	QueryPerformanceCounter( (LARGE_INTEGER*)&startDeltaTimeE );
	return (unsigned int)(startDeltaTimeE * 1000 / freq);
}

unsigned int Time::getTime()
{
	return (unsigned int)(getDeltaTimeE() - timeStart);
}