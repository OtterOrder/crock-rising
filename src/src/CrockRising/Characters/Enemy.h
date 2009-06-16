#ifndef		_Enemy_H_
#define		_Enemy_H_

//******************************************************************

#include		<vector>
#include		<Objects/SceneObjectAnimated.h>
#include		<Core/Types/Vector.h>

//******************************************************************

class Enemy 
{
public:
	static std::vector<Enemy*>  listEnemy;

	Enemy();
	Enemy(Vector3f position);
	~Enemy();
	

	int			 getLife () const { return m_iLife; }
	void	     setLife ( const int life ) { m_iLife += life; }   // La variable life représentant un bonus ou malus 

	void					update( );
	SceneObjectAnimated*	getSceneObjectAnimated()	{	return m_pAnimated;	}

protected:
	int				      m_iLife;
	SceneObjectAnimated*  m_pAnimated;
};

//******************************************************************
#endif


