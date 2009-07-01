
;*******************************************************************

; Nom de l'installeur
Name "Crock Rising"

; Installeur
OutFile "CrockRising_Setup.exe"

; Dossier d'installation par défaut
InstallDir "$PROGRAMFILES\Crock Rising"

;*******************************************************************

; Pages
Page directory
Page instfiles

;*******************************************************************

; Ce qu'il faut copier..
Section ""
	
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
	
SectionEnd
