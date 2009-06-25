#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "SceneObject.h"
#include "Objects/AnimPlayer.h"

class Texture;
class Shader;
class Mesh;
class Anim;
class Renderer;

//===========================================================================//
// Classe pour les objets animés                                             //
//===========================================================================//
class SceneObjectAnimated : public SceneObject
{
public:
	SceneObjectAnimated();
	SceneObjectAnimated(const std::string& mesh, const std::string& anim, const D3DXVECTOR3& Position);
	virtual ~SceneObjectAnimated() {}


	void			Init();
	void			SetShader(const std::string &strShader);
	virtual void	SetTransform(const D3DXMATRIX* view, const D3DXMATRIX* proj, const D3DXVECTOR3 CamPos);
	virtual void	SetTransform(const D3DXMATRIX* world);

	virtual void	Update();

	virtual void	SetVisible(bool value);
	virtual bool	GetVisible();

	void			SetAnim(const std::string& anim);
	void			SetAnimFPS(float fps);
	D3DXMATRIX      GetMatrixTransformBone(int indBone);

	inline bool		IsAtEnd ()	{ return m_AnimPlayer.IsAtEnd(); };
	inline float	GetSpeed ()	{ return m_AnimPlayer.GetSpeed(); };
	inline void		SetSpeed (float _speed)		{ m_AnimPlayer.SetSpeed(_speed); };
	inline void		SetLoop (bool _loop)	{ m_AnimPlayer.SetLoop(_loop); };
	inline void		Play	()		{ m_AnimPlayer.Play(); };
	inline void		Pause	()		{ m_AnimPlayer.Pause(); };
	inline void		Stop	()		{ m_AnimPlayer.Stop(); }
	inline bool		IsPlaying ()	{ return m_AnimPlayer.IsPlaying(); };
	
protected:

	void			InitObject();
	void			InitDeviceData();
	void			Draw();
	void			DrawShadow();
	void			DeleteData();
	void			UpdateAnimation();

private:
	u32				m_CurrentFrame;
	std::string		m_strAnim;
	Anim*			m_pAnim;
	Mesh*			m_pBoneMesh;
	D3DXMATRIX*		m_matrices;
	Renderer*		m_Instance;
	bool			m_bShowBone;

	AnimPlayer		m_AnimPlayer;
};

