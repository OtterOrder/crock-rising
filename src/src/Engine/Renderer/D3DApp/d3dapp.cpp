#define STRICT

//===========================================================================//
// Include                                                                   //
//===========================================================================//
#define NOMINMAX
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>
#include <stdio.h>
#include <tchar.h>
#include <d3d9.h>
#include "DXUtil.h"
#include "D3DUtil.h"
#include "D3DEnumeration.h"
#include "D3DSettings.h"
#include "D3DApp.h"

//===========================================================================//
// Accès global à l'application                                              //
//===========================================================================//
static CD3DApplication* g_pD3DApp = NULL;


//===========================================================================//
// Constructeur par défaut                                                   //
//===========================================================================//
CD3DApplication::CD3DApplication()
{
    g_pD3DApp           = this;

    m_pD3D              = NULL;
    m_pd3dDevice        = NULL;
    m_hWnd              = NULL;
    m_hWndFocus         = NULL;
    m_hMenu             = NULL;
    m_bWindowed         = true;
    m_bActive           = false;
    m_bDeviceLost       = false;
    m_bMinimized        = false;
    m_bMaximized        = true;
    m_bIgnoreSizeChange = false;
    m_bDeviceObjectsInited = false;
    m_bDeviceObjectsRestored = false;
    m_dwCreateFlags     = 0;

    m_bFrameMoving      = true;
    m_bSingleStep       = false;
    m_fTime             = 0.0f;
    m_fElapsedTime      = 0.0f;
    m_fFPS              = 0.0f;
    m_strDeviceStats[0] = _T('\0');
    m_strFrameStats[0]  = _T('\0');
	m_pStatsFont		= NULL;

    m_strWindowTitle    = _T("Engine");
    m_dwCreationWidth   = 800;
    m_dwCreationHeight  = 600;
    m_bShowCursorWhenFullscreen = false;
    m_bStartFullscreen  = false;

    Pause( true ); // Pause jusqu'à ce que l'on soit prêt pour le rendu

#if defined(_DEBUG) || defined(DEBUG)
    m_bClipCursorWhenFullscreen = false;
#else
    m_bClipCursorWhenFullscreen = true;
#endif

}

//===========================================================================//
// Confirmation des paramètres de l'énumération                              //
//===========================================================================//
bool CD3DApplication::ConfirmDeviceHelper( D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, 
                         D3DFORMAT backBufferFormat )
{
    DWORD dwBehavior;

    if (vertexProcessingType == SOFTWARE_VP)
        dwBehavior = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    else if (vertexProcessingType == MIXED_VP)
        dwBehavior = D3DCREATE_MIXED_VERTEXPROCESSING;
    else if (vertexProcessingType == HARDWARE_VP)
        dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else if (vertexProcessingType == PURE_HARDWARE_VP)
        dwBehavior = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    else
        dwBehavior = 0; // TODO: exception
    
    return SUCCEEDED( g_pD3DApp->ConfirmDevice( pCaps, dwBehavior, backBufferFormat ) );
}




//===========================================================================//
// Création de la fenêtre, création du device                                //
//===========================================================================//
HRESULT CD3DApplication::Create( HINSTANCE hInstance, WNDCLASS wndClass)
{
    HRESULT hr;

    // Création de l'objet D3D
    m_pD3D = Direct3DCreate9( D3D_SDK_VERSION );
    if( m_pD3D == NULL )
        return DisplayErrorMsg( D3DAPPERR_NODIRECT3D, MSGERR_APPMUSTEXIT );

	//Création d'une liste d'adapters, modes, et devices. Le callback ComfirmDevice()
	//est utilisé pour comfirmer les devices qui ont les prérequis
    m_d3dEnumeration.SetD3D( m_pD3D );
    m_d3dEnumeration.ConfirmDeviceCallback = ConfirmDeviceHelper;
    if( FAILED( hr = m_d3dEnumeration.Enumerate() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

	// A part si un Hwnd de substitution à été crée, on crée la fenêtre
    if( m_hWnd == NULL)
    {
        if( !RegisterClass( &wndClass ) )
        {
            DWORD dwError = GetLastError();
            if( dwError != ERROR_CLASS_ALREADY_EXISTS )
                return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
        }

        
#ifdef _DEBUG 
        // On met le style initial de fenêtre
		m_dwWindowStyle = WS_OVERLAPPED| WS_CAPTION | WS_THICKFRAME|
                           WS_MINIMIZEBOX | WS_VISIBLE;
        // Taille initiale
        RECT rc;
        SetRect( &rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight );
        AdjustWindowRect( &rc, m_dwWindowStyle, TRUE );
#else
		// Set the window's initial style
		m_dwWindowStyle  =  WS_POPUP | WS_MAXIMIZE | WS_VISIBLE | WS_CLIPSIBLINGS;
        // Set the window's initial width
        RECT rc;
        SetRect( &rc, 0, 0, m_dwCreationWidth, m_dwCreationHeight );
#endif 

        // Création de la fenêtre de rendu
        m_hWnd = CreateWindow( _T(wndClass.lpszClassName), m_strWindowTitle, WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT,
                               (rc.right-rc.left), (rc.bottom-rc.top), 0L,
                               NULL , 
                               hInstance, 0L ) ; 
    } ; 

    // La fenêtre de focus peut être spécifiée à une fenêtre différente que celle du device,
	// sinon on utilise celle du device par défaut
    if( m_hWndFocus == NULL )
        m_hWndFocus = m_hWnd;

    // Sauvegarde des propriétés de la fenêtre
    m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );
    GetWindowRect( m_hWnd, &m_rcWindowBounds );
    GetClientRect( m_hWnd, &m_rcWindowClient );

    if( FAILED( hr = ChooseInitialD3DSettings() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // Initialisation du timer
    DXUtil_Timer( TIMER_START );

    // Initialisation des données diverse de l'application
    if( FAILED( hr = BeforeCreateDevice() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // Initialisation de l'environnement 3D
    if( FAILED( hr = Initialize3DEnvironment() ) )
    {
        SAFE_RELEASE( m_pD3D );
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // L'application est prête à être lancée
    Pause( false );

    return S_OK;
}

//===========================================================================//
// On met m_d3dSettings au meilleur mode fenêtré possible, en fonction       //
// des paramètres HAL et REF. Retourne faux si aucun mode ne peut être       //
// trouvé.                                                                   //
//===========================================================================//
bool CD3DApplication::FindBestWindowedMode( bool bRequireHAL, bool bRequireREF )
{
    // Obtention du mode d'affichage de l'adapter principal 
    D3DDISPLAYMODE primaryDesktopDisplayMode;
    m_pD3D->GetAdapterDisplayMode(0, &primaryDesktopDisplayMode);

    D3DAdapterInfo* pBestAdapterInfo = NULL;
    D3DDeviceInfo* pBestDeviceInfo = NULL;
    D3DDeviceCombo* pBestDeviceCombo = NULL;

    for( UINT iai = 0; iai < m_d3dEnumeration.m_pAdapterInfoList->Count(); iai++ )
    {
        D3DAdapterInfo* pAdapterInfo = (D3DAdapterInfo*)m_d3dEnumeration.m_pAdapterInfoList->GetPtr(iai);
        for( UINT idi = 0; idi < pAdapterInfo->pDeviceInfoList->Count(); idi++ )
        {
            D3DDeviceInfo* pDeviceInfo = (D3DDeviceInfo*)pAdapterInfo->pDeviceInfoList->GetPtr(idi);
            if (bRequireHAL && pDeviceInfo->DevType != D3DDEVTYPE_HAL)
                continue;
            if (bRequireREF && pDeviceInfo->DevType != D3DDEVTYPE_REF)
                continue;
            for( UINT idc = 0; idc < pDeviceInfo->pDeviceComboList->Count(); idc++ )
            {
                D3DDeviceCombo* pDeviceCombo = (D3DDeviceCombo*)pDeviceInfo->pDeviceComboList->GetPtr(idc);
                bool bAdapterMatchesBB = (pDeviceCombo->BackBufferFormat == pDeviceCombo->AdapterFormat);
                if (!pDeviceCombo->IsWindowed)
                    continue;
                if (pDeviceCombo->AdapterFormat != primaryDesktopDisplayMode.Format)
                    continue;
                // Si on a pas encore trouvé de DeviceCombo compatible, ou si celui est meilleur,
                // on sauvegarde
                if( pBestDeviceCombo == NULL || 
                    pBestDeviceCombo->DevType != D3DDEVTYPE_HAL && pDeviceCombo->DevType == D3DDEVTYPE_HAL ||
                    pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB )
                {
                    pBestAdapterInfo = pAdapterInfo;
                    pBestDeviceInfo = pDeviceInfo;
                    pBestDeviceCombo = pDeviceCombo;
                    if( pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesBB )
                    {
                        // Ce mode fenêtré à l'air pas mal : on le prend
                        goto EndWindowedDeviceComboSearch;
                    }
                    // Ou sinon on continue de chercher un autre mode
                }
            }
        }
    }
EndWindowedDeviceComboSearch:
    if (pBestDeviceCombo == NULL )
        return false;

    m_d3dSettings.pWindowed_AdapterInfo = pBestAdapterInfo;
    m_d3dSettings.pWindowed_DeviceInfo = pBestDeviceInfo;
    m_d3dSettings.pWindowed_DeviceCombo = pBestDeviceCombo;
    m_d3dSettings.IsWindowed = true;
    m_d3dSettings.Windowed_DisplayMode = primaryDesktopDisplayMode;
    m_d3dSettings.Windowed_Width = m_rcWindowClient.right - m_rcWindowClient.left;
    m_d3dSettings.Windowed_Height = m_rcWindowClient.bottom - m_rcWindowClient.top;
    if (m_d3dEnumeration.AppUsesDepthBuffer)
        m_d3dSettings.Windowed_DepthStencilBufferFormat = *(D3DFORMAT*)pBestDeviceCombo->pDepthStencilFormatList->GetPtr(0);
    m_d3dSettings.Windowed_MultisampleType = *(D3DMULTISAMPLE_TYPE*)pBestDeviceCombo->pMultiSampleTypeList->GetPtr(0);
    m_d3dSettings.Windowed_MultisampleQuality = 0;
    m_d3dSettings.Windowed_VertexProcessingType = *(VertexProcessingType*)pBestDeviceCombo->pVertexProcessingTypeList->GetPtr(0);
    m_d3dSettings.Windowed_PresentInterval = *(UINT*)pBestDeviceCombo->pPresentIntervalList->GetPtr(0);
    return true;
}

//===========================================================================//
// On met m_d3dSettings au meilleur mode plein écran possible, en fonction   //
// des paramètres HAL et REF. Retourne faux si aucun mode ne peut être       //
// trouvé.                                                                   //
//===========================================================================//
bool CD3DApplication::FindBestFullscreenMode( bool bRequireHAL, bool bRequireREF )
{
	// Pour le fullscreen, on prend par défaut le premier HAL DeviceCombo qui correspond au bureau
    D3DDISPLAYMODE adapterDesktopDisplayMode;
    D3DDISPLAYMODE bestAdapterDesktopDisplayMode;
    D3DDISPLAYMODE bestDisplayMode;
    bestAdapterDesktopDisplayMode.Width = 0;
    bestAdapterDesktopDisplayMode.Height = 0;
    bestAdapterDesktopDisplayMode.Format = D3DFMT_UNKNOWN;
    bestAdapterDesktopDisplayMode.RefreshRate = 0;

    D3DAdapterInfo* pBestAdapterInfo = NULL;
    D3DDeviceInfo* pBestDeviceInfo = NULL;
    D3DDeviceCombo* pBestDeviceCombo = NULL;

    for( UINT iai = 0; iai < m_d3dEnumeration.m_pAdapterInfoList->Count(); iai++ )
    {
        D3DAdapterInfo* pAdapterInfo = (D3DAdapterInfo*)m_d3dEnumeration.m_pAdapterInfoList->GetPtr(iai);
        m_pD3D->GetAdapterDisplayMode( pAdapterInfo->AdapterOrdinal, &adapterDesktopDisplayMode );
        for( UINT idi = 0; idi < pAdapterInfo->pDeviceInfoList->Count(); idi++ )
        {
            D3DDeviceInfo* pDeviceInfo = (D3DDeviceInfo*)pAdapterInfo->pDeviceInfoList->GetPtr(idi);
            if (bRequireHAL && pDeviceInfo->DevType != D3DDEVTYPE_HAL)
                continue;
            if (bRequireREF && pDeviceInfo->DevType != D3DDEVTYPE_REF)
                continue;
            for( UINT idc = 0; idc < pDeviceInfo->pDeviceComboList->Count(); idc++ )
            {
                D3DDeviceCombo* pDeviceCombo = (D3DDeviceCombo*)pDeviceInfo->pDeviceComboList->GetPtr(idc);
                bool bAdapterMatchesBB = (pDeviceCombo->BackBufferFormat == pDeviceCombo->AdapterFormat);
                bool bAdapterMatchesDesktop = (pDeviceCombo->AdapterFormat == adapterDesktopDisplayMode.Format);
                if (pDeviceCombo->IsWindowed)
                    continue;
                if (pBestDeviceCombo == NULL ||
                    pBestDeviceCombo->DevType != D3DDEVTYPE_HAL && pDeviceInfo->DevType == D3DDEVTYPE_HAL ||
                    pDeviceCombo->DevType == D3DDEVTYPE_HAL && pBestDeviceCombo->AdapterFormat != adapterDesktopDisplayMode.Format && bAdapterMatchesDesktop ||
                    pDeviceCombo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB )
                {
                    bestAdapterDesktopDisplayMode = adapterDesktopDisplayMode;
                    pBestAdapterInfo = pAdapterInfo;
                    pBestDeviceInfo = pDeviceInfo;
                    pBestDeviceCombo = pDeviceCombo;
                    if (pDeviceInfo->DevType == D3DDEVTYPE_HAL && bAdapterMatchesDesktop && bAdapterMatchesBB)
                    {
                        goto EndFullscreenDeviceComboSearch;
                    }
                }
            }
        }
    }
EndFullscreenDeviceComboSearch:
    if (pBestDeviceCombo == NULL)
        return false;

    // On a besoin de trouver un mode d'affichage sur le meilleur adapter qui utilise pBestDeviceCombo->AdapterFormat
    bestDisplayMode.Width = 0;
    bestDisplayMode.Height = 0;
    bestDisplayMode.Format = D3DFMT_UNKNOWN;
    bestDisplayMode.RefreshRate = 0;
    for( UINT idm = 0; idm < pBestAdapterInfo->pDisplayModeList->Count(); idm++ )
    {
        D3DDISPLAYMODE* pdm = (D3DDISPLAYMODE*)pBestAdapterInfo->pDisplayModeList->GetPtr(idm);
        if( pdm->Format != pBestDeviceCombo->AdapterFormat )
            continue;
        if( pdm->Width == bestAdapterDesktopDisplayMode.Width &&
            pdm->Height == bestAdapterDesktopDisplayMode.Height && 
            pdm->RefreshRate == bestAdapterDesktopDisplayMode.RefreshRate )
        {
            // On a trouvé la meilleure combinaison donc on arrête
            bestDisplayMode = *pdm;
            break;
        }
        else if( pdm->Width == bestAdapterDesktopDisplayMode.Width &&
                 pdm->Height == bestAdapterDesktopDisplayMode.Height && 
                 pdm->RefreshRate > bestDisplayMode.RefreshRate )
        {
            // Le rafraîchissement de correspond pas, mais la hauteur/largeur correspond
            bestDisplayMode = *pdm;
        }
        else if( pdm->Width == bestAdapterDesktopDisplayMode.Width )
        {
            bestDisplayMode = *pdm;
        }
        else if( bestDisplayMode.Width == 0 )
        {
            bestDisplayMode = *pdm;
        }
    }

    m_d3dSettings.pFullscreen_AdapterInfo = pBestAdapterInfo;
    m_d3dSettings.pFullscreen_DeviceInfo = pBestDeviceInfo;
    m_d3dSettings.pFullscreen_DeviceCombo = pBestDeviceCombo;
    m_d3dSettings.IsWindowed = false;
    m_d3dSettings.Fullscreen_DisplayMode = bestDisplayMode;
    if (m_d3dEnumeration.AppUsesDepthBuffer)
        m_d3dSettings.Fullscreen_DepthStencilBufferFormat = *(D3DFORMAT*)pBestDeviceCombo->pDepthStencilFormatList->GetPtr(0);
    m_d3dSettings.Fullscreen_MultisampleType = *(D3DMULTISAMPLE_TYPE*)pBestDeviceCombo->pMultiSampleTypeList->GetPtr(0);
    m_d3dSettings.Fullscreen_MultisampleQuality = 0;
    m_d3dSettings.Fullscreen_VertexProcessingType = *(VertexProcessingType*)pBestDeviceCombo->pVertexProcessingTypeList->GetPtr(0);
    m_d3dSettings.Fullscreen_PresentInterval = D3DPRESENT_INTERVAL_DEFAULT;
    return true;
}

//===========================================================================//
// On choisit les paramètres initiaux								         //
//===========================================================================//
HRESULT CD3DApplication::ChooseInitialD3DSettings()
{
    bool bFoundFullscreen = FindBestFullscreenMode( false, false );
    bool bFoundWindowed = FindBestWindowedMode( false, false );

    if( m_bStartFullscreen && bFoundFullscreen )
        m_d3dSettings.IsWindowed = false;
    if( !bFoundWindowed && bFoundFullscreen )
        m_d3dSettings.IsWindowed = false;

    if( !bFoundFullscreen && !bFoundWindowed )
        return D3DAPPERR_NOCOMPATIBLEDEVICES;

    return S_OK;
}

//===========================================================================//
// Fonction d'écoute des évènements									         //
//===========================================================================//
LRESULT CD3DApplication::EventsCallback( HWND hWnd, UINT uMsg, WPARAM wParam,
                                  LPARAM lParam )
{
    switch( uMsg )
    {
        case WM_PAINT:
            // Ecoute des message paint quand l'application est en pause
            if( m_pd3dDevice && !m_bActive && m_bWindowed &&
                m_bDeviceObjectsInited && m_bDeviceObjectsRestored )
            {
                Render();
                m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
            }
            break;

        case WM_GETMINMAXINFO:
            ((MINMAXINFO*)lParam)->ptMinTrackSize.x = 100;
            ((MINMAXINFO*)lParam)->ptMinTrackSize.y = 100;
            break;

        case WM_ENTERSIZEMOVE:
            // On stoppe les frames quand l'application est en train d'être redimentionnée 
			Pause( true );
            break;

        case WM_SIZE:
            // Ecoute des changement possible, dus aux réductions et agrandissements
            if( m_bWindowed && m_hWnd != NULL )
                m_dwWindowStyle = GetWindowLong( m_hWnd, GWL_STYLE );

            if( SIZE_MINIMIZED == wParam )
            {
                if( m_bClipCursorWhenFullscreen && !m_bWindowed )
                    ClipCursor( NULL );
                Pause( true ); // Pause tant qu'on réduit
                m_bMinimized = true;
                m_bMaximized = false;
            }
            else if( SIZE_MAXIMIZED == wParam )
            {
                if( m_bMinimized )
                    Pause( false ); // Fin de la pause lorsqu'on est plus réduit
                m_bMinimized = false;
                m_bMaximized = true;
                HandlePossibleSizeChange();
            }
            else if( SIZE_RESTORED == wParam )
            {
                if( m_bMaximized )
                {
                    m_bMaximized = false;
                    HandlePossibleSizeChange();
                }
                else if( m_bMinimized)
                {
                    Pause( false ); // Fin de la pause lorsqu'on est plus réduit
                    m_bMinimized = false;
                    HandlePossibleSizeChange();
                }
                else
                {
					// Si on est ni réduit ni agrandi, la taille de la fenêtre
					// peut être changée par l'utilisateur. Dans ce cas,
					// on ne reset pas encre le device, on attend que l'utilisateur
					// arrête le redimensionnement (message WM_EXITSIZEMOVE)
                }
            }
            break;

        case WM_EXITSIZEMOVE:
            Pause( false );
            HandlePossibleSizeChange();
            break;

        case WM_SETCURSOR:
            // On suprime le cureur en plein écran
            if( m_bActive && !m_bWindowed )
            {
                SetCursor( NULL );
                if( m_bShowCursorWhenFullscreen )
                    m_pd3dDevice->ShowCursor( true );
                return true;
            }
            break;

         case WM_MOUSEMOVE:
            if( m_bActive && m_pd3dDevice != NULL )
            {
                POINT ptCursor;
                GetCursorPos( &ptCursor );
                if( !m_bWindowed )
                    ScreenToClient( m_hWnd, &ptCursor );
                m_pd3dDevice->SetCursorPosition( ptCursor.x, ptCursor.y, 0 );
            }
            break;

       case WM_ENTERMENULOOP:
            // Pause  de l'application quand les menus sont utilisés
            Pause(true);
            break;

        case WM_EXITMENULOOP:
            Pause(false);
            break;

        case WM_NCHITTEST:
            if( !m_bWindowed )
                return HTCLIENT;
            break;

        case WM_POWERBROADCAST:
            switch( wParam )
            {
                #ifndef PBT_APMQUERYSUSPEND
                    #define PBT_APMQUERYSUSPEND 0x0000
                #endif
                case PBT_APMQUERYSUSPEND:
                    return true;

                #ifndef PBT_APMRESUMESUSPEND
                    #define PBT_APMRESUMESUSPEND 0x0007
                #endif
                case PBT_APMRESUMESUSPEND:
                    return true;
            }
            break;

        case WM_SYSCOMMAND:
			// Prévient d'un déplacement/redimentionnement et d'une perte de l'écran en fullscreen
            switch( wParam )
            {
                case SC_MOVE:
                case SC_SIZE:
                case SC_MAXIMIZE:
                case SC_KEYMENU:
                case SC_MONITORPOWER:
                    if( false == m_bWindowed )
                        return 1;
                    break;
            }
            break;

		 case WM_SYSKEYDOWN:

            switch( wParam )
            {
                case VK_RETURN:
                {
                    // On met en plein écran lorsqu'on appuie sur Alt+Enter
                    DWORD dwMask = ( 1 << 29 );
                    if( ( lParam & dwMask ) != 0 ) // Alt is down also
                    {
                        // On passe en fullscreen
						Pause(true);
                        ToggleFullscreen();
						Pause(false);
                        return 0;
                    }
                }
            }
            break;

		case WM_KEYDOWN:
        {
            switch( wParam )
            {
                case VK_ESCAPE:
                {
                    //SendMessage( hWnd, WM_CLOSE, 0, 0 );
					Cleanup3DEnvironment();
					SAFE_RELEASE( m_pD3D );
					AfterDestroyDevice();
					DestroyWindow( hWnd );
					PostQuitMessage(0);
					m_hWnd = NULL;
                    break;
                }
                case VK_PAUSE:
                {
                    /*bool bTimePaused = DXUTIsTimePaused();
                    bTimePaused = !bTimePaused;
                    if( bTimePaused )
                        DXUTPause( true, false );
                    else
                        DXUTPause( false, false );
                    break;*/
                }
				case 'O':
				{
					if( m_bActive)
                    {
                        Pause(true);

                        if( FAILED( UserSelectNewDevice() ) )
                            return 0;

                        Pause(false);
                    }
					break;
				}
            }
            break;
        }

		case WM_CLOSE:
            Cleanup3DEnvironment();
            SAFE_RELEASE( m_pD3D );
            AfterDestroyDevice();
	        DestroyWindow( hWnd );
            PostQuitMessage(0);
            m_hWnd = NULL;
            break;

    }

	m_DevCamera.HandleMessages(hWnd, uMsg, wParam, lParam);

	return S_OK;
}

//===========================================================================//
// Fonction d'écoute des changements de la taille de la fenêtre              //
//===========================================================================//
HRESULT CD3DApplication::HandlePossibleSizeChange()
{
    HRESULT hr = S_OK;
    RECT rcClientOld;
    rcClientOld = m_rcWindowClient;

    if( m_bIgnoreSizeChange )
        return S_OK;

    // Mise à jour des propriétés de windows
    GetWindowRect( m_hWnd, &m_rcWindowBounds );
    GetClientRect( m_hWnd, &m_rcWindowClient );

    if( rcClientOld.right - rcClientOld.left !=
        m_rcWindowClient.right - m_rcWindowClient.left ||
        rcClientOld.bottom - rcClientOld.top !=
        m_rcWindowClient.bottom - m_rcWindowClient.top)
    {
        // Un nouveau redimensionnement de la fenêtre demande une nouvelle taille de
		// backbuffer donc la structure 3d doit être changée
        Pause( true );

        m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
        m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
    
        if( m_pd3dDevice != NULL )
        {
            // Reset de l'environnement 3D
            if( FAILED( hr = Reset3DEnvironment() ) )
            {
                if( hr != D3DERR_OUTOFVIDEOMEMORY )
                    hr = D3DAPPERR_RESETFAILED;
                DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
            }
        }
        Pause( false );
    }
    return hr;
}

//===========================================================================//
// Initialisation de l'environnement 3D								         //
//===========================================================================//
HRESULT CD3DApplication::Initialize3DEnvironment()
{
    HRESULT hr;

    D3DAdapterInfo* pAdapterInfo = m_d3dSettings.PAdapterInfo();
    D3DDeviceInfo* pDeviceInfo = m_d3dSettings.PDeviceInfo();

    m_bWindowed = m_d3dSettings.IsWindowed;

    // Prépare windows pour un changement possible fenêtré/plein écran
    AdjustWindowForChange();

    // Création des paramètres de présentation
    BuildPresentParamsFromSettings();

    if( pDeviceInfo->Caps.PrimitiveMiscCaps & D3DPMISCCAPS_NULLREFERENCE )
    {
        // Averti l'utilisateur d'une reference nulle
        DisplayErrorMsg( D3DAPPERR_NULLREFDEVICE, 0 );
    }

    DWORD behaviorFlags;
    if (m_d3dSettings.GetVertexProcessingType() == SOFTWARE_VP)
        behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
    else if (m_d3dSettings.GetVertexProcessingType() == MIXED_VP)
        behaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
    else if (m_d3dSettings.GetVertexProcessingType() == HARDWARE_VP)
        behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;
    else if (m_d3dSettings.GetVertexProcessingType() == PURE_HARDWARE_VP)
        behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_PUREDEVICE;
    else
        behaviorFlags = 0; // TODO: throw exception

    // Création du device the device
    hr = m_pD3D->CreateDevice( m_d3dSettings.AdapterOrdinal(), pDeviceInfo->DevType,
                               m_hWndFocus, behaviorFlags, &m_d3dpp,
                               &m_pd3dDevice );

    if( SUCCEEDED(hr) )
    {
        // Quand on passe de fullscreen à fenetré, c'est important d'ajuster
		// la taille de la fenêtre avec de créer le device
        if( m_bWindowed )
        {
            SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                          m_rcWindowBounds.left, m_rcWindowBounds.top,
                          ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                          ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                          SWP_SHOWWINDOW );
        }

        // Stockage du device cap
        m_pd3dDevice->GetDeviceCaps( &m_d3dCaps );
        m_dwCreateFlags = behaviorFlags;

        // Stockage des informations du device
        if( pDeviceInfo->DevType == D3DDEVTYPE_REF )
            lstrcpy( m_strDeviceStats, TEXT("REF") );
        else if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
            lstrcpy( m_strDeviceStats, TEXT("HAL") );
        else if( pDeviceInfo->DevType == D3DDEVTYPE_SW )
            lstrcpy( m_strDeviceStats, TEXT("SW") );

        if( behaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING &&
            behaviorFlags & D3DCREATE_PUREDEVICE )
        {
            if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (pure hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated pure hw vp)") );
        }
        else if( behaviorFlags & D3DCREATE_HARDWARE_VERTEXPROCESSING )
        {
            if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (hw vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated hw vp)") );
        }
        else if( behaviorFlags & D3DCREATE_MIXED_VERTEXPROCESSING )
        {
            if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
                lstrcat( m_strDeviceStats, TEXT(" (mixed vp)") );
            else
                lstrcat( m_strDeviceStats, TEXT(" (simulated mixed vp)") );
        }
        else if( behaviorFlags & D3DCREATE_SOFTWARE_VERTEXPROCESSING )
        {
            lstrcat( m_strDeviceStats, TEXT(" (sw vp)") );
        }

        if( pDeviceInfo->DevType == D3DDEVTYPE_HAL )
        {
            lstrcat( m_strDeviceStats, TEXT(": ") );
            const int cchDesc = sizeof(pAdapterInfo->AdapterIdentifier.Description);
            TCHAR szDescription[cchDesc];
            DXUtil_ConvertAnsiStringToGenericCch( szDescription, 
                pAdapterInfo->AdapterIdentifier.Description, cchDesc );
            int maxAppend = sizeof(m_strDeviceStats) / sizeof(TCHAR) -
                lstrlen( m_strDeviceStats ) - 1;
            _tcsncat( m_strDeviceStats, szDescription, maxAppend );
        }

        // Stockage de la description de la surface du render targer 
        LPDIRECT3DSURFACE9 pBackBuffer = NULL;
        m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
        pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
        pBackBuffer->Release();

        // Création du curseur fullscreen
        if( m_bShowCursorWhenFullscreen && !m_bWindowed )
        {
            HCURSOR hCursor;
#ifdef _WIN64
            hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
            hCursor = (HCURSOR)ULongToHandle( GetClassLong( m_hWnd, GCL_HCURSOR ) );
#endif
            D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor, true );
            m_pd3dDevice->ShowCursor( true );
        }

        // On confine le curseur à la fenêtre plein écran
        if( m_bClipCursorWhenFullscreen )
        {
            if (!m_bWindowed )
            {
                RECT rcWindow;
                GetWindowRect( m_hWnd, &rcWindow );
                ClipCursor( &rcWindow );
            }
            else
            {
                ClipCursor( NULL );
            }
        }

        // Initialisation des objets dépendants du device
        hr = OnCreateDevice();
        if( FAILED(hr) )
        {
            OnDestroyDevice();
        }
        else
        {
            m_bDeviceObjectsInited = true;
            hr = OnResetDevice();
            if( FAILED(hr) )
            {
                OnLostDevice();
            }
            else
            {
                m_bDeviceObjectsRestored = true;
                return S_OK;
            }
        }

        // Nettoyage avant de recommencer
        Cleanup3DEnvironment();
    }

    // Si ça a échoué, on passe au reference rasterizer
    if( hr != D3DAPPERR_MEDIANOTFOUND && 
        hr != HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ) && 
        pDeviceInfo->DevType == D3DDEVTYPE_HAL )
    {
        if (FindBestWindowedMode(false, true))
        {
            m_bWindowed = true;
            AdjustWindowForChange();
            // Pour être sûr que la fenêtre n'est pas topmost
            SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                          m_rcWindowBounds.left, m_rcWindowBounds.top,
                          ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                          ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                          SWP_SHOWWINDOW );

            // On dit à l'utilisateur qu'on est passé au reference rasterizer
            DisplayErrorMsg( hr, MSGWARN_SWITCHEDTOREF );

            hr = Initialize3DEnvironment();
        }
    }
    return hr;
}

//===========================================================================//
// Création des paramètres de présentation								         //
//===========================================================================//
void CD3DApplication::BuildPresentParamsFromSettings()
{
    m_d3dpp.Windowed               = m_d3dSettings.IsWindowed;
    m_d3dpp.BackBufferCount        = 1;
    m_d3dpp.MultiSampleType        = m_d3dSettings.MultisampleType();
    m_d3dpp.MultiSampleQuality     = m_d3dSettings.MultisampleQuality();
    m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
    m_d3dpp.EnableAutoDepthStencil = m_d3dEnumeration.AppUsesDepthBuffer;
    m_d3dpp.hDeviceWindow          = m_hWnd;
    if( m_d3dEnumeration.AppUsesDepthBuffer )
    {
        m_d3dpp.Flags              = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
        m_d3dpp.AutoDepthStencilFormat = m_d3dSettings.DepthStencilBufferFormat();
    }
    else
    {
        m_d3dpp.Flags              = 0;
    }

    if( m_bWindowed )
    {
        m_d3dpp.BackBufferWidth  = m_rcWindowClient.right - m_rcWindowClient.left;
        m_d3dpp.BackBufferHeight = m_rcWindowClient.bottom - m_rcWindowClient.top;
        m_d3dpp.BackBufferFormat = m_d3dSettings.PDeviceCombo()->BackBufferFormat;
        m_d3dpp.FullScreen_RefreshRateInHz = 0;
        m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE ; // m_d3dSettings.PresentInterval();
    }
    else
    {
        m_d3dpp.BackBufferWidth  = m_d3dSettings.DisplayMode().Width;
        m_d3dpp.BackBufferHeight = m_d3dSettings.DisplayMode().Height;
        m_d3dpp.BackBufferFormat = m_d3dSettings.PDeviceCombo()->BackBufferFormat;
        m_d3dpp.FullScreen_RefreshRateInHz = m_d3dSettings.Fullscreen_DisplayMode.RefreshRate;
        m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE ; // m_d3dSettings.PresentInterval();
    }
}

//===========================================================================//
// Reset de l'environnement 3D      								         //
//===========================================================================//
HRESULT CD3DApplication::Reset3DEnvironment()
{
    HRESULT hr;

    // Supprime les objets de la mémoire
    if( m_bDeviceObjectsRestored )
    {
        m_bDeviceObjectsRestored = false;
        OnLostDevice();
    }
    // Reset du device
    if( FAILED( hr = m_pd3dDevice->Reset( &m_d3dpp ) ) )
        return hr;

    // Stockage render target surface desc
    LPDIRECT3DSURFACE9 pBackBuffer;
    m_pd3dDevice->GetBackBuffer( 0, 0, D3DBACKBUFFER_TYPE_MONO, &pBackBuffer );
    pBackBuffer->GetDesc( &m_d3dsdBackBuffer );
    pBackBuffer->Release();

    // création du curseur plein écran
    if( m_bShowCursorWhenFullscreen && !m_bWindowed )
    {
        HCURSOR hCursor;
#ifdef _WIN64
        hCursor = (HCURSOR)GetClassLongPtr( m_hWnd, GCLP_HCURSOR );
#else
        hCursor = (HCURSOR)ULongToHandle( GetClassLong( m_hWnd, GCL_HCURSOR ) );
#endif
        D3DUtil_SetDeviceCursor( m_pd3dDevice, hCursor, true );
        m_pd3dDevice->ShowCursor( true );
    }

    // On confine le curseur à la fenêtre plein écran
    if( m_bClipCursorWhenFullscreen )
    {
        if (!m_bWindowed )
        {
            RECT rcWindow;
            GetWindowRect( m_hWnd, &rcWindow );
            ClipCursor( &rcWindow );
        }
        else
        {
            ClipCursor( NULL );
        }
    }

    // Initialisation des objets dépendants du device
    hr = OnResetDevice();
    if( FAILED(hr) )
    {
        OnLostDevice();
        return hr;
    }
    m_bDeviceObjectsRestored = true;

    // Si l'application est en pause, on stoppe le rendu
    if( false == m_bFrameMoving )
    {
        m_bSingleStep = true;
        DXUtil_Timer( TIMER_START );
        DXUtil_Timer( TIMER_STOP );
    }

    return S_OK;
}

//===========================================================================//
// Fonction pour passer en plein écran 								         //
//===========================================================================//
HRESULT CD3DApplication::ToggleFullscreen()
{
    HRESULT hr;
    int AdapterOrdinalOld = m_d3dSettings.AdapterOrdinal();
    D3DDEVTYPE DevTypeOld = m_d3dSettings.DevType();

    Pause( true );
    m_bIgnoreSizeChange = true;

    // Etat fenêtré
    m_bWindowed = !m_bWindowed;
    m_d3dSettings.IsWindowed = m_bWindowed;

    // On se prépare à un changement fenêtré/plein écran
    AdjustWindowForChange();

    // Si l'AdapterOrdinal et le DevType sont les mêmes, on peut juste faire un reset
    // Si ils ont changés, on a besoin de reconstruire complètement le device
    if (m_d3dSettings.AdapterOrdinal() == AdapterOrdinalOld &&
        m_d3dSettings.DevType() == DevTypeOld)
    {
        // Reset du device 3D
        BuildPresentParamsFromSettings();
        hr = Reset3DEnvironment();
    }
    else
    {
        Cleanup3DEnvironment();
        hr = Initialize3DEnvironment();
    }
    if( FAILED( hr ) )
    {
        if( hr != D3DERR_OUTOFVIDEOMEMORY )
            hr = D3DAPPERR_RESETFAILED;
        m_bIgnoreSizeChange = false;
        if( !m_bWindowed )
        {
            // Restore le type de fenêtre au type fenêtré
            m_bWindowed = !m_bWindowed;
            m_d3dSettings.IsWindowed = m_bWindowed;
            AdjustWindowForChange();
            SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                        m_rcWindowBounds.left, m_rcWindowBounds.top,
                        ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                        ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                        SWP_SHOWWINDOW );
        }
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    m_bIgnoreSizeChange = false;

    if( m_bWindowed )
    {
        SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                      m_rcWindowBounds.left, m_rcWindowBounds.top,
                      ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                      ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                      SWP_SHOWWINDOW );
    }

    Pause( false );
    return S_OK;
}

//===========================================================================//
// Fonction pour passer au mode fenêtré								         //
//===========================================================================//
HRESULT CD3DApplication::ForceWindowed()
{
    HRESULT hr;

    if( m_bWindowed )
        return S_OK;

    if( !FindBestWindowedMode(false, false) )
    {
        return E_FAIL;
    }
    m_bWindowed = true;

    // On détruit les objets du device 3D et on réinitialise

    Pause( true );

    // On suppprime tous les objets de la scène qui seront recrées pour le nouveau device
    Cleanup3DEnvironment();

    // Création du nouveau device
    if( FAILED(hr = Initialize3DEnvironment() ) )
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );

    Pause( false );
    return S_OK;
}

//============================================================================//
// Prépare la fenêtre pour un changement possible entre fenêtré et plein écran//
//============================================================================//
HRESULT CD3DApplication::AdjustWindowForChange()
{
    if( m_bWindowed )
    {
        // On met style mode fenêtré
        SetWindowLong( m_hWnd, GWL_STYLE, m_dwWindowStyle );
        if( m_hMenu != NULL )
        {
            SetMenu( m_hWnd, m_hMenu );
            m_hMenu = NULL;
        }
    }
    else
    {
        // On met le style mode plein écran
        SetWindowLong( m_hWnd, GWL_STYLE, WS_POPUP|WS_SYSMENU|WS_VISIBLE );
        if( m_hMenu == NULL )
        {
            m_hMenu = GetMenu( m_hWnd );
            SetMenu( m_hWnd, NULL );
        }
    }
    return S_OK;
}

//============================================================================//
// Création d'un nouveau device par l'uitlisateur                             //
//============================================================================//
HRESULT CD3DApplication::UserSelectNewDevice()
{
    HRESULT hr;

    // Can't display dialogs in fullscreen mode
    if( m_bWindowed == false )
    {
        if( FAILED( ToggleFullscreen() ) )
        {
            DisplayErrorMsg( D3DAPPERR_RESETFAILED, MSGERR_APPMUSTEXIT );
            return E_FAIL;
        }
    }

    CD3DSettingsDialog settingsDialog( &m_d3dEnumeration, &m_d3dSettings);
    if( settingsDialog.ShowDialog( m_hWnd ) != IDOK )
        return S_OK;
	
    settingsDialog.GetFinalSettings( &m_d3dSettings );

    m_bWindowed = m_d3dSettings.IsWindowed;

    // On suppprime tous les objets de la scène qui seront recrées pour le nouveau device
    Cleanup3DEnvironment();

    // On informe la classe d'affichage du changement. Cela créera de nouvelles surfaces, device etc...
    if( FAILED( hr = Initialize3DEnvironment() ) )
    {
        if( hr != D3DERR_OUTOFVIDEOMEMORY )
            hr = D3DAPPERR_RESETFAILED;
        if( !m_bWindowed )
        {
            // Restauration du type de fenêtre au mode fenêtré
            m_bWindowed = !m_bWindowed;
            m_d3dSettings.IsWindowed = m_bWindowed;
            AdjustWindowForChange();
            SetWindowPos( m_hWnd, HWND_NOTOPMOST,
                        m_rcWindowBounds.left, m_rcWindowBounds.top,
                        ( m_rcWindowBounds.right - m_rcWindowBounds.left ),
                        ( m_rcWindowBounds.bottom - m_rcWindowBounds.top ),
                        SWP_SHOWWINDOW );
        }
        return DisplayErrorMsg( hr, MSGERR_APPMUSTEXIT );
    }

    // Si l'application est en pause, on stoppe le rendu
    if( false == m_bFrameMoving )
    {
        m_bSingleStep = true;
        DXUtil_Timer( TIMER_START );
        DXUtil_Timer( TIMER_STOP );
    }
    return S_OK;
}

//============================================================================//
// Fonction qui lance le rendu                                                //
//============================================================================//
void CD3DApplication::Run()
{

	if( m_bDeviceLost )
	{
		// On laisse du temps au CPU pour les autres processus
		Sleep ( 100 ); // 100 millisecondes
	}
	// On lance le rendu des frames
	if( m_bActive )
	{
		if( FAILED( Render3DEnvironment() ) )
			SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
	}
        
 
}

//============================================================================//
// Fonction qui stoppe le moteur                                              //
//============================================================================//
void CD3DApplication::Close()
{
	Cleanup3DEnvironment();
    SAFE_RELEASE( m_pD3D );
    AfterDestroyDevice();
    DestroyWindow( m_hWnd );
    m_hWnd = NULL;

}

//============================================================================//
// Fonction de rendu de l'environnement 3D                                    //
//============================================================================//
HRESULT CD3DApplication::Render3DEnvironment()
{
    HRESULT hr;

    if( m_bDeviceLost )
    {
        // On teste le cooperative level pour voir si on peut faire le rendu
        if( FAILED( hr = m_pd3dDevice->TestCooperativeLevel() ) )
        {
            // Si le device est perdu, on ne fait pas le rendu tant qu'on l'a pas retrouvé
            if( D3DERR_DEVICELOST == hr )
                return S_OK;

            // On regarde si le device à besoin d'être reset
            if( D3DERR_DEVICENOTRESET == hr )
            {
                // Si on est fenêtré, on lit le format du bureau  et on utilise le même pour le back buffer
                if( m_bWindowed )
                {
                    D3DAdapterInfo* pAdapterInfo = m_d3dSettings.PAdapterInfo();
                    m_pD3D->GetAdapterDisplayMode( pAdapterInfo->AdapterOrdinal, &m_d3dSettings.Windowed_DisplayMode );
                    m_d3dpp.BackBufferFormat = m_d3dSettings.Windowed_DisplayMode.Format;
                }

                if( FAILED( hr = Reset3DEnvironment() ) )
                    return hr;
            }
            return hr;
        }
        m_bDeviceLost = false;
    }

    // Temps de l'application en secondes. On ne fait pas le rendu si auucn temps n'est écoulé
    FLOAT fAppTime        = DXUtil_Timer( TIMER_GETAPPTIME );
    FLOAT fElapsedAppTime = DXUtil_Timer( TIMER_GETELAPSEDTIME );

	// Conflit avec le système de pause
	// 
    // if( ( 0.0f == fElapsedAppTime ) && m_bFrameMoving )
    //    return S_OK;

    // FrameMove (animation) de la scene
    if( m_bFrameMoving || m_bSingleStep )
    {
        // On stocke le temps pour l'application
        m_fTime        = fAppTime;
        m_fElapsedTime = fElapsedAppTime;

        // On fait l'animation
        if( FAILED( hr = FrameMove(m_fElapsedTime) ) )
            return hr;
		m_DevCamera.FrameMove(m_fElapsedTime);

        m_bSingleStep = false;
    }

    // Rendu de la scène
    if( FAILED( hr = Render() ) )
        return hr;

    UpdateStats();

    // On présente la surface à l'écran
    hr = m_pd3dDevice->Present( NULL, NULL, NULL, NULL );
    if( D3DERR_DEVICELOST == hr )
        m_bDeviceLost = true;

    return S_OK;
}

//============================================================================//
// Mise à jour statistiques                                                   //
//============================================================================//
void CD3DApplication::UpdateStats()
{
    // On laisse une trace du nombre de frames
    static FLOAT fLastTime = 0.0f;
    static DWORD dwFrames  = 0;
    FLOAT fTime = DXUtil_Timer( TIMER_GETABSOLUTETIME );
    ++dwFrames;

    // On met à jour le stats une fois par seconde
    if( fTime - fLastTime > 1.0f )
    {
        m_fFPS    = dwFrames / (fTime - fLastTime);
        fLastTime = fTime;
        dwFrames  = 0;

        TCHAR strFmt[100];
        D3DFORMAT fmtAdapter = m_d3dSettings.DisplayMode().Format;
        if( fmtAdapter == m_d3dsdBackBuffer.Format )
        {
            lstrcpyn( strFmt, D3DUtil_D3DFormatToString( fmtAdapter, false ), 100 );
        }
        else
        {
            _sntprintf( strFmt, 100, TEXT("backbuf %s, adapter %s"), 
                D3DUtil_D3DFormatToString( m_d3dsdBackBuffer.Format, false ), 
                D3DUtil_D3DFormatToString( fmtAdapter, false ) );
        }
        strFmt[99] = TEXT('\0');

        TCHAR strDepthFmt[100];
        if( m_d3dEnumeration.AppUsesDepthBuffer )
        {
            _sntprintf( strDepthFmt, 100, TEXT(" (%s)"), 
                D3DUtil_D3DFormatToString( m_d3dSettings.DepthStencilBufferFormat(), false ) );
            strDepthFmt[99] = TEXT('\0');
        }
        else
        {
            // Pas de z-buffer
            strDepthFmt[0] = TEXT('\0');
        }

        TCHAR* pstrMultiSample;
        switch( m_d3dSettings.MultisampleType() )
        {
        case D3DMULTISAMPLE_NONMASKABLE:  pstrMultiSample = TEXT(" (Nonmaskable Multisample)"); break;
        case D3DMULTISAMPLE_2_SAMPLES:  pstrMultiSample = TEXT(" (2x Multisample)"); break;
        case D3DMULTISAMPLE_3_SAMPLES:  pstrMultiSample = TEXT(" (3x Multisample)"); break;
        case D3DMULTISAMPLE_4_SAMPLES:  pstrMultiSample = TEXT(" (4x Multisample)"); break;
        case D3DMULTISAMPLE_5_SAMPLES:  pstrMultiSample = TEXT(" (5x Multisample)"); break;
        case D3DMULTISAMPLE_6_SAMPLES:  pstrMultiSample = TEXT(" (6x Multisample)"); break;
        case D3DMULTISAMPLE_7_SAMPLES:  pstrMultiSample = TEXT(" (7x Multisample)"); break;
        case D3DMULTISAMPLE_8_SAMPLES:  pstrMultiSample = TEXT(" (8x Multisample)"); break;
        case D3DMULTISAMPLE_9_SAMPLES:  pstrMultiSample = TEXT(" (9x Multisample)"); break;
        case D3DMULTISAMPLE_10_SAMPLES: pstrMultiSample = TEXT(" (10x Multisample)"); break;
        case D3DMULTISAMPLE_11_SAMPLES: pstrMultiSample = TEXT(" (11x Multisample)"); break;
        case D3DMULTISAMPLE_12_SAMPLES: pstrMultiSample = TEXT(" (12x Multisample)"); break;
        case D3DMULTISAMPLE_13_SAMPLES: pstrMultiSample = TEXT(" (13x Multisample)"); break;
        case D3DMULTISAMPLE_14_SAMPLES: pstrMultiSample = TEXT(" (14x Multisample)"); break;
        case D3DMULTISAMPLE_15_SAMPLES: pstrMultiSample = TEXT(" (15x Multisample)"); break;
        case D3DMULTISAMPLE_16_SAMPLES: pstrMultiSample = TEXT(" (16x Multisample)"); break;
        default:                        pstrMultiSample = TEXT(""); break;
        }

        const int cchMaxFrameStats = sizeof(m_strFrameStats) / sizeof(TCHAR);
        _sntprintf( m_strFrameStats, cchMaxFrameStats, _T("%.02f fps (%dx%d), %s%s%s"), m_fFPS,
                    m_d3dsdBackBuffer.Width, m_d3dsdBackBuffer.Height,
                    strFmt, strDepthFmt, pstrMultiSample );
        m_strFrameStats[cchMaxFrameStats - 1] = TEXT('\0');
    }
}

//============================================================================//
// Fonction de pause de l'application                                         //
//============================================================================//
void CD3DApplication::Pause( bool bPause )
{
    static DWORD dwAppPausedCount = 0;

    dwAppPausedCount += ( bPause ? +1 : -1 );
    m_bActive         = ( dwAppPausedCount ? false : true );

    // Vérifie la première requête de pause
    if( bPause && ( 1 == dwAppPausedCount ) )
    {
        // On stope l'animation
        if( m_bFrameMoving )
            DXUtil_Timer( TIMER_STOP );
    }

    if( 0 == dwAppPausedCount )
    {
        // On relance les timers
        if( m_bFrameMoving )
            DXUtil_Timer( TIMER_START );
    }
}

//============================================================================//
// Nettoie les objets de l'environnement                                      //
//============================================================================//
void CD3DApplication::Cleanup3DEnvironment()
{
    if( m_pd3dDevice != NULL )
    {
        if( m_bDeviceObjectsRestored )
        {
            m_bDeviceObjectsRestored = false;
            OnLostDevice();
        }
        if( m_bDeviceObjectsInited )
        {
            m_bDeviceObjectsInited = false;
            OnDestroyDevice();
        }

        if( m_pd3dDevice->Release() > 0 )
            DisplayErrorMsg( D3DAPPERR_NONZEROREFCOUNT, MSGERR_APPMUSTEXIT );
        m_pd3dDevice = NULL;
    }
}

//============================================================================//
// Affiche un message d'erreur dans une message box                            //
//============================================================================//
HRESULT CD3DApplication::DisplayErrorMsg( HRESULT hr, DWORD dwType )
{
    static bool s_bFatalErrorReported = false;
    TCHAR strMsg[512];

    if( s_bFatalErrorReported )
        return hr;

    switch( hr )
    {
        case D3DAPPERR_NODIRECT3D:
            _tcscpy( strMsg, _T("Could not initialize Direct3D. You may\n")
                             _T("want to check that the latest version of\n")
                             _T("DirectX is correctly installed on your\n")
                             _T("system.  ") );
            break;

        case D3DAPPERR_NOCOMPATIBLEDEVICES:
            _tcscpy( strMsg, _T("Could not find any compatible Direct3D\n")
                             _T("devices.") );
            break;

        case D3DAPPERR_NOWINDOWABLEDEVICES:
            _tcscpy( strMsg, _T("This game cannot run in a desktop\n")
                             _T("window with the current display settings.\n")
                             _T("Please change your desktop settings to a\n")
                             _T("16- or 32-bit display mode and re-run the\n")
                             _T("game.") );
            break;

        case D3DAPPERR_NOHARDWAREDEVICE:
            _tcscpy( strMsg, _T("No hardware-accelerated Direct3D devices\n")
                             _T("were found.") );
            break;

        case D3DAPPERR_HALNOTCOMPATIBLE:
            _tcscpy( strMsg, _T("This game requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator.") );
            break;

        case D3DAPPERR_NOWINDOWEDHAL:
            _tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window.\n") ) ; 
            break;

        case D3DAPPERR_NODESKTOPHAL:
            _tcscpy( strMsg, _T("Your Direct3D hardware accelerator cannot\n")
                             _T("render into a window with the current\n")
                             _T("desktop display settings.\n") ) ; 
            break;

        case D3DAPPERR_NOHALTHISMODE:
            _tcscpy( strMsg, _T("This game requires functionality that is\n")
                             _T("not available on your Direct3D hardware\n")
                             _T("accelerator with the current desktop display\n")
                             _T("settings.\n") ) ; 
            break;

        case D3DAPPERR_MEDIANOTFOUND:
        case HRESULT_FROM_WIN32( ERROR_FILE_NOT_FOUND ):
            _tcscpy( strMsg, _T("Could not load required media." ) );
            break;

        case D3DAPPERR_RESETFAILED:
            _tcscpy( strMsg, _T("Could not reset the Direct3D device." ) );
            break;

        case D3DAPPERR_NONZEROREFCOUNT:
            _tcscpy( strMsg, _T("A D3D object has a non-zero reference\n")
                             _T("count.") );
            break;

        case D3DAPPERR_NULLREFDEVICE:
            _tcscpy( strMsg, _T("Reference Device: Nothing will be rendered.\n") ) ; 
            break;

        case E_OUTOFMEMORY:
            _tcscpy( strMsg, _T("Not enough memory.") );
            break;

        case D3DERR_OUTOFVIDEOMEMORY:
            _tcscpy( strMsg, _T("Not enough video memory.") );
            break;

        default:
            _tcscpy( strMsg, _T("Internal application error.") );
    }

    if( MSGERR_APPMUSTEXIT == dwType )
    {
        s_bFatalErrorReported = true;
        _tcscat( strMsg, _T("\n\nThis game will now exit.") );
        MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONERROR|MB_OK );

        // Ferme la fenêtre, qui arrêtera l'application
        if( m_hWnd )
            SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
    }
    else
    {
        if( MSGWARN_SWITCHEDTOREF == dwType )
            _tcscat( strMsg, _T("\n\nSwitching to the reference rasterizer,\n")
                             _T("a software device that implements the entire\n")
                             _T("Direct3D feature set, but runs very slowly.") 
                             _T("\n\nContinue?") 
							 );
        if ( IDYES != MessageBox( NULL, strMsg, m_strWindowTitle, MB_ICONWARNING|MB_YESNO ) ) 
			// Fermeture 
	        if( m_hWnd )
		        SendMessage( m_hWnd, WM_CLOSE, 0, 0 );
    } ; 

    return hr;
}