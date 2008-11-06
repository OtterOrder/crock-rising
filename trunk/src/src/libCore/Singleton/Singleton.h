#ifndef		_Singleton_H
#define		_Singleton_H

//******************************************************************

// Note: pour les classes template, on doit obligatoirement tout
// définir dans le .h (c'est crade, mais c'est imposé par le c++).

template< typename T >
class Singleton
{
	public:

		// =========================================================
		// Méthodes publiques
		
		static T* GetInstance( void )		// Donne l'instance du singleton
		{
			if( m_Instance == NULL )
				m_Instance = new T;

			return m_Instance;
		}
		
		static void Destroy( void )			// Détruit l'instance
		{
			if( m_Instance != NULL )
			{
				delete m_Instance;
				m_Instance = NULL;
			}
		}


	protected:

		// =========================================================
		// Méthodes protégées
		
		Singleton		( void ){}			// Constructeur
		~Singleton		( void ){}			// Destructeur


	private:

		// =========================================================
		// Données privées

		static T	*m_Instance;		// Instance du singleton

};

//******************************************************************

template< typename T >
T* Singleton< T >::m_Instance = NULL;

//******************************************************************
#endif		// _Singleton_H
