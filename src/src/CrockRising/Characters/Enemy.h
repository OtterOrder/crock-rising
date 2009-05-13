#ifndef		_Enemy_H_
#define		_Enemy_H_

//******************************************************************

#include		<Objects/SceneObjectAnimated.h>

//******************************************************************

class Enemy 
{
public:
	Enemy();
	~Enemy();

	int			 getLife () const { return m_iLife; }
	void	     setLife ( const int life ) { m_iLife += life; }   // La variable life représentant un bonus ou malus 

	void	     update( );

protected:
	int			 m_iLife;
	SceneObjectAnimated*  m_pAnimated;

};

//******************************************************************
#endif


