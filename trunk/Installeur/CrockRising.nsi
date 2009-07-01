

; Nom et fichier
Name "Crock Rising"
OutFile "CrockRising_Setup.exe"

; Dossier d'installation par défaut
InstallDir "$PROGRAMFILES\Crock Rising"

; Style
XPStyle on

;*******************************************************************

; Choix dossier d'installation
Page directory

;*******************************************************************

; Copie des fichiers..
Page instfiles
Section
	
	; Exe
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
	
	WriteUninstaller "$INSTDIR\Uninstall.exe"
	
SectionEnd

;*******************************************************************

Section "Uninstall"

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
