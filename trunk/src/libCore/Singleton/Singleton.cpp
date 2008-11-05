#include "Singleton.h"

//******************************************************************

template< typename T >
T* Singleton< T >::m_Instance = NULL;

//******************************************************************

template< typename T >
T* Singleton< T >::GetInstance( void )
{
	if( m_Instance == NULL )
		m_Instance = new T;

	return m_Instance;
}

template< typename T >
void Singleton< T >::Release( void )
{
	if( m_Instance != NULL )
	{
		delete m_Instance;
		m_Instance = NULL;
	}
}
