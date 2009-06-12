#ifndef		_TIME_H_
#define		_TIME_H_

//******************************************************************

#ifndef		NOMINMAX
#define		NOMINMAX
#endif

#include	<windows.h>
#include	"Core/Types/Types.h"

//******************************************************************

// class Time
// {
// public:
// 
// 	Time();
// 	~Time();
// 
// 	// =========================================================
// 	// Méthodes publiques
// 
// 	u32 GetDeltaTimeMs();
// 	u32 GetTimeMs();
// 	
// 	inline float GetDeltaTime() { return m_DeltaTime; }
// 	float GetTime();
// 
// 	void EndE();	// Fin d'un tour moteur (System)
// 
// protected:
// 
// 	// =========================================================
// 	// Données protégées
// 
// 	float			m_Freq;
// 	LARGE_INTEGER	m_TimeStart;
// 	LARGE_INTEGER	m_StartDeltaTime;
// 	float			m_DeltaTime;
// 
// };

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
#endif		// _TIME_H_
