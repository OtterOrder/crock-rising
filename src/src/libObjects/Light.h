#ifndef		_Light_H
#define		_Light_H

//******************************************************************

#include  "Object.h"
#include  <libcore/Types/Vector.h>


struct ColorValue {
	float r ; 
	float g ; 
	float b ; 
	float alpha ; 
};


//******************************************************************

class Light : public Object 
{

	public :


		Light();
		
		Light(Vector3f position , Vector3f direction , ColorValue diffuse , ColorValue specular , ColorValue ambient );

		void setDiffuse    (ColorValue diffuse)  { m_diffuse = diffuse ; }
		void setSpecular   (ColorValue specular) { m_specular = specular ; }
		void setAmbient	   (ColorValue ambient)  { m_ambient = ambient ; }
		void setPosition   (Vector3f position)   { m_position = position ; }
		void setDirection  (Vector3f direction)  { m_direction = direction ; }

		ColorValue getDiffuse  ()   { return m_diffuse ; }
		ColorValue getSpecular ()   { return m_specular ; }
		ColorValue getAmbient  ()   { return m_ambient ; }
		Vector3f getPosition   ()   { return m_position ; }
		Vector3f getDirection  ()   { return m_direction ; } 

		

	protected :
		
		ColorValue m_diffuse ; 
		ColorValue m_specular ; 
		ColorValue m_ambient ; 
		
		Vector3f m_position ; 
		Vector3f m_direction ; 

		
};


//******************************************************************
#endif		// _Light_H
