#pragma once

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "D3DUtil.h"
#include "D3DEnumeration.h"  
#include "D3DSettings.h"  
#include "d3dfont.h"

//===========================================================================//
// Codes d'erreurs                                                           //
//===========================================================================//
enum APPMSGTYPE { MSG_NONE, MSGERR_APPMUSTEXIT, MSGWARN_SWITCHEDTOREF };

#define D3DAPPERR_NODIRECT3D          0x82000001
#define D3DAPPERR_NOWINDOW            0x82000002
#define D3DAPPERR_NOCOMPATIBLEDEVICES 0x82000003
#define D3DAPPERR_NOWINDOWABLEDEVICES 0x82000004
#define D3DAPPERR_NOHARDWAREDEVICE    0x82000005
#define D3DAPPERR_HALNOTCOMPATIBLE    0x82000006
#define D3DAPPERR_NOWINDOWEDHAL       0x82000007
#define D3DAPPERR_NODESKTOPHAL        0x82000008
#define D3DAPPERR_NOHALTHISMODE       0x82000009
#define D3DAPPERR_NONZEROREFCOUNT     0x8200000a
#define D3DAPPERR_MEDIANOTFOUND       0x8200000b
#define D3DAPPERR_RESETFAILED         0x8200000c
#define D3DAPPERR_NULLREFDEVICE       0x8200000d

//===========================================================================//
// Classe bas-niveau D3D Application                                         //
//===========================================================================//
class CD3DApplication
{
protected:
    CD3DEnumeration   m_d3dEnumeration;
    CD3DSettings      m_d3dSettings;

	//===========================================================================//
    // Variables internes pour les différents états								 //
	//===========================================================================//
    bool              m_bWindowed;
    bool              m_bActive;
    bool              m_bDeviceLost;
    bool              m_bMinimized;
    bool              m_bMaximized;
    bool              m_bIgnoreSizeChange;
    bool              m_bDeviceObjectsInited;
    bool              m_bDeviceObjectsRestored;

	//===========================================================================//
    // Variables internes pour le timing									     //
	//===========================================================================//
    bool              m_bFrameMoving;
    bool              m_bSingleStep;

	//===========================================================================//
    // Objets principaux pour créer et rendre la scène 3D                        //
	//===========================================================================//
    D3DPRESENT_PARAMETERS	m_d3dpp;			 // Paramètres pour la création/reset du device
    HWND              		m_hWnd;              // Main app Windows
    HWND              		m_hWndFocus;         // Focus Windows
    HMENU             		m_hMenu;             // Menu Windows
    LPDIRECT3D9       		m_pD3D;              // Object principal D3D

protected:
    DWORD             		m_dwCreateFlags;     // Indique si software ou hardware vertex processing
    DWORD             		m_dwWindowStyle;     // Sauvegarde du style de la fenêtre
    RECT              		m_rcWindowBounds;    // Sauvegarde de la zone de la fenêtre
    RECT              		m_rcWindowClient;    // Sauvegarde de la taille de la fenêtre

public:
    LPDIRECT3DDEVICE9 		m_pd3dDevice;        // Pointeur vers le device de rendu
    D3DCAPS9          		m_d3dCaps;           // Capabilities du device
    D3DSURFACE_DESC   		m_d3dsdBackBuffer;   // Description de la surface du backbuffer
	CFirstPersonCamera 	    m_Camera;


    //===========================================================================//
    // Variables temporelles (public car utile partout)                          //
	//===========================================================================//
    FLOAT             m_fTime;             		// Le temps en seconde
    FLOAT             m_fElapsedTime;      		// Temps écoulé depuis la frame précédente
    FLOAT             m_fFPS;              		// Frame rate instantané
    TCHAR             m_strDeviceStats[90];		// String pour stocker les stats du device
    TCHAR             m_strFrameStats[90]; 		// String pour stocker les stats des frames
	CD3DFont*         m_pStatsFont;        		// Font pour les framestats

protected:
    //===========================================================================//
    // Variables propre à l'application                                          //
	//===========================================================================//
    TCHAR*            m_strWindowTitle;    			// Titre de l'application Windows
    DWORD             m_dwCreationWidth;   			// Hauteur pour créer la fenêtre
    DWORD             m_dwCreationHeight;  			// Largeur pour créer la fenêtre
    bool              m_bShowCursorWhenFullscreen;  // Pour savoir si l'on montre le curseur en fullscreen
    bool              m_bClipCursorWhenFullscreen;  // Pour savoir si l'on limite la position du curseur en fullscreen
    bool              m_bStartFullscreen;			// Pour savoir si on commence l'application en fullscreen

	//===========================================================================//
    // Callback à dériver                                                        //
	//===========================================================================//
    virtual HRESULT ConfirmDevice(D3DCAPS9*,DWORD,D3DFORMAT)   { return S_OK; }
    virtual HRESULT BeforeCreateDevice()                       { return S_OK; }
    virtual HRESULT OnCreateDevice()                           { return S_OK; }
    virtual HRESULT OnResetDevice()                            { return S_OK; }
    virtual HRESULT FrameMove(float fElapsedTime)              { return S_OK; }
    virtual HRESULT Render()                                   { return S_OK; }
    virtual HRESULT OnLostDevice()                             { return S_OK; }
    virtual HRESULT OnDestroyDevice()                          { return S_OK; }
    virtual HRESULT AfterDestroyDevice()                       { return S_OK; }

	//===========================================================================//
    // Fonction de gestion d'erreur												 //
	//===========================================================================//
    HRESULT DisplayErrorMsg( HRESULT hr, DWORD dwType );

	//===========================================================================//
    // Fonctions internes pour gérer et rendre la scene 3D                       //
	//===========================================================================//
    static bool 	ConfirmDeviceHelper (D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, D3DFORMAT backBufferFormat);
    void    		BuildPresentParamsFromSettings ();
    bool    		FindBestWindowedMode (bool bRequireHAL, bool bRequireREF);
    bool    		FindBestFullscreenMode ( bool bRequireHAL, bool bRequireREF);
    HRESULT 		ChooseInitialD3DSettings ();
    HRESULT 		Initialize3DEnvironment ();
    HRESULT 		HandlePossibleSizeChange ();
    HRESULT 		Reset3DEnvironment ();
    HRESULT 		ToggleFullscreen ();
    HRESULT 		ForceWindowed ();
    HRESULT 		UserSelectNewDevice ();
    void    		Cleanup3DEnvironment ();
    HRESULT 		Render3DEnvironment ();
    virtual 		HRESULT AdjustWindowForChange ();
    virtual void	UpdateStats ();

public:
	//===========================================================================//
    // Fonctions pour créer, lancer, et mettre en pause l'application
	//===========================================================================//
    virtual HRESULT 	Create( HINSTANCE hInstance, WNDCLASS wndClass );
    virtual void    	Run();
	virtual void		Close();
    virtual LRESULT 	EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );
    virtual void    	Pause( bool bPause );
    virtual         	~CD3DApplication() { }

    CD3DApplication(); // Constructeur par défaut

};



