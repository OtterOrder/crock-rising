#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "SceneObject.h"

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

	virtual void	SetVisible(bool value);
	virtual bool	GetVisible();

	void			SetAnim(const std::string& anim);
	void			StartAnim();
	void			PauseAnim();
	void			StopAnim();
	void			SetAnimFPS(float fps);
	D3DXMATRIX      GetMatrixTransformBone(int indBone);
	
protected:

	void			InitObject();
	void			InitDeviceData();
	void			Draw();
	void			DrawShadow();
	void			DeleteData();
	void			UpdateAnimation();

private:
	int				m_CurrentFrame;
	std::string		m_strAnim;
	Anim*			m_pAnim;
	Mesh*			m_pBoneMesh;
	D3DXMATRIX*		m_matrices;
	Renderer*		m_Instance;
	bool			m_bShowBone;
	bool			m_bIsRunning;
	float			m_fAnimFPS;
};

