

; Nom et fichier
Name "Crock Rising"
OutFile "CrockRising_Setup.exe"

; Dossier d'installation par défaut
InstallDir "$PROGRAMFILES\Crock Rising"

; Style
XPStyle on

;*******************************************************************

; Pages
Page directory
Page components
Page instfiles

;*******************************************************************

; Fichiers du jeu..
Section "!Crock Rising"
	
	; Exe + dll
	SetOutPath $INSTDIR
	File "..\src\finalrelease\CrockRising.exe"
	File "redist\NxCharacter.dll"
	
	; Data\anim
	SetOutPath "$INSTDIR\data\anim"
	File "..\src\data\anim\*.DAE"
	
	; Data\mapAI
	SetOutPath "$INSTDIR\data\mapAI"
	File "..\src\data\mapAI\*.bmp"
	
	; Data\models
	SetOutPath "$INSTDIR\data\models"
	File "..\src\data\models\*.DAE"
	
	; Data\models\physic
	SetOutPath "$INSTDIR\data\models\physic"
	File "..\src\data\models\physic\*.DAE"
	
	; Data\shader
	SetOutPath "$INSTDIR\data\shader"
	File "..\src\data\shader\*.fx"
	
	; Data\sound
	SetOutPath "$INSTDIR\data\sound"
	File "..\src\data\sound\*.ogg"
	
	; Data\sprite
	SetOutPath "$INSTDIR\data\sprite"
	File "..\src\data\sprite\*.png"
	File "..\src\data\sprite\*.jpg"
	
	; Data\texture
	SetOutPath "$INSTDIR\data\texture"
	File "..\src\data\texture\*.bmp"
	File "..\src\data\texture\*.png"
	File "..\src\data\texture\*.jpg"
	File "..\src\data\texture\*.dds"
	
	; Création du désinstalleur..
	WriteUninstaller "$INSTDIR\Uninstall.exe"
	
	; Raccourcis dans le menu Démarrer
	SetOutPath $INSTDIR ; /!\ Contexte de lancement des raccourcis
	CreateDirectory "$SMPROGRAMS\Crock Rising"
	CreateShortCut "$SMPROGRAMS\Crock Rising\Crock Rising.lnk" "$INSTDIR\CrockRising.exe"
	CreateShortCut "$SMPROGRAMS\Crock Rising\Uninstall.lnk" "$INSTDIR\Uninstall.exe"
	
SectionEnd

;*******************************************************************

Section "DirectX"
	
	SetOutPath "$INSTDIR\Temp"
	File "redist\directx_aug2008_redist.exe"
	ExecWait '"$INSTDIR\Temp\directx_aug2008_redist.exe"'
	
SectionEnd

;*******************************************************************

Section "PhysX"
	
	SetOutPath "$INSTDIR\Temp"
	File "redist\PhysX_9.09.0428_SystemSoftware.exe"
	ExecWait '"$INSTDIR\Temp\PhysX_9.09.0428_SystemSoftware.exe"'
	
SectionEnd

;*******************************************************************

Section "OpenAL"
	
	SetOutPath "$INSTDIR\Temp"
	File "redist\oalinst.exe"
	ExecWait '"$INSTDIR\Temp\oalinst.exe"'
	
SectionEnd

;*******************************************************************

Section "-Finish Installation"
	
	; On termine proprement en supprimant les redistribuables
	SetOutPath $INSTDIR ; /!\ Sinon on pourra pas supprimer Temp
	Delete "$INSTDIR\Temp\*"
	RMDir "$INSTDIR\Temp"
	
SectionEnd

;*******************************************************************

; Désinstallation
Section "Uninstall"

	; Menu Démarrer
	Delete "$SMPROGRAMS\Crock Rising\*"
	RMDir "$SMPROGRAMS\Crock Rising"
	
	; On vire les redistribuables au cas ou ça n'aurait
	; pas marché à la fin de l'installation..
	Delete "$INSTDIR\Temp\*"
	RMDir "$INSTDIR\Temp"
	
	; Répertoire d'installation
	Delete "$INSTDIR\data\anim\*"
	RMDir "$INSTDIR\data\anim"
	Delete "$INSTDIR\data\mapAI\*"
	RMDir "$INSTDIR\data\mapAI"
	Delete "$INSTDIR\data\models\physic\*"
	RMDir "$INSTDIR\data\models\physic"
	Delete "$INSTDIR\data\models\*"
	RMDir "$INSTDIR\data\models"
	Delete "$INSTDIR\data\shader\*"
	RMDir "$INSTDIR\data\shader"
	Delete "$INSTDIR\data\sound\*"
	RMDir "$INSTDIR\data\sound"
	Delete "$INSTDIR\data\sprite\*"
	RMDir "$INSTDIR\data\sprite"
	Delete "$INSTDIR\data\texture\*"
	RMDir "$INSTDIR\data\texture"
	RMDir "$INSTDIR\data"
	Delete "$INSTDIR\*"
	RMDir "$INSTDIR"
	
SectionEnd

