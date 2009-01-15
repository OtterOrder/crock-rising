#include "Light.h"


Light::Light() 
{
	
	m_diffuse.r = 0.f ;
	m_diffuse.g = 0.f ;
	m_diffuse.b = 0.f ;
	m_diffuse.alpha = 0.f ;

	m_specular.r = 0.f ; 
	m_specular.g = 0.f ; 
	m_specular.b = 0.f ; 
	m_specular.alpha = 0.f ; 

	m_ambient.r = 0.f ; 
	m_ambient.g = 0.f ; 
	m_ambient.b = 0.f ; 
	m_ambient.alpha = 0.f ; 

	m_direction.x = 0.f ; 
	m_direction.y = 0.f ; 
	m_direction.z = 0.f ; 
	
	m_position.x = 0.f ; 
	m_position.y = 0.f ; 
	m_position.z = 0.f ; 

}

/* -------------------------------------------------------------------------------------------------- */

Light::Light(Vector3f position, Vector3f direction, ColorValue diffuse, ColorValue specular, ColorValue ambient) 
{
	m_position = position ; 
	m_direction = direction ;  
	m_diffuse = diffuse ; 
	m_specular = specular ; 
	m_ambient = ambient ; 
}

