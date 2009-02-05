#include	"Time.h"

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