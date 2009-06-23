//===========================================================================//
// Include                                                                   //
//===========================================================================//
#include "AnimLoader.h"



ResourceResult AnimLoader::Load(const char* sAnimPath, std::vector < Bone* > &Bones, D3DXMATRIX &BindShape, int * &ArrayOrder, int &Nbframes)
{

	TiXmlDocument animFile( sAnimPath );

	if (!animFile.LoadFile()) 
		return RES_FAILED ; 

	TiXmlNode *rootNode ; 
	rootNode = animFile.FirstChild("COLLADA"); // Get root node

	// Chargement des bones (hiérarchie, position)
	TiXmlElement* Node = rootNode->FirstChildElement("library_visual_scenes");
	TiXmlElement* Childnode = Node->FirstChildElement("visual_scene");
	Childnode=Childnode->FirstChildElement("node");
	if(Childnode->FirstChildElement("node")==NULL)
		Childnode=Childnode->NextSiblingElement();
	LoadBones(Bones, Childnode, NULL);

	// Chargement bind matrices (bind shape, inv bind pose)
	LoadBindMatrices(Bones, rootNode, BindShape, ArrayOrder);

	// Chargement animation (transformation des bones)
	Node = rootNode->FirstChildElement("library_animations");
	LoadAnimation(Bones, Node);

	Nbframes=m_Nbframes;

	// On remplit les animation des bones immobiles
	FillEmptyAnimation(Bones);

	return RES_SUCCEED;
}


ResourceResult AnimLoader::LoadBones(std::vector<Bone*> &Bones, TiXmlElement *rootNode, Bone * parent)
{
	TiXmlElement* Node=NULL;
	TiXmlElement* Childnode=NULL; 
	TiXmlElement* Nextnode=NULL; 
	std::string sid, type;

	Childnode=rootNode;

	if (rootNode) 
	{

		// On récupère les information du bone
		if(Childnode)
		{
			sid=Childnode->Attribute("id");
			type=Childnode->Attribute("type");
			if(type=="JOINT" && Childnode->Attribute("sid")!=NULL)
			{
				FillNodeBone(Bones, Childnode, parent);
				parent=Bones.back();
			}
		}

		Childnode=Childnode->FirstChildElement("node");

		// On parcourt tout ses enfants
		while(Childnode)
		{
			LoadBones(Bones, Childnode, parent);
			Childnode=Childnode->NextSiblingElement("node");			
		}

	}


	return RES_SUCCEED;
}

ResourceResult AnimLoader::FillNodeBone(std::vector < Bone *> &Bones, TiXmlElement *boneNode, Bone * ParentBone)
{
	const char* StringArray;
	D3DXMATRIX matrix;
	Bone *parentBone=NULL;

	std::string sid=boneNode->ToElement()->Attribute("sid");
	std::string id=boneNode->ToElement()->Attribute("id");
	TiXmlElement *Node=boneNode->FirstChildElement("matrix");

	if(Node)
	{
		StringArray = Node->ToElement()->GetText();
		float * fArray=new float[16];
		ConvertStringToFloatArray(StringArray, fArray, 16);
		Bone * bone = new Bone(LoadMatrix(fArray, 0), ParentBone, (int)Bones.size(), sid );
		bone->id=id;
		delete [] fArray;
		Bones.push_back(bone);

		if(ParentBone!=NULL)
			ParentBone->Children.push_back(bone);
	}

	
	return RES_SUCCEED;

}


ResourceResult AnimLoader::LoadBindMatrices(std::vector<Bone*> &Bones, TiXmlNode *rootNode, D3DXMATRIX &BindShape, int * &ArrayOrder)
{
	TiXmlElement* Node = rootNode->FirstChildElement("library_controllers");
	Node = Node->FirstChildElement("controller");
	std::string controlid=Node->Attribute("id");

	while(((int)controlid.find("skin"))<=0)
	{
		Node=Node->NextSiblingElement();
		controlid=Node->Attribute("id");
	}
	Node = Node->FirstChildElement("skin");
	Node = Node->FirstChildElement("bind_shape_matrix");

	// On récupère la bind shape matrix
	const char* StringArray=Node->ToElement()->GetText();
	float * fArray=new float[16];
	ConvertStringToFloatArray(StringArray, fArray, 16);
	BindShape=LoadMatrix(fArray, 0);
	delete [] fArray;
	

	Node = Node->NextSiblingElement("source");
	TiXmlElement* ChildNode = Node->FirstChildElement("Name_array");
	std::string stringArray=ChildNode->ToElement()->GetText();

	// On récupère l'ordre des bones (qui peut etre différente de la notre)
	ArrayOrder = new int[Bones.size()];
	int found;
	int arrayIndex=0;
	std::string strName;
	found = (int)stringArray.find_first_of(" ");

	for(int i=0; i<(int)Bones.size(); i++)
	{
		strName=stringArray.substr(0,found);

		// Pour chaque bone
		std::vector< Bone* >::iterator it=Bones.begin();
		while( it != Bones.end() )
		{
			if((*it)->Sid==strName)
				ArrayOrder[arrayIndex]=(*it)->Num;

			++it;
		}
		arrayIndex++;

		stringArray = stringArray.substr(found+1);
		found = (int)stringArray.find_first_of(" ");
	}

	// On récupère les inv bind pose matrix
	Node = Node->NextSiblingElement("source");
	std::string id=Node->Attribute("id");
	if(id.find("bind_poses")>=0)
	{
		ChildNode = Node->FirstChildElement("float_array");
		const char * charArray=ChildNode->ToElement()->GetText();
		fArray=new float[16*Bones.size()];
		ConvertStringToFloatArray(charArray, fArray, 16*(int)Bones.size());
		std::vector< Bone* >::iterator it;
		
		for(int i=0; i<(int)Bones.size(); i++)
		{
			it=Bones.begin();
			std::advance(it, ArrayOrder[i]);
			(*it)->invBoneSkinMatrix=LoadMatrix(fArray, i*16);
		}
	}

	delete [] fArray;


	return RES_SUCCEED;
}


ResourceResult AnimLoader::LoadAnimation(std::vector<Bone*> &Bones, TiXmlNode *rootNode)
{

	TiXmlElement* Node = rootNode->ToElement();
	Node = Node->FirstChildElement();

	while(Node)
	{
		std::string Value=Node->Value();
		if(Value=="animation")
		{
			LoadAnimation(Bones, Node);
			Node = Node->NextSiblingElement();
			continue;
		}
		if(Value!="channel")
		{
			Node = Node->NextSiblingElement();
			continue;
		}

		LoadBoneAnimation(Bones, rootNode, Node);
		break;
		
	}

	return RES_SUCCEED;
}

ResourceResult AnimLoader::LoadBoneAnimation(std::vector < Bone * > &Bones, TiXmlNode *rootNode, TiXmlElement* Node)
{

	std::string AnimationName=rootNode->ToElement()->Attribute("id");
	int pos= (int)AnimationName.find("-transform");

	int taille=(int)AnimationName.size()-pos;
	AnimationName=AnimationName.substr(0, AnimationName.size()-taille);

	std::string samplername=Node->Attribute("source");
	samplername=samplername.substr(1);
	std::string targetname = Node->Attribute("target");

	// On cherche le Sampler
	TiXmlElement* SamplerNode=rootNode->FirstChildElement("source");
	std::string sourceid=SamplerNode->Attribute("id");

	while(sourceid!=samplername)
	{
		SamplerNode=SamplerNode->NextSiblingElement();
		sourceid=SamplerNode->Attribute("id");
	}

	SamplerNode=SamplerNode->FirstChildElement("input");
	sourceid=SamplerNode->Attribute("semantic");
	while(sourceid!="OUTPUT")
	{
		SamplerNode=SamplerNode->NextSiblingElement();
		sourceid=SamplerNode->Attribute("semantic");
	}
	std::string valuename=SamplerNode->Attribute("source");
	valuename=valuename.substr(1);

	// On cherche la liste des matrices pour l'animation
	TiXmlElement* OutputNode=rootNode->FirstChildElement("source");
	std::string idSrc=OutputNode->Attribute("id");
	while(idSrc!=valuename)
	{
		OutputNode=OutputNode->NextSiblingElement();
		idSrc=OutputNode->Attribute("id");
	}

	OutputNode=OutputNode->FirstChildElement("float_array");
	int count=atoi(OutputNode->Attribute("count"));
	m_Nbframes=count/16;
	const char * charArray=OutputNode->ToElement()->GetText();
	float * Array=new float[count];
	ConvertStringToFloatArray(charArray, Array, count);

	std::vector< Bone* >::iterator it=Bones.begin();

	while( it != Bones.end() )
	{
		if(((*it)->Sid==AnimationName) || ((*it)->id==AnimationName))
		{
			(*it)->animationMatrix=new D3DXMATRIX[m_Nbframes];
			for(int i=0; i<m_Nbframes; i++)
			{
				(*it)->animationMatrix[i]=LoadMatrix(Array, i*16);
			}
		}
		++it;
	}

	delete [] Array;

	return RES_SUCCEED;
}

ResourceResult AnimLoader::FillEmptyAnimation(std::vector < Bone * > &Bones)
{

	std::vector< Bone* >::iterator it=Bones.begin();
	while( it != Bones.end() )
	{
		if((*it)->animationMatrix==NULL)
		{
			(*it)->animationMatrix=new D3DXMATRIX[m_Nbframes];
			for(int i=0; i<m_Nbframes; i++)
			{
				(*it)->animationMatrix[i]=(*it)->InitialMatrix;
			}
		}

		++it;
	}

	return RES_SUCCEED;

}


void ConvertStringToFloatArray(const char * Array, float * FloatArray, int iCount)
{
	int id=0;
	int dec=0;

	float result	= 0;
	float signe		= 1;
	float mantisse	= 0.1f;

	for (int i = 0 ; i < iCount ; i++)
	{
		while(Array[id+dec] != ' ' && Array[id+dec] != '\0')
		{			
			if(Array[id]!= '.')
			{
				if(Array[id]== '-')
					signe=-1;	
				else
					result=Array[id]-'0'+result*10;	
				id++;
			}
			else
			{	
				dec++;	
				result=((Array[id+dec]-'0')*mantisse)+result;
				mantisse *= 0.1f;

				if(Array[id+dec+1] == ' ')
					dec++;

			}
		}
		FloatArray[i]=result*signe;
		id=id+dec+1;
		dec=0, result=0, mantisse=0.1f, signe=1;
	}

}

D3DXMATRIX LoadMatrix(float * FloatArray, int offset)
{
	return D3DXMATRIX
	(
		FloatArray[offset + 0], FloatArray[offset + 4], FloatArray[offset + 8], FloatArray[offset + 12],
		FloatArray[offset + 1], FloatArray[offset + 5], FloatArray[offset + 9], FloatArray[offset + 13],
		FloatArray[offset + 2], FloatArray[offset + 6], FloatArray[offset + 10], FloatArray[offset + 14],
		FloatArray[offset + 3], FloatArray[offset + 7], FloatArray[offset + 11], FloatArray[offset + 15]
	
	);
}