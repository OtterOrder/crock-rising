//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "Anim.h"
#include "AnimLoader.h"

Bone::~Bone()
{
	Children.clear();
	if(animationMatrix)
		delete [] animationMatrix;
}

Bone::Bone(D3DXMATRIX setMatrix, Bone * ParentBone, int setNum, std::string setSid)
{
	InitialMatrix = setMatrix;
	Position = D3DXVECTOR3(InitialMatrix._41, InitialMatrix._42, InitialMatrix._43);
	Parent = ParentBone;
	Num = setNum;
	Sid = setSid;
	animationMatrix=NULL;
}

D3DXMATRIX Bone::GetMatrixRecursively()
{
	D3DXMATRIX ret = InitialMatrix;

	// Si on a un parent, il faut multiplier la matrice par la matrice parent.
	if (Parent != NULL)
		ret *= Parent->GetMatrixRecursively();

	return ret;
}

Anim::~Anim()
{
	delete [] m_BoneArrayOrder;
	
	std::list< Bone* >::iterator iter = m_Bones.begin() ;
	while ( iter != m_Bones.end() ) 
	{
		delete *iter;
		++iter;	
	} 
	m_Bones.clear();


}

ResourceResult Anim::Load(std::string resource , ResourceParam param)
{
	std::string str="..\\..\\data\\anim\\"+resource;

	AnimLoader animLoader; 
	animLoader.Load(str.c_str(), m_Bones, m_BindShape, m_BoneArrayOrder, m_NbFrames);

	CalculateAbsoluteBoneMatrices();
	return RES_SUCCEED;
}

void Anim::CalculateAbsoluteBoneMatrices()
{
	std::list< Bone* >::iterator it=m_Bones.begin();

	while( it != m_Bones.end() )
	{
		(*it)->FinalMatrix=(*it)->GetMatrixRecursively();
		(*it)->Position = D3DXVECTOR3((*it)->FinalMatrix._41, (*it)->FinalMatrix._42, (*it)->FinalMatrix._43); 
		++it;
	}
}