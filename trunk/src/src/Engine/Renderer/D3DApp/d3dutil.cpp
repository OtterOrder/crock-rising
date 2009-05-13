#define STRICT

//===========================================================================//
// Include                                                                   //
//===========================================================================//

#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500 
#endif

#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0500 
#endif
#define NOMINMAX
#include <Windows.h>
#include <WindowsX.h>
#include <tchar.h>
#include <stdio.h>
#include "D3DUtil.h"
#include "DXUtil.h"
#include "D3DX9.h"

//===========================================================================//
// Initialise une structure D3DMATERIAL9 avec une couleur diffuse et ambiante//
//===========================================================================//
VOID D3DUtil_InitMaterial( D3DMATERIAL9& mtrl, FLOAT r, FLOAT g, FLOAT b,
                           FLOAT a )
{
    ZeroMemory( &mtrl, sizeof(D3DMATERIAL9) );
    mtrl.Diffuse.r = mtrl.Ambient.r = r;
    mtrl.Diffuse.g = mtrl.Ambient.g = g;
    mtrl.Diffuse.b = mtrl.Ambient.b = b;
    mtrl.Diffuse.a = mtrl.Ambient.a = a;
}

//===========================================================================//
// Initialise une structure D3DLIGHT avec une position						 //
//===========================================================================//
VOID D3DUtil_InitLight( D3DLIGHT9& light, D3DLIGHTTYPE ltType,
                        FLOAT x, FLOAT y, FLOAT z )
{
    D3DXVECTOR3 vecLightDirUnnormalized(x, y, z);
    ZeroMemory( &light, sizeof(D3DLIGHT9) );
    light.Type        = ltType;
    light.Diffuse.r   = 1.0f;
    light.Diffuse.g   = 1.0f;
    light.Diffuse.b   = 1.0f;
    D3DXVec3Normalize( (D3DXVECTOR3*)&light.Direction, &vecLightDirUnnormalized );
    light.Position.x   = x;
    light.Position.y   = y;
    light.Position.z   = z;
    light.Range        = 1000.0f;
}

//===========================================================================//
// Fonction d'aide pour créer une texture									 //
//===========================================================================//
HRESULT D3DUtil_CreateTexture( LPDIRECT3DDEVICE9 pd3dDevice, TCHAR* strTexture,
                               LPDIRECT3DTEXTURE9* ppTexture, D3DFORMAT d3dFormat )
{
    HRESULT hr;
    TCHAR strPath[MAX_PATH];

    // Chemin de la texture
    if( FAILED( hr = DXUtil_FindMediaFileCb( strPath, sizeof(strPath), strTexture ) ) )
        return hr;

    // Création de la texture avec DirectX
    return D3DXCreateTextureFromFileEx( pd3dDevice, strPath, 
                D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, d3dFormat, 
                D3DPOOL_MANAGED, D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
                D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, NULL, NULL, ppTexture );
}

//===========================================================================//
// Retourne une matrice de vue pour le rendu d'une face d'un cubemap		 //
//===========================================================================//
D3DXMATRIX D3DUtil_GetCubeMapViewMatrix( DWORD dwFace )
{
    D3DXVECTOR3 vEyePt   = D3DXVECTOR3( 0.0f, 0.0f, 0.0f );
    D3DXVECTOR3 vLookDir;
    D3DXVECTOR3 vUpDir;

    switch( dwFace )
    {
        case D3DCUBEMAP_FACE_POSITIVE_X:
            vLookDir = D3DXVECTOR3( 1.0f, 0.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_X:
            vLookDir = D3DXVECTOR3(-1.0f, 0.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Y:
            vLookDir = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Y:
            vLookDir = D3DXVECTOR3( 0.0f,-1.0f, 0.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
            break;
        case D3DCUBEMAP_FACE_POSITIVE_Z:
            vLookDir = D3DXVECTOR3( 0.0f, 0.0f, 1.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
        case D3DCUBEMAP_FACE_NEGATIVE_Z:
            vLookDir = D3DXVECTOR3( 0.0f, 0.0f,-1.0f );
            vUpDir   = D3DXVECTOR3( 0.0f, 1.0f, 0.0f );
            break;
    }

    // Matrice de vue pour cette surface de cubemap
    D3DXMATRIXA16 matView;
    D3DXMatrixLookAtLH( &matView, &vEyePt, &vLookDir, &vUpDir );
    return matView;
}

//===========================================================================//
// Retourne un quaternion pour la rotation faire par la rotation du curseur  //
//===========================================================================//
D3DXQUATERNION D3DUtil_GetRotationFromCursor( HWND hWnd,
                                              FLOAT fTrackBallRadius )
{
    POINT pt;
    RECT  rc;
    GetCursorPos( &pt );
    GetClientRect( hWnd, &rc );
    ScreenToClient( hWnd, &pt );
    FLOAT sx = ( ( ( 2.0f * pt.x ) / (rc.right-rc.left) ) - 1 );
    FLOAT sy = ( ( ( 2.0f * pt.y ) / (rc.bottom-rc.top) ) - 1 );
    FLOAT sz;

    if( sx == 0.0f && sy == 0.0f )
        return D3DXQUATERNION( 0.0f, 0.0f, 0.0f, 1.0f );

    FLOAT d2 = sqrtf( sx*sx + sy*sy );

    if( d2 < fTrackBallRadius * 0.70710678118654752440 ) // Inside sphere
        sz = sqrtf( fTrackBallRadius*fTrackBallRadius - d2*d2 );
    else                                                 // On hyperbola
        sz = (fTrackBallRadius*fTrackBallRadius) / (2.0f*d2);

    // On prend deux points sur la sphere de trackball
    D3DXVECTOR3 p1( sx, sy, sz );
    D3DXVECTOR3 p2( 0.0f, 0.0f, fTrackBallRadius );

    // L'axe de rotation et le produit vectoriel de p1 et p2
    D3DXVECTOR3 vAxis;
    D3DXVec3Cross( &vAxis, &p1, &p2);

    // Calcul l'angle pour la rotation selon l'axe
    D3DXVECTOR3 vecDiff = p2-p1;
    FLOAT t = D3DXVec3Length( &vecDiff ) / ( 2.0f*fTrackBallRadius );
    if( t > +1.0f) t = +1.0f;
    if( t < -1.0f) t = -1.0f;
    FLOAT fAngle = 2.0f * asinf( t );

    // Convertion de l'angle en quaternion
    D3DXQUATERNION quat;
    D3DXQuaternionRotationAxis( &quat, &vAxis, fAngle );
    return quat;
}

//===========================================================================//
// Construit et met un curseur pour le d3d device grâce au hCursor           //
//===========================================================================//
HRESULT D3DUtil_SetDeviceCursor( LPDIRECT3DDEVICE9 pd3dDevice, HCURSOR hCursor,
                                 BOOL bAddWatermark )
{
    HRESULT hr = E_FAIL;
    ICONINFO iconinfo;
    BOOL bBWCursor;
    LPDIRECT3DSURFACE9 pCursorSurface = NULL;
    HDC hdcColor = NULL;
    HDC hdcMask = NULL;
    HDC hdcScreen = NULL;
    BITMAP bm;
    DWORD dwWidth;
    DWORD dwHeightSrc;
    DWORD dwHeightDest;
    COLORREF crColor;
    COLORREF crMask;
    UINT x;
    UINT y;
    BITMAPINFO bmi;
    COLORREF* pcrArrayColor = NULL;
    COLORREF* pcrArrayMask = NULL;
    DWORD* pBitmap;
    HGDIOBJ hgdiobjOld;

    ZeroMemory( &iconinfo, sizeof(iconinfo) );
    if( !GetIconInfo( hCursor, &iconinfo ) )
        goto End;

    if (0 == GetObject((HGDIOBJ)iconinfo.hbmMask, sizeof(BITMAP), (LPVOID)&bm))
        goto End;
    dwWidth = bm.bmWidth;
    dwHeightSrc = bm.bmHeight;

    if( iconinfo.hbmColor == NULL )
    {
        bBWCursor = TRUE;
        dwHeightDest = dwHeightSrc / 2;
    }
    else 
    {
        bBWCursor = FALSE;
        dwHeightDest = dwHeightSrc;
    }

    // Création d'une surface pour le curseur plein écran
    if( FAILED( hr = pd3dDevice->CreateOffscreenPlainSurface( dwWidth, dwHeightDest, 
        D3DFMT_A8R8G8B8, D3DPOOL_SCRATCH, &pCursorSurface, NULL ) ) )
    {
        goto End;
    }

    pcrArrayMask = new DWORD[dwWidth * dwHeightSrc];

    ZeroMemory(&bmi, sizeof(bmi));
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = dwWidth;
    bmi.bmiHeader.biHeight = dwHeightSrc;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    hdcScreen = GetDC( NULL );
    hdcMask = CreateCompatibleDC( hdcScreen );
    if( hdcMask == NULL )
    {
        hr = E_FAIL;
        goto End;
    }
    hgdiobjOld = SelectObject(hdcMask, iconinfo.hbmMask);
    GetDIBits(hdcMask, iconinfo.hbmMask, 0, dwHeightSrc, 
        pcrArrayMask, &bmi, DIB_RGB_COLORS);
    SelectObject(hdcMask, hgdiobjOld);

    if (!bBWCursor)
    {
        pcrArrayColor = new DWORD[dwWidth * dwHeightDest];
        hdcColor = CreateCompatibleDC( hdcScreen );
        if( hdcColor == NULL )
        {
            hr = E_FAIL;
            goto End;
        }
        SelectObject(hdcColor, iconinfo.hbmColor);
        GetDIBits(hdcColor, iconinfo.hbmColor, 0, dwHeightDest, 
            pcrArrayColor, &bmi, DIB_RGB_COLORS);
    }

    // Transfère l'image du curseur sur la surface
    D3DLOCKED_RECT lr;
    pCursorSurface->LockRect( &lr, NULL, 0 );
    pBitmap = (DWORD*)lr.pBits;
    for( y = 0; y < dwHeightDest; y++ )
    {
        for( x = 0; x < dwWidth; x++ )
        {
            if (bBWCursor)
            {
                crColor = pcrArrayMask[dwWidth*(dwHeightDest-1-y) + x];
                crMask = pcrArrayMask[dwWidth*(dwHeightSrc-1-y) + x];
            }
            else
            {
                crColor = pcrArrayColor[dwWidth*(dwHeightDest-1-y) + x];
                crMask = pcrArrayMask[dwWidth*(dwHeightDest-1-y) + x];
            }
            if (crMask == 0)
                pBitmap[dwWidth*y + x] = 0xff000000 | crColor;
            else
                pBitmap[dwWidth*y + x] = 0x00000000;

            if( bAddWatermark && x < 12 && y < 5 )
            {
                // 11.. 11.. 11.. .... CCC0
                // 1.1. ..1. 1.1. .... A2A0
                // 1.1. .1.. 1.1. .... A4A0
                // 1.1. ..1. 1.1. .... A2A0
                // 11.. 11.. 11.. .... CCC0

                const WORD wMask[5] = { 0xccc0, 0xa2a0, 0xa4a0, 0xa2a0, 0xccc0 };
                if( wMask[y] & (1 << (15 - x)) )
                {
                    pBitmap[dwWidth*y + x] |= 0xff808080;
                }
            }
        }
    }
    pCursorSurface->UnlockRect();

    // On met le curseur du device
    if( FAILED( hr = pd3dDevice->SetCursorProperties( iconinfo.xHotspot, 
        iconinfo.yHotspot, pCursorSurface ) ) )
    {
        goto End;
    }

    hr = S_OK;

End:
    if( iconinfo.hbmMask != NULL )
        DeleteObject( iconinfo.hbmMask );
    if( iconinfo.hbmColor != NULL )
        DeleteObject( iconinfo.hbmColor );
    if( hdcScreen != NULL )
        ReleaseDC( NULL, hdcScreen );
    if( hdcColor != NULL )
        DeleteDC( hdcColor );
    if( hdcMask != NULL )
        DeleteDC( hdcMask );
    SAFE_DELETE_ARRAY( pcrArrayColor );
    SAFE_DELETE_ARRAY( pcrArrayMask );
    SAFE_RELEASE( pCursorSurface );
    return hr;
}

//===========================================================================//
// Retourne le string pour le D3DFORMAT donné                                //
//===========================================================================//
TCHAR* D3DUtil_D3DFormatToString( D3DFORMAT format, bool bWithPrefix )
{
    TCHAR* pstr = NULL;
    switch( format )
    {
    case D3DFMT_UNKNOWN:         pstr = TEXT("D3DFMT_UNKNOWN"); break;
    case D3DFMT_R8G8B8:          pstr = TEXT("D3DFMT_R8G8B8"); break;
    case D3DFMT_A8R8G8B8:        pstr = TEXT("D3DFMT_A8R8G8B8"); break;
    case D3DFMT_X8R8G8B8:        pstr = TEXT("D3DFMT_X8R8G8B8"); break;
    case D3DFMT_R5G6B5:          pstr = TEXT("D3DFMT_R5G6B5"); break;
    case D3DFMT_X1R5G5B5:        pstr = TEXT("D3DFMT_X1R5G5B5"); break;
    case D3DFMT_A1R5G5B5:        pstr = TEXT("D3DFMT_A1R5G5B5"); break;
    case D3DFMT_A4R4G4B4:        pstr = TEXT("D3DFMT_A4R4G4B4"); break;
    case D3DFMT_R3G3B2:          pstr = TEXT("D3DFMT_R3G3B2"); break;
    case D3DFMT_A8:              pstr = TEXT("D3DFMT_A8"); break;
    case D3DFMT_A8R3G3B2:        pstr = TEXT("D3DFMT_A8R3G3B2"); break;
    case D3DFMT_X4R4G4B4:        pstr = TEXT("D3DFMT_X4R4G4B4"); break;
    case D3DFMT_A2B10G10R10:     pstr = TEXT("D3DFMT_A2B10G10R10"); break;
    case D3DFMT_A8B8G8R8:        pstr = TEXT("D3DFMT_A8B8G8R8"); break;
    case D3DFMT_X8B8G8R8:        pstr = TEXT("D3DFMT_X8B8G8R8"); break;
    case D3DFMT_G16R16:          pstr = TEXT("D3DFMT_G16R16"); break;
    case D3DFMT_A2R10G10B10:     pstr = TEXT("D3DFMT_A2R10G10B10"); break;
    case D3DFMT_A16B16G16R16:    pstr = TEXT("D3DFMT_A16B16G16R16"); break;
    case D3DFMT_A8P8:            pstr = TEXT("D3DFMT_A8P8"); break;
    case D3DFMT_P8:              pstr = TEXT("D3DFMT_P8"); break;
    case D3DFMT_L8:              pstr = TEXT("D3DFMT_L8"); break;
    case D3DFMT_A8L8:            pstr = TEXT("D3DFMT_A8L8"); break;
    case D3DFMT_A4L4:            pstr = TEXT("D3DFMT_A4L4"); break;
    case D3DFMT_V8U8:            pstr = TEXT("D3DFMT_V8U8"); break;
    case D3DFMT_L6V5U5:          pstr = TEXT("D3DFMT_L6V5U5"); break;
    case D3DFMT_X8L8V8U8:        pstr = TEXT("D3DFMT_X8L8V8U8"); break;
    case D3DFMT_Q8W8V8U8:        pstr = TEXT("D3DFMT_Q8W8V8U8"); break;
    case D3DFMT_V16U16:          pstr = TEXT("D3DFMT_V16U16"); break;
    case D3DFMT_A2W10V10U10:     pstr = TEXT("D3DFMT_A2W10V10U10"); break;
    case D3DFMT_UYVY:            pstr = TEXT("D3DFMT_UYVY"); break;
    case D3DFMT_YUY2:            pstr = TEXT("D3DFMT_YUY2"); break;
    case D3DFMT_DXT1:            pstr = TEXT("D3DFMT_DXT1"); break;
    case D3DFMT_DXT2:            pstr = TEXT("D3DFMT_DXT2"); break;
    case D3DFMT_DXT3:            pstr = TEXT("D3DFMT_DXT3"); break;
    case D3DFMT_DXT4:            pstr = TEXT("D3DFMT_DXT4"); break;
    case D3DFMT_DXT5:            pstr = TEXT("D3DFMT_DXT5"); break;
    case D3DFMT_D16_LOCKABLE:    pstr = TEXT("D3DFMT_D16_LOCKABLE"); break;
    case D3DFMT_D32:             pstr = TEXT("D3DFMT_D32"); break;
    case D3DFMT_D15S1:           pstr = TEXT("D3DFMT_D15S1"); break;
    case D3DFMT_D24S8:           pstr = TEXT("D3DFMT_D24S8"); break;
    case D3DFMT_D24X8:           pstr = TEXT("D3DFMT_D24X8"); break;
    case D3DFMT_D24X4S4:         pstr = TEXT("D3DFMT_D24X4S4"); break;
    case D3DFMT_D16:             pstr = TEXT("D3DFMT_D16"); break;
    case D3DFMT_L16:             pstr = TEXT("D3DFMT_L16"); break;
    case D3DFMT_VERTEXDATA:      pstr = TEXT("D3DFMT_VERTEXDATA"); break;
    case D3DFMT_INDEX16:         pstr = TEXT("D3DFMT_INDEX16"); break;
    case D3DFMT_INDEX32:         pstr = TEXT("D3DFMT_INDEX32"); break;
    case D3DFMT_Q16W16V16U16:    pstr = TEXT("D3DFMT_Q16W16V16U16"); break;
    case D3DFMT_MULTI2_ARGB8:    pstr = TEXT("D3DFMT_MULTI2_ARGB8"); break;
    case D3DFMT_R16F:            pstr = TEXT("D3DFMT_R16F"); break;
    case D3DFMT_G16R16F:         pstr = TEXT("D3DFMT_G16R16F"); break;
    case D3DFMT_A16B16G16R16F:   pstr = TEXT("D3DFMT_A16B16G16R16F"); break;
    case D3DFMT_R32F:            pstr = TEXT("D3DFMT_R32F"); break;
    case D3DFMT_G32R32F:         pstr = TEXT("D3DFMT_G32R32F"); break;
    case D3DFMT_A32B32G32R32F:   pstr = TEXT("D3DFMT_A32B32G32R32F"); break;
    case D3DFMT_CxV8U8:          pstr = TEXT("D3DFMT_CxV8U8"); break;
    default:                     pstr = TEXT("Unknown format"); break;
    }
    if( bWithPrefix || _tcsstr( pstr, TEXT("D3DFMT_") )== NULL )
        return pstr;
    else
        return pstr + lstrlen( TEXT("D3DFMT_") );
}

//===========================================================================//
// Axis to axis quaternion double angle (no normalization)					 //					
// Takes two points on unit sphere an angle THETA apart, returns			 //
// quaternion that represents a rotation around cross product by 2*THETA.    //
//===========================================================================//
inline D3DXQUATERNION* WINAPI D3DXQuaternionUnitAxisToUnitAxis2
( D3DXQUATERNION *pOut, const D3DXVECTOR3 *pvFrom, const D3DXVECTOR3 *pvTo)
{
    D3DXVECTOR3 vAxis;
    D3DXVec3Cross(&vAxis, pvFrom, pvTo);    // proportionel à sin(theta)
    pOut->x = vAxis.x;
    pOut->y = vAxis.y;
    pOut->z = vAxis.z;
    pOut->w = D3DXVec3Dot( pvFrom, pvTo );
    return pOut;
}

//===========================================================================// 
// Takes two points on unit sphere an angle THETA apart, returns             //
// quaternion that represents a rotation around cross product by theta.      //
//===========================================================================//
inline D3DXQUATERNION* WINAPI D3DXQuaternionAxisToAxis
( D3DXQUATERNION *pOut, const D3DXVECTOR3 *pvFrom, const D3DXVECTOR3 *pvTo)
{
    D3DXVECTOR3 vA, vB;
    D3DXVec3Normalize(&vA, pvFrom);
    D3DXVec3Normalize(&vB, pvTo);
    D3DXVECTOR3 vHalf(vA + vB);
    D3DXVec3Normalize(&vHalf, &vHalf);
    return D3DXQuaternionUnitAxisToUnitAxis2(pOut, &vA, &vHalf);
}

//===========================================================================//
// Constructeur CBaseCamera					                                 //
//===========================================================================//
CBaseCamera::CBaseCamera()
{
    m_cKeysDown = 0;
    ZeroMemory( m_aKeys, sizeof(BYTE)*CAM_MAX_KEYS );

    // Attributs de la matrice vue
    D3DXVECTOR3 vEyePt    = D3DXVECTOR3(0.0f,0.0f,0.0f);
    D3DXVECTOR3 vLookatPt = D3DXVECTOR3(0.0f,0.0f,1.0f);

    // Matrice de vue
    SetViewParams( &vEyePt, &vLookatPt );

    // Matrice de projection
    SetProjParams( D3DX_PI/4, 1.0f, 1.0f, 1000.0f );

    GetCursorPos( &m_ptLastMousePosition );
    m_bMouseLButtonDown = false;
    m_bMouseMButtonDown = false;
    m_bMouseRButtonDown = false;
    m_nCurrentButtonMask = 0;
    m_nMouseWheelDelta = 1000;

    m_fCameraYawAngle = 0.0f;
    m_fCameraPitchAngle = 0.0f;

    SetRect( &m_rcDrag, LONG_MIN, LONG_MIN, LONG_MAX, LONG_MAX );
    m_vVelocity     = D3DXVECTOR3(0,0,0);
    m_bMovementDrag = false;
    m_vVelocityDrag = D3DXVECTOR3(0,0,0);
    m_fDragTimer    = 0.0f;
    m_fTotalDragTimeToZero = 0.25;
    m_vRotVelocity = D3DXVECTOR2(0,0);

    m_fRotationScaler = 0.01f;           
    m_fMoveScaler = 5.0f;           

    m_bInvertPitch = false;
    m_bEnableYAxisMovement = true;
    m_bEnablePositionMovement = true;

    m_vMouseDelta   = D3DXVECTOR2(0,0);
    m_fFramesToSmoothMouseData = 2.0f;

    m_bClipToBoundary = false;
    m_vMinBoundary = D3DXVECTOR3(-1,-1,-1);
    m_vMaxBoundary = D3DXVECTOR3(1,1,1);

    m_bResetCursorAfterMove = false;
}

//===========================================================================//
// Le client peut appeller ceci pour changer la position et la direction	 //
// de la caméra.															 //
//===========================================================================//
VOID CBaseCamera::SetViewParams( D3DXVECTOR3* pvEyePt, D3DXVECTOR3* pvLookatPt )
{
    if( NULL == pvEyePt || NULL == pvLookatPt )
        return;

    m_vDefaultEye = m_vEye = *pvEyePt;
    m_vDefaultLookAt = m_vLookAt = *pvLookatPt;

    // Calcul de la matrice de vue
    D3DXVECTOR3 vUp(0,1,0);
    D3DXMatrixLookAtLH( &m_mView, pvEyePt, pvLookatPt, &vUp );

    D3DXMATRIX mInvView;
    D3DXMatrixInverse( &mInvView, NULL, &m_mView );

    D3DXVECTOR3* pZBasis = (D3DXVECTOR3*) &mInvView._31;

    m_fCameraYawAngle   = atan2f( pZBasis->x, pZBasis->z );
    float fLen = sqrtf(pZBasis->z*pZBasis->z + pZBasis->x*pZBasis->x);
    m_fCameraPitchAngle = -atan2f( pZBasis->y, fLen );
}

//===========================================================================//
// Calculer la matrice de projection basée sur les paramètres.				 //
//===========================================================================//
VOID CBaseCamera::SetProjParams( FLOAT fFOV, FLOAT fAspect, FLOAT fNearPlane,
                                   FLOAT fFarPlane )
{
    // Attributs pour la matrice de projection
    m_fFOV        = fFOV;
    m_fAspect     = fAspect;
    m_fNearPlane  = fNearPlane;
    m_fFarPlane   = fFarPlane;

    D3DXMatrixPerspectiveFovLH( &m_mProj, fFOV, fAspect, fNearPlane, fFarPlane );
}

//===========================================================================//
// Ecoute des messages pour la caméra.										 //
//===========================================================================//
LRESULT CBaseCamera::HandleMessages( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{
    UNREFERENCED_PARAMETER( hWnd );
    UNREFERENCED_PARAMETER( lParam );

    switch( uMsg )
    {
        case WM_KEYDOWN:
        {
            D3DUtil_CameraKeys mappedKey = MapKey( (UINT)wParam );
            if( mappedKey != CAM_UNKNOWN )
            {
                if( FALSE == IsKeyDown(m_aKeys[mappedKey]) )
                {
                    m_aKeys[ mappedKey ] = KEY_WAS_DOWN_MASK | KEY_IS_DOWN_MASK;
                    ++m_cKeysDown;
                }
            }
            break;
        }

        case WM_KEYUP:
        {
            D3DUtil_CameraKeys mappedKey = MapKey( (UINT)wParam );
            if( mappedKey != CAM_UNKNOWN && (DWORD)mappedKey < 8 )
            {
                m_aKeys[ mappedKey ] &= ~KEY_IS_DOWN_MASK;
                --m_cKeysDown;
            }
            break;
        }

        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDBLCLK:
        case WM_MBUTTONDBLCLK:
        case WM_LBUTTONDBLCLK:
        {
            // Calcul le drag rectangle en coordonnées écran
            POINT ptCursor = { (short)LOWORD(lParam), (short)HIWORD(lParam) };

            // Mise à jour variable d'état
            if( ( uMsg == WM_LBUTTONDOWN || uMsg == WM_LBUTTONDBLCLK ) && PtInRect( &m_rcDrag, ptCursor ) )
                { m_bMouseLButtonDown = true; m_nCurrentButtonMask |= MOUSE_LEFT_BUTTON; }
           /* if( ( uMsg == WM_MBUTTONDOWN || uMsg == WM_MBUTTONDBLCLK ) && PtInRect( &m_rcDrag, ptCursor ) )
                { m_bMouseMButtonDown = true; m_nCurrentButtonMask |= MOUSE_MIDDLE_BUTTON; }
            if( ( uMsg == WM_RBUTTONDOWN || uMsg == WM_RBUTTONDBLCLK ) && PtInRect( &m_rcDrag, ptCursor ) )
                { m_bMouseRButtonDown = true; m_nCurrentButtonMask |= MOUSE_RIGHT_BUTTON; }*/

            // Capture la souris, donc si le bouton de la souris est relaché en dehors de la fenêtre 
            // il y a aura un message WM_LBUTTONUP
            SetCapture(hWnd);
            GetCursorPos( &m_ptLastMousePosition );
            return TRUE;
        }

        case WM_RBUTTONUP: 
        case WM_MBUTTONUP: 
        case WM_LBUTTONUP:   
        {
            // Mise à jour variables d'état
            if( uMsg == WM_LBUTTONUP ) { m_bMouseLButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_LEFT_BUTTON; }
            if( uMsg == WM_MBUTTONUP ) { m_bMouseMButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_MIDDLE_BUTTON; }
            if( uMsg == WM_RBUTTONUP ) { m_bMouseRButtonDown = false; m_nCurrentButtonMask &= ~MOUSE_RIGHT_BUTTON; }

            // Arret capture si aucun bouton est appuyé
            if( !m_bMouseLButtonDown  && 
                !m_bMouseRButtonDown &&
                !m_bMouseMButtonDown )
            {
                ReleaseCapture();
            }
            break;
        }

        case WM_CAPTURECHANGED:
        {
            if( (HWND)lParam != hWnd )
            {
                if( (m_nCurrentButtonMask & MOUSE_LEFT_BUTTON) ||
                    (m_nCurrentButtonMask & MOUSE_MIDDLE_BUTTON) ||
                    (m_nCurrentButtonMask & MOUSE_RIGHT_BUTTON) )
                {
                    m_bMouseLButtonDown = false;
                    m_bMouseMButtonDown = false;
                    m_bMouseRButtonDown = false;
                    m_nCurrentButtonMask &= ~MOUSE_LEFT_BUTTON;
                    m_nCurrentButtonMask &= ~MOUSE_MIDDLE_BUTTON;
                    m_nCurrentButtonMask &= ~MOUSE_RIGHT_BUTTON;
                    ReleaseCapture();
                }
            }
            break;
        }

        case WM_MOUSEWHEEL: 
		{
            // Mise à jour variables d'état
			int acc=(short)HIWORD(wParam);
			if(acc<1200.0f)
				m_nMouseWheelDelta += (short)HIWORD(wParam);
			if(m_nMouseWheelDelta<0)
				m_nMouseWheelDelta=40;
            break;
		}
    }

    return FALSE;
}

//===========================================================================//
// Récupération des entrée pour calculer vitesse.    						 //
//===========================================================================//
void CBaseCamera::GetInput( bool bGetKeyboardInput, bool bGetMouseInput, bool bGetGamepadInput, bool bResetCursorAfterMove )
{
    m_vKeyboardDirection = D3DXVECTOR3(0,0,0);
    if( bGetKeyboardInput )
    {
        // Mise à jour accélération basée sur les états clavier
        if( IsKeyDown(m_aKeys[CAM_MOVE_FORWARD]) )
            m_vKeyboardDirection.z += 1.0f;
        if( IsKeyDown(m_aKeys[CAM_MOVE_BACKWARD]) )
            m_vKeyboardDirection.z -= 1.0f;
        if( m_bEnableYAxisMovement )
        {
            if( IsKeyDown(m_aKeys[CAM_MOVE_UP]) )
                m_vKeyboardDirection.y += 1.0f;
            if( IsKeyDown(m_aKeys[CAM_MOVE_DOWN]) )
                m_vKeyboardDirection.y -= 1.0f;
        }
        if( IsKeyDown(m_aKeys[CAM_STRAFE_RIGHT]) )
            m_vKeyboardDirection.x += 1.0f;
        if( IsKeyDown(m_aKeys[CAM_STRAFE_LEFT]) )
            m_vKeyboardDirection.x -= 1.0f;
    }

    if( bGetMouseInput )
    {
        UpdateMouseDelta();
    }
}

//===========================================================================//
// Récupération delta souris basé sur les mouvements   						 //
//===========================================================================//
void CBaseCamera::UpdateMouseDelta()
{
    POINT ptCurMouseDelta;
    POINT ptCurMousePos;
    
    // Position courante de la souris
    GetCursorPos( &ptCurMousePos );

    // Calcul de la distance effectuée depuis la dernière frame
    ptCurMouseDelta.x = ptCurMousePos.x - m_ptLastMousePosition.x;
    ptCurMouseDelta.y = ptCurMousePos.y - m_ptLastMousePosition.y;

    // Enregistre la positon courante pour la prochaine fois
    m_ptLastMousePosition = ptCurMousePos;

   /* if( m_bResetCursorAfterMove)
    {
        POINT ptCenter;

        MONITORINFO mi;
        mi.cbSize = sizeof(MONITORINFO);
        ptCenter.x = (mi.rcMonitor.left + mi.rcMonitor.right) / 2;
        ptCenter.y = (mi.rcMonitor.top + mi.rcMonitor.bottom) / 2;
        SetCursorPos( ptCenter.x, ptCenter.y );
        m_ptLastMousePosition = ptCenter;
    }*/

    // Lissage des données 
    float fPercentOfNew =  1.0f / m_fFramesToSmoothMouseData;
    float fPercentOfOld =  1.0f - fPercentOfNew;
    m_vMouseDelta.x = m_vMouseDelta.x*fPercentOfOld + ptCurMouseDelta.x*fPercentOfNew;
    m_vMouseDelta.y = m_vMouseDelta.y*fPercentOfOld + ptCurMouseDelta.y*fPercentOfNew;

    m_vRotVelocity = m_vMouseDelta * m_fRotationScaler;
}

//===========================================================================//
// Mise à jour vélocité                                						 //
//===========================================================================//
void CBaseCamera::UpdateVelocity( float fElapsedTime )
{
    D3DXMATRIX mRotDelta;

    D3DXVECTOR3 vAccel = m_vKeyboardDirection;

    // Vecteur normalisé, donc si ça bouge selon deux axes, 
    // la caméra ne bougera pas plus vite que si il y avait qu'un axe.
    D3DXVec3Normalize( &vAccel, &vAccel );

    // Mise à l'échelle du vecteur d'accélération
	vAccel *= (m_nMouseWheelDelta)/100.0f;
    vAccel *= m_fMoveScaler;
	

    if( m_bMovementDrag )
    {
        // Est ce qu'il y a eu une accélération cette frame ?
        if( D3DXVec3LengthSq( &vAccel ) > 0 )
        {
            // Si oui, l'utilisateur a appuyé sur un bouton de mouvement
            // on change directement la vélocité en accéleration 
            m_vVelocity = vAccel;
            m_fDragTimer = m_fTotalDragTimeToZero;
            m_vVelocityDrag = vAccel / m_fDragTimer;
        }
        else 
        {
            // Si non aucune touche n'a été appuyée, on décrémente doucement jusqu'à 0
            if( m_fDragTimer > 0 )
            {
                m_vVelocity -= m_vVelocityDrag * fElapsedTime;
                m_fDragTimer -= fElapsedTime;
            }
            else
            {
                m_vVelocity = D3DXVECTOR3(0,0,0);
            }
        }
    }
    else
    {
        // Pas de glissement, on change immédiatement la vélocitée
        m_vVelocity = vAccel;
    }
}

//===========================================================================//
// Clamp de pV entre m_vMinBoundary et m_vMaxBoundary      					 //
//===========================================================================//
void CBaseCamera::ConstrainToBoundary( D3DXVECTOR3* pV )
{ 
    pV->x = __max(pV->x, m_vMinBoundary.x);
    pV->y = __max(pV->y, m_vMinBoundary.y);
    pV->z = __max(pV->z, m_vMinBoundary.z);

    pV->x = __min(pV->x, m_vMaxBoundary.x);
    pV->y = __min(pV->y, m_vMaxBoundary.y);
    pV->z = __min(pV->z, m_vMaxBoundary.z);
}



//===========================================================================//
// Mappe un clavier virtuel windows dans une énum       					 //
//===========================================================================//
D3DUtil_CameraKeys CBaseCamera::MapKey( UINT nKey )
{
    // Ceci peut être amélioré par une méthode redéfinissable par l'utilisateur 
    switch( nKey )
    {
        case VK_CONTROL:  return CAM_CONTROLDOWN;
        case VK_LEFT:  return CAM_STRAFE_LEFT;
        case VK_RIGHT: return CAM_STRAFE_RIGHT;
        case VK_UP:    return CAM_MOVE_FORWARD;
        case VK_DOWN:  return CAM_MOVE_BACKWARD;
        case VK_PRIOR: return CAM_MOVE_UP;        
        case VK_NEXT:  return CAM_MOVE_DOWN;      

        case 'Q':      return CAM_STRAFE_LEFT;
        case 'D':      return CAM_STRAFE_RIGHT;
        case 'Z':      return CAM_MOVE_FORWARD;
        case 'S':      return CAM_MOVE_BACKWARD;
        case 'A':      return CAM_MOVE_DOWN;
        case 'E':      return CAM_MOVE_UP;

        case VK_NUMPAD4: return CAM_STRAFE_LEFT;
        case VK_NUMPAD6: return CAM_STRAFE_RIGHT;
        case VK_NUMPAD8: return CAM_MOVE_FORWARD;
        case VK_NUMPAD2: return CAM_MOVE_BACKWARD;
        case VK_NUMPAD9: return CAM_MOVE_UP;        
        case VK_NUMPAD3: return CAM_MOVE_DOWN;      

        case VK_HOME:   return CAM_RESET;
    }

    return CAM_UNKNOWN;
}

//===========================================================================//
// Remet la position de la caméra par défaut            					 //
//===========================================================================//
VOID CBaseCamera::Reset()
{
    SetViewParams( &m_vDefaultEye, &m_vDefaultLookAt );
}

//===========================================================================//
// Constructeur CFirstPersonCamera			            					 //
//===========================================================================//
CFirstPersonCamera::CFirstPersonCamera() :
    m_nActiveButtonMask( 0x07 )
{
    m_bRotateWithoutButtonDown = false;
}

//===========================================================================//
// Mise à jour matrice de vue basée sur les évènement et le temps			 //
//===========================================================================//
VOID CFirstPersonCamera::FrameMove( FLOAT fElapsedTime )
{
    /*if( DXUTGetGlobalTimer()->IsStopped() )
        fElapsedTime = 1.0f / DXUTGetFPS();*/

    if( IsKeyDown(m_aKeys[CAM_RESET]) )
        Reset();

    // Evenements clavier, souris
    GetInput( m_bEnablePositionMovement, ( m_nActiveButtonMask & m_nCurrentButtonMask ) || m_bRotateWithoutButtonDown, true, m_bResetCursorAfterMove );

    //if( (m_nActiveButtonMask & m_nCurrentButtonMask) || m_bRotateWithoutButtonDown )
    //    UpdateMouseDelta( fElapsedTime );

    // Obtention de la vélocité basée sur les évènements claver
    UpdateVelocity( fElapsedTime );

    // Simple méthode d'euler pour calculer le delta de la position
    D3DXVECTOR3 vPosDelta = m_vVelocity * fElapsedTime;

    // Si il y a rotation de la caméra 
    if( (m_nActiveButtonMask & m_nCurrentButtonMask) ||
        m_bRotateWithoutButtonDown  )
    {
        // Mise à jour pitch & yaw grâce au mouvement de la souris
        float fYawDelta   = m_vRotVelocity.x;
        float fPitchDelta = m_vRotVelocity.y;

        // Invertion pitch si besoin
        if( m_bInvertPitch )
            fPitchDelta = -fPitchDelta;

        m_fCameraPitchAngle += fPitchDelta;
        m_fCameraYawAngle   += fYawDelta;

        // Limitation pitch haut et bas
        m_fCameraPitchAngle = __max( -D3DX_PI/2.0f,  m_fCameraPitchAngle );
        m_fCameraPitchAngle = __min( +D3DX_PI/2.0f,  m_fCameraPitchAngle );
    }

    // On fait une matrice de rotation basée sur les yaw et pitch de la caméra
    D3DXMATRIX mCameraRot;
    D3DXMatrixRotationYawPitchRoll( &mCameraRot, m_fCameraYawAngle, m_fCameraPitchAngle, 0 );

    // Transformation des vecteurs basé sur matrice de rotation de la caméra
    D3DXVECTOR3 vWorldUp, vWorldAhead;
    D3DXVECTOR3 vLocalUp    = D3DXVECTOR3(0,1,0);
    D3DXVECTOR3 vLocalAhead = D3DXVECTOR3(0,0,1);
    D3DXVec3TransformCoord( &vWorldUp, &vLocalUp, &mCameraRot );
    D3DXVec3TransformCoord( &vWorldAhead, &vLocalAhead, &mCameraRot );

    // Transforme le delta de la position par la caméra de rotation
    D3DXVECTOR3 vPosDeltaWorld;
    if( !m_bEnableYAxisMovement )
    {
        // Si on restreint le mouvement des Y, on inclut pas le pitch
        // quand on transforme la position du vecteur position delta
        D3DXMatrixRotationYawPitchRoll( &mCameraRot, m_fCameraYawAngle, 0.0f, 0.0f );
    }
    D3DXVec3TransformCoord( &vPosDeltaWorld, &vPosDelta, &mCameraRot );

    // On bouge la position de l'oeil
    m_vEye += vPosDeltaWorld;
    if( m_bClipToBoundary )
        ConstrainToBoundary( &m_vEye );

    // Mise à jour du lookAt grâce à la position de l'oeil 
    m_vLookAt = m_vEye + vWorldAhead;

    // Mise à jour matrice de vue
    D3DXMatrixLookAtLH( &m_mView, &m_vEye, &m_vLookAt, &vWorldUp );

    D3DXMatrixInverse( &m_mCameraWorld, NULL, &m_mView );
}

//===========================================================================//
// Active ou désactive chaque boutons de la souris pour la rotation			 //
//===========================================================================//
void CFirstPersonCamera::SetRotateButtons( bool bLeft, bool bMiddle, bool bRight, bool bRotateWithoutButtonDown )
{
    m_nActiveButtonMask = ( bLeft ? MOUSE_LEFT_BUTTON : 0 ) |
                          ( bMiddle ? MOUSE_MIDDLE_BUTTON : 0 ) |
                          ( bRight ? MOUSE_RIGHT_BUTTON : 0 );
    m_bRotateWithoutButtonDown = bRotateWithoutButtonDown;
}
