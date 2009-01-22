#include "Light.h"

/**
Light type Point
*/
Light::Light() 
{
	m_diffuse	= ColorValue();
	m_specular	= ColorValue();
	m_ambient	= ColorValue();

	m_direction	= Vector3f(0.f, 0.f, 0.f);
	m_position	= Vector3f(0.f, 0.f, 0.f); 
}

/* -------------------------------------------------------------------------------------------------- */

// Lumière type Point
Light::Light( ColorValue ambient, ColorValue diffuse, ColorValue specular, Vector3f position, float attenu )
:	m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_position(position), m_attenuation(attenu)
{
}

// Lumière type Spot
Light::Light( ColorValue ambient, ColorValue diffuse, ColorValue specular, Vector3f position,  Vector3f direction, float attenu, float angle )
:	m_ambient(ambient), m_diffuse(diffuse), m_specular(specular), m_position(position), m_direction(direction), m_attenuation(attenu), m_angle(angle)
{
}

// Lumière type Directionnelle
Light::Light( ColorValue diffuse, ColorValue specular, Vector3f direction )
:	m_diffuse(diffuse), m_specular(specular), m_direction(direction)
{

}