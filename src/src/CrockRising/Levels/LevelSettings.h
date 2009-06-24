#ifndef		_LEVEL_SETTINGS_H_
#define		_LEVEL_SETTINGS_H_

//******************************************************************

#include	<Game/Level.h>
#include	"../2D/ButtonText.h"

//******************************************************************

#define		LEVEL_settings			0xe545a0c5

//******************************************************************

class Camera;
class Sprite;
class SoundObject;

class LevelSettings : public Level
{
public:

	enum Link
	{
		RATIO = 0,
		RESOLUTION,

		NB_SETTINGS,
		
		APPLY = NB_SETTINGS,
		BACK,

		NB_LINKS
	};

	enum Ratio
	{
		R4_3 = 0,
		R16_10,

		NB_RATIOS
	};

	LevelSettings				( crc32 levelID );			// Constructeur
	virtual ~LevelSettings		( void );					// Destructeur

	virtual void	Init		( void );					// Initialisation
	virtual void	Update		( void );					// Update du niveau

	void IncRatio();
	void IncResolution();


protected:

	// Boutons de menu d'options
	struct SettingButton : public ButtonText
	{
		Link linkID;

		SettingButton( Link _linkID );
		void OnClic();
	};
	
	Camera			*m_Camera;
	Sprite			*m_Background;
	SoundObject		*m_Sound;

	SettingButton	*m_Menu[NB_LINKS];
	Text			*m_Values[NB_SETTINGS];

	int				m_CurrentRatio;
	int				m_CurrentResolution;

	void ReadFromBackup();
	void SaveInBackup() const;
	void RefreshValues();

};

//******************************************************************
#endif		//_LEVEL_SETTINGS_H_
