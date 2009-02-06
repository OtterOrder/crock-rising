#include	"GameCamera.h"

#include	"Core/Inputs/InputManager.h"

//******************************************************************

void GameCamera::Update()
{
	/*InputManager *pIM = InputManager::GetInstance();
	Point2f point = pIM->GetMouseOffset();
	
	//OffsetCurseur -> Camera
	const int sensibilite = 15;
	int offsetCursor;

	if( pIM->IsKeyPressed( 'Z' ) )
	{
		Vector3f pos = GetPosition();
		Vector3f target	 = GetTarget();
		Vector3f vectDir = target - pos;
		float distance = sqrt( vectDir.x*vectDir.x + vectDir.y*vectDir.y + vectDir.z*vectDir.z );
		float angleY = GetOrientationYRad();
		
		//float s = sin( angleY );
		float c = cos( angleY );

		//pos.x =  pos.x + s;
		pos.z =  pos.z + c/2;		

		if( distance>60.0f )
			SetPosition( pos );
	}

	if( pIM->IsKeyPressed( 'S' ) )
	{
		Vector3f pos = GetPosition();
		Vector3f target	 = GetTarget();
		Vector3f vectDir = target - pos;
		float distance = sqrt( vectDir.x*vectDir.x + vectDir.y*vectDir.y + vectDir.z*vectDir.z );
		float angleY = GetOrientationYRad();
		
		//float s = sin( angleY );
		float c = cos( angleY );

		//pos.x =  pos.x + s;
		pos.z =  pos.z - c/2;		

		if( distance<300.0f )
			SetPosition( pos );
	}

	if( pIM->IsKeyPressed( 'A' ) )
	{
		InitAngle();
		SetPosition(Vector3f(0.0f, 10.0f, -100.0f));
	}


	if( point.x != 0 ) 
	{
		offsetCursor = (int)point.x%sensibilite; 
		SetOrientationY( -offsetCursor );
	}
	if( point.y != 0 ) 
	{
		offsetCursor = (int)point.y%sensibilite; 
		SetOrientationX( offsetCursor );
	}

	UpdateMatrixView();*/
}
