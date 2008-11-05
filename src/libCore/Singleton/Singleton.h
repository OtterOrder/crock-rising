#ifndef		_Singleton_H
#define		_Singleton_H

//******************************************************************

template<typename T> class Singleton
{
	public:

		// =========================================================
		// Méthodes publiques
		
		Singleton*	GetInstance		( void );

	protected:

		// =========================================================
		// Données privées
		
		static Singleton	*m_Instance;

		// =========================================================
		// Méthodes privées
		
		Singleton					( void );				// Constructeur
		~Singleton					( void );				// Destructeur
};

//******************************************************************
#endif		// _Singleton_H
