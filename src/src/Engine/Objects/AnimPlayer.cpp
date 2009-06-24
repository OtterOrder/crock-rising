//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "AnimPlayer.h"

//===========================================================================//
// Constructeur SceneAnimObject                                              //
//===========================================================================//
AnimPlayer::AnimPlayer()
{
	m_Flags = 0;

	m_Time = 0.f;
	m_Fps = 0.f;
	m_NbFrames = 1;
	m_CurrentFrame = 0;
	m_Speed = 1.f;
}

void AnimPlayer::Update (float _Dt)
{
	m_Flags &= ~AtEnd;

	m_Time += _Dt*m_Speed;
	m_CurrentFrame = (u32) (m_Time*m_Fps);

	if (m_CurrentFrame >= m_NbFrames)
	{
		m_Flags |= AtEnd;
		m_CurrentFrame = m_NbFrames -1;
		m_Time = (float)m_CurrentFrame / (float)m_NbFrames;

		if (m_Flags & Loop)
			m_Time = 0.f;
	}
}