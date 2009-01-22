#ifndef		_Light_H
#define		_Light_H

//******************************************************************

#include "Object.h"
#include "Core/Types/Vector.h"
#include "../Resources/Shader.h"
#include "../Resources/ResourceManager.h"

struct ColorValue 
{
	ColorValue	( float _r=0.f, float _g=0.f, float _b=0.f, float _a=1.f )	
				{	r = _r; g = _g; b = _b; alpha = _a;	}

	float r ;
	float g ;
	float b ;
	float alpha ;
};

//******************************************************************

class Light : public Object
{
public:

	Light();
	// Lumière type Point
	Light( ColorValue ambient, ColorValue diffuse, ColorValue specular, Vector3f position, float attenu );
	// Lumière type Spot
	Light( ColorValue ambient, ColorValue diffuse, ColorValue specular, Vector3f position,  Vector3f direction, float attenu, float angle );
	// Lumière type Directionnelle
	Light( ColorValue diffuse, ColorValue specular, Vector3f direction );

	void setDiffuse		(ColorValue diffuse)	{ m_diffuse = diffuse ;		}
	void setSpecular	(ColorValue specular)	{ m_specular = specular ;	}
	void setAmbient		(ColorValue ambient)	{ m_ambient = ambient ;		}
	void setPosition	(Vector3f position)		{ m_position = position ;	}
	void setDirection	(Vector3f direction)	{ m_direction = direction ; }
	void setAttenuation	(float atte)			{ m_attenuation = atte;		}
	void setAngle		(float angle)			{ m_angle = angle ;			}

	ColorValue	getDiffuse  ()		{ return m_diffuse ; }
	ColorValue	getSpecular ()		{ return m_specular ; }
	ColorValue	getAmbient  ()		{ return m_ambient ; }
	Vector3f	getPosition ()		{ return m_position ; }
	Vector3f	getDirection()		{ return m_direction ; }
	float		getAttenuation()	{ return m_attenuation ; }
	float		getAngle()			{ return m_angle ;	}

protected :
	ColorValue m_diffuse ;
	ColorValue m_specular ;
	ColorValue m_ambient ;
	
	Vector3f m_position ;
	Vector3f m_direction ;

	float m_attenuation;
	float m_angle;
};


//******************************************************************
#endif		// _Light_H
