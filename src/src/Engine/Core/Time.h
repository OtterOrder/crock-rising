#ifndef		_TIME_H_
#define		_TIME_H_

//******************************************************************

#include	<windows.h>
#include	"Core/Types/Types.h"

//******************************************************************

class Time
{
public:

	Time();
	~Time();

	// =========================================================
	// Méthodes publiques
	u32 GetDeltaTimeF();
	u32 GetDeltaTimeE();
	u32 GetTime();

	float GetDeltaTimeESeconde();
	float GetDeltaTimeFSeconde();
	float GetTimeSeconde();

	void ResetDeltaTimeE();
	void ResetDeltaTimeF();

protected:

	// =========================================================
	// Données protégées
	__int64 m_StartDeltaTimeE;
	__int64 m_StartDeltaTimeF;
	__int64 m_DeltaTimeF;
	__int64 m_TimeStart;
	__int64 m_Freq;

};

//******************************************************************
#endif		// _TIME_H_
