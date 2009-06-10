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

	u32 GetDeltaTimeMs();
	u32 GetTimeMs();
	
	inline float GetDeltaTime() { return m_DeltaTime; }
	float GetTime();

	void EndE();	// Fin d'un tour moteur (System)

protected:

	// =========================================================
	// Données protégées

	float			m_Freq;
	LARGE_INTEGER	m_TimeStart;
	LARGE_INTEGER	m_StartDeltaTime;
	float			m_DeltaTime;

};

//******************************************************************
#endif		// _TIME_H_
