#pragma once

#include "Core/Types/Types.h"

//===========================================================================//
// Include                                                                   //
//===========================================================================//

//===========================================================================//
// Classe pour les objets anim�s                                             //
//===========================================================================//
class AnimPlayer
{
	enum AnimFlag
	{
		Flag_Loop	= 1 << 0,
		Flag_AtEnd	= 1 << 1,
		Flag_Pause	= 1 << 2
	};

	u32			m_Flags;

	float		m_Time;
	float		m_Fps;
	u32			m_NbFrames;
	u32			m_CurrentFrame;
	float		m_Speed;

public:
	AnimPlayer  ();
	~AnimPlayer ()	{};

	void Init ();

	void Update (float _Dt);	// seconds

	inline bool		IsAtEnd ()	{ return (m_Flags & Flag_AtEnd? true : false); };

	inline float	GetFps()	{ return m_Fps; };
	inline void		SetFps (float _fps)		{ m_Fps = _fps; };

	inline float	GetSpeed ()	{ return m_Speed; };
	inline void		SetSpeed (float _speed)		{ m_Speed = _speed; };

	inline u32		GetNbFrames ()	{ return m_NbFrames; };
	inline void		SetNbFrames (u32 _nbFrames)		{ m_NbFrames = _nbFrames; };

	inline u32		GetCurrentFrame ()	{ return m_CurrentFrame; };

	inline void		SetLoop (bool _loop)	{ if (_loop) m_Flags |= Flag_Loop; };

	inline void		Play	()		{ m_Flags &= ~Flag_Pause; };
	inline void		Pause	()		{ m_Flags |= Flag_Pause; };
		   void		Stop	();
	inline bool		IsPlaying ()	{ return (m_Flags & Flag_Pause? false : true); };
};