#ifndef		_Singleton_H
#define		_Singleton_H

//******************************************************************

// Note: pour les classes template, on doit obligatoirement tout
// définir dans le .h (c'est crade, mais c'est imposé par le c++).

template< typename T >
class Singleton
{
public:

	/***********************************************************
	 * Donne l'instance du singleton.
	 * @return	pointeur sur l'instance
	 **********************************************************/
	static T* GetInstance( void )
	{
		if( !m_Instance )
			m_Instance = new T;

		return m_Instance;
	}
	
	/***********************************************************
	 * Détruit l'instance.
	 **********************************************************/
	static void Destroy( void )
	{
		if( m_Instance )
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}


protected:

	Singleton			( void ){}		// Constructeur
	virtual ~Singleton	( void ){}		// Destructeur


private:

	static T	*m_Instance;			// Instance du singleton

};

//******************************************************************

template< typename T >
T* Singleton< T >::m_Instance = NULL;

//******************************************************************
#endif		// _Singleton_H
