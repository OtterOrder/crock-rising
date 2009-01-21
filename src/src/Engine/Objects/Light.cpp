#include "Light.h"


Light::Light() 
{
	m_diffuse	= ColorValue();
	m_specular	= ColorValue();
	m_ambient	= ColorValue();

	m_direction	= Vector3f(0.f, 0.f, 0.f);
	m_position	= Vector3f(0.f, 0.f, 0.f); 
}

/* -------------------------------------------------------------------------------------------------- */

Light::Light(Vector3f position, Vector3f direction, ColorValue diffuse, ColorValue specular, ColorValue ambient, TypeLight type)
:	m_position(position), m_direction(direction), m_diffuse(diffuse), m_specular(specular), m_ambient(ambient), m_typeLight(type)
{
	switch (type)
	{
	case LT_AMBIANT:
		//shader = ResourceManager::GetInstance()->Load<Shader>("default.fx");
		break;
	case LT_DIRECTIONNEL:
		//shader = ResourceManager::GetInstance()->Load<Shader>("directionnel.fx");
		break;
	case LT_POINTLIGHT:
		//shader = ResourceManager::GetInstance()->Load<Shader>("pointlight.fx");
		break;
	case LT_SPOTLIGHT:
		//shader = ResourceManager::GetInstance()->Load<Shader>("spotlight.fx");
		break;
	}

}