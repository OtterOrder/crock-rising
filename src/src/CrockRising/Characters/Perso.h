#ifndef		_Perso_H_
#define		_Perso_H_

#include		<vector>
#include		<map>
#include		<list>
#include		<string>
#include		<Objects/SceneObjectAnimated.h>
#include		<Core/Types/Vector.h>
#include		<Physics/Physicalizer.h>
#include		<Resources/Material.h>
#include		<Core/Inputs/InputManager.h>
#include		<Sound/SoundObject.h>
#include		<NxPhysics.h>


enum SolidType
{
	WEAPON,
	NOT_WEAPON
};

struct AnimParam 
{
	AnimParam(){}
	AnimParam(	std::string _anim_name,	float _fps, bool _loop)
		: anim_name(_anim_name), fps(_fps), loop(_loop) {}
	std::string anim_name;
	float fps;
	bool loop;
};

class Perso
{
public:	
	Perso(Vector3f pos = Vector3f( 0.f, 0.f, 0.f ));
	~Perso();

	enum State 
	{
		STATIC,	
		RUN,
		ATTACK,
		DIE,
		HIT
	};

	typedef std::map<Perso::State, std::vector<AnimParam>> TAnimMap;
	typedef std::map<Perso::State, std::vector<SoundObject*>> TSoundMap;

	//Fonctions abstraites nécessaires à chaque dérivée
	virtual void		    changeState( State newState );
	virtual void			Init()=0;
	virtual void			Hit()=0;
	virtual void			Die()=0;
	virtual void			InitSound()=0;
	virtual void			InitAnim()=0;
	virtual void			DestroyPerso();
	virtual void			decLife ( const int decreaseLife );
	virtual void			update();	
	
	//Accesseurs
	SceneObjectAnimated* 	getSceneObjectAnimated () const { return m_pAnimated; }
	SceneObject*			getArme(){ return m_pArme; }
	State					getCurrentState(){ return m_currentState; }	
	int						getLife() { return m_iLife; }

	//Pioches aléatoirement dans la banque de son en fonction de l'état.
	void					PlayPersoSound( State state );
	void					ReleaseSounds();

	
protected:
	//Conteneur de toutes les anims du perso selon son état
	SceneObjectAnimated*	m_pAnimated;
	//Arme du perso (visuelle)
	SceneObject*			m_pArme;

	int					    m_iLife;
	State					m_currentState;

	//InputManager*			m_pInputManager;
	Vector3f				m_Position;
	Vector3f				m_Translate;
	//ID du bone de la main, permet de placer l'arme à l'endroit adéquat.
	int						m_idBone;

	void					AddSound(State state, const std::string soundName );
	void					AddAnim(State state, const std::string animName, float fps, bool loop );

	//Initialise l'acteur de l'arme, simple factorisation de code.
	void					InitWeapon();

private:
	//Banque d'anim du personnage
	TAnimMap				m_AnimMap;
	//Banque de son du personnage
	TSoundMap				m_SoundMap;

	void	Animate( State state );
	bool	isPlayingSound(); //Test si l'un des sons de l'état courant est en train d'être joué
	bool	isPlayingAnim();  //Test si l'une des anims de l'état courant est en train d'être joué
};


class ContactReportCR : public ContactReport
{
public:
	virtual void onContactNotify(NxContactPair& pair, NxU32 events);

};

#endif
