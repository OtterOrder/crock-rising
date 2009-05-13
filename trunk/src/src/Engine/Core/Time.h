#ifndef	_Time_H
#define	_Time_H

//******************************************************************
#include <windows.h>

class Time
{
public:

	Time();
	~Time();

	// =========================================================
	// Méthodes publiques
	unsigned int getDeltaTimeF();
	unsigned int getDeltaTimeE();
	unsigned int getTime();

	float getDeltaTimeESeconde()	{	return float(getDeltaTimeE()) / 1000.0f;	}
	float getDeltaTimeFSeconde()	{	return float(getDeltaTimeF()) / 1000.0f;	}
	float getTimeSeconde()			{	return float(getTime()) / 1000.0f;			}

	void resetDeltaTimeE()	{	startDeltaTimeE = 0;	}
	void resetDeltaTimeF()	{	deltaTimeF = 0;	startDeltaTimeF = getDeltaTimeE();	}

protected:

	// =========================================================
	// Données protégées
	__int64 startDeltaTimeE;
	__int64 startDeltaTimeF;
	__int64 deltaTimeF;
	__int64 timeStart;
	__int64 freq;
};

//******************************************************************
#endif		// _Time_H