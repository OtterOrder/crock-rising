#ifndef		_Light_H
#define		_Light_H

//******************************************************************

#include "Object.h"
#include "Core/Types/Vector.h"
#include "../Resources/Shader.h"
#include "../Resources/ResourceManager.h"

struct ColorValue 
{
	ColorValue( float _r=0.f, float _g=0.f, float _b=0.f, float _a=1.f )	{	r = _r; g = _g; b = _b; alpha = _a;		}

	float r ;
	float g ;
	float b ;
	float alpha ;
};

//******************************************************************

class Light : public Object
{
public:
	enum TypeLight	{	LT_AMBIANT, LT_DIRECTIONNEL, LT_POINTLIGHT, LT_SPOTLIGHT	};

	Light();
	Light(Vector3f position , Vector3f direction , ColorValue diffuse , ColorValue specular , ColorValue ambient, TypeLight type );

	void setDiffuse		(ColorValue diffuse)	{ m_diffuse = diffuse ; }
	void setSpecular	(ColorValue specular)	{ m_specular = specular ; }
	void setAmbient		(ColorValue ambient)	{ m_ambient = ambient ; }
	void setPosition	(Vector3f position)		{ m_position = position ; }
	void setDirection	(Vector3f direction)	{ m_direction = direction ; }
	void setTypeLight	(TypeLight typeL)		{ m_typeLight = typeL;	}

	ColorValue getDiffuse  ()   { return m_diffuse ; }
	ColorValue getSpecular ()   { return m_specular ; }
	ColorValue getAmbient  ()   { return m_ambient ; }
	Vector3f   getPosition ()   { return m_position ; }
	Vector3f   getDirection()   { return m_direction ; }
	TypeLight  getTypeLight()   { return m_typeLight ; }

protected :
	ColorValue m_diffuse ;
	ColorValue m_specular ;
	ColorValue m_ambient ;
	
	Vector3f m_position ;
	Vector3f m_direction ;

	TypeLight m_typeLight;
	Shader* shader;
};


//******************************************************************
#endif		// _Light_H
