#ifndef		_TIME_H_
#define		_TIME_H_

//******************************************************************

#ifndef		NOMINMAX
#define		NOMINMAX
#endif

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

	u32 GetDeltaTimeEMs();
	u32 GetDeltaTimeFMs();
	u32 GetTimeMs();
	
	inline float GetDeltaTimeE() { return m_DeltaTimeE; }
	inline float GetDeltaTimeF() { return m_DeltaTimeF; }
	float GetTime();

	void EndE();	// Fin d'un tour moteur (System)
	void EndF();	// Fin d'un frame (System)

protected:

	// =========================================================
	// Données protégées

	float			m_Freq;
	LARGE_INTEGER	m_TimeStart;
	LARGE_INTEGER	m_StartDeltaTimeE;
	LARGE_INTEGER	m_StartDeltaTimeF;
	float			m_DeltaTimeE;
	float			m_DeltaTimeF;

};

//******************************************************************
#endif		// _TIME_H_
