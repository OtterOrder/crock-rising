#ifndef		_SceneObjectAnimated_H
#define		_SceneObjectAnimated_H

//******************************************************************

#include	"SceneObject.h"

class Texture;
class Shader;
class Mesh;
class Anim ; 

//******************************************************************

class SceneObjectAnimated : public SceneObject
{
public:
	SceneObjectAnimated();

	SceneObjectAnimated(const std::string& mesh,
						const std::string& anim,
						const std::string& Tex,
						const D3DXVECTOR3& Position,
						const std::string shader = "default_skinning.fx",
						BoundingBox aBoundingBox = BoundingBox());

	void InitObjectAnimation();

	void	Draw();
	virtual void	SetTransform(const D3DXMATRIX* world, const D3DXMATRIX* view, const D3DXMATRIX* proj);
private:
	unsigned int m_uCurrentAnim;
	std::string			m_Anim;

	Anim*		m_pAnim;

	D3DXMATRIX* m_AnimMatrices;

protected:
};

//******************************************************************
#endif		// _SceneObjectAnimated_H
