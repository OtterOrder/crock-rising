#ifndef		_Object_H
#define		_Object_H

#include "Core/Types/Vector.h"

class Object
{
	public:
		// Constructeur
		Object( void );
		Object( float initPosX, float initPosY, float initPosZ );
		Object( Vector3f pos );

		Vector3f GetPosition();
		void	 SetPosition( Vector3f position );

	protected:
		Vector3f m_Position;
		D3DXMATRIX m_TransformMatrix;
};

#endif		// _Object_H