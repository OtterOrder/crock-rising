#include "BoundingBoxLoader.h"


ResourceResult BoundingBoxLoader::Load(const std::string resource)
{

	std::string physXFilePath = "..\\..\\data\\models\\"+resource;
	TiXmlDocument physXFile( physXFilePath.c_str());
	NodeSaver NodeSave;

	if(!physXFile.LoadFile())
		return RES_FAILED;
	
	TiXmlNode *rootNode;
	rootNode = physXFile.FirstChild("COLLADA");
		
	TiXmlNode *node = rootNode;

	//Récupération préalable de la position.
	NodeSave.library_visual_scenes = node->FirstChildElement("library_visual_scenes");
	NodeSave.library_physics_materials = node->FirstChildElement("library_physics_materials");

	node = rootNode;
	node = node->FirstChildElement("library_physics_models");
	if(node)
	{
		node = node->FirstChild("physics_model");
		if (node)
		{
			NodeSave.physics_model = node;
			node = node->FirstChild("rigid_body");
			if (node)
			{
				//On compte le nombre de rigid_body
				TiXmlNode *rigidBodyNode = node;
				int nbRigidBody = 1; 
				while ( rigidBodyNode = rigidBodyNode->NextSibling("rigid_body") )
					nbRigidBody++;
				NodeSave.NumShape = 0;

				while ( nbRigidBody > 0)
				{
					TiXmlNode *rigidBodyNode_BeforeTechnique = node;
					node = node->FirstChild("technique_common");
					if(node)
					{
						NodeSave.RigidBodyNode = node;
						//Première vérification importante à effectuer, tester si l'objet est "dynamic" ou non
						node = node->FirstChild("dynamic");
						if (node)
						{
						   const char* pTypeObject = node->FirstChild()->Value();
						   if ( strcmp(pTypeObject,"true") == 0) 
						   {
							   m_vDynamicBody.push_back(new PhysicBody());
							   fillPhysicBody(NodeSave, true);
						   }
						   else
						   {
							   m_vDynamicBody.push_back(new PhysicBody());
							   fillPhysicBody(NodeSave, false);
						   }
						   
						}
					}
				    nbRigidBody--;
					NodeSave.NumShape++;
					node = rigidBodyNode_BeforeTechnique;
					node = node->NextSibling("rigid_body");
				}
			}
		}
	}

	return RES_SUCCEED;
}

ResourceResult BoundingBoxLoader::fillPhysicBody( NodeSaver NodeSave, bool Dyn )
{
	m_vDynamicBody.at(m_vDynamicBody.size()-1)->bDisableCollision = !Dyn;
	TiXmlNode* node = NodeSave.RigidBodyNode;
	node = node->FirstChildElement("shape");

	if( getSize(node)				!= RES_SUCCEED ) return RES_FAILED ;
	if( getMass(node)				!= RES_SUCCEED ) return RES_FAILED ; 
	if( getDampingAndType(node)		!= RES_SUCCEED ) return RES_FAILED ;
	if( getVelocity(NodeSave)		!= RES_SUCCEED ) return RES_FAILED ;
	if( getLocalPos(NodeSave)		!= RES_SUCCEED ) return RES_FAILED ;
	if( getMaterial(NodeSave)		!= RES_SUCCEED ) return RES_FAILED ;

	return RES_SUCCEED;
}


ResourceResult BoundingBoxLoader::getVelocity  ( NodeSaver NodeSave )
{	
	TiXmlNode* node;
	node = NodeSave.physics_model;
	node = node->NextSibling();
	if(node)
	{
		node = node->FirstChild("instance_physics_model");
		if (node)
		{
			node = node->FirstChild("instance_rigid_body");
			int i = 0;
			while(node && i < NodeSave.NumShape)
			{
				node = node->NextSibling();
				++i;
			}
			if (node)
			{
				node = node->FirstChild("technique_common");
				if (node)
				{
					node = node->FirstChild("angular_velocity");
					const char* pValues = node->ToElement()->GetText();
					float* pfValues = new float[3];
					if(node)
					{
						ConvertStringToFloatArrayV2(pValues,pfValues,3);
						m_vDynamicBody.at(m_vDynamicBody.size()-1)->angularVelocity = D3DXVECTOR3(pfValues[0],pfValues[1],pfValues[2]);
					}
					node = node->NextSibling("velocity");
					if(node)
					{
						ConvertStringToFloatArrayV2(pValues,pfValues,3);
						m_vDynamicBody.at(m_vDynamicBody.size()-1)->linearVelocity = D3DXVECTOR3(pfValues[0],pfValues[1],pfValues[2]);
						delete [] pfValues;
					}
				}
			}
		}
	}
	else return RES_FAILED;

	return RES_SUCCEED;
}


ResourceResult BoundingBoxLoader::getLocalPos( NodeSaver NodeSave )
{
	TiXmlNode* node;	
	node = NodeSave.library_visual_scenes;
	if(node)
	{
		node = node->FirstChildElement("visual_scene");
		if(node)
		{
			node = node->FirstChildElement("node");
			int i = 0;
			while(node && i < NodeSave.NumShape)
			{
				node = node->NextSibling();
				++i;
			}
		}
	}

	if(node)
	{
		const char* pValues;
		float* pfValues = new float[4];

		node = node->FirstChildElement("translate");
		if(node)
		{
			// TRANSLATE
			pValues = node->ToElement()->GetText();
			ConvertStringToFloatArrayV2(pValues,pfValues,3);
			m_vDynamicBody.at(m_vDynamicBody.size()-1)->translate = D3DXVECTOR3(pfValues[0],pfValues[1],pfValues[2]);

			node = node->NextSibling("rotate");
			if(node)
			{
				// ROTATION
				pValues = node->ToElement()->GetText();
				ConvertStringToFloatArrayV3(pValues,pfValues,4);
				NxQuat rotX(0.f, NxVec3(1.f, 0.f, 0.f)); //For Max
				NxQuat quat(pfValues[3], NxVec3(pfValues[0], pfValues[1], pfValues[2]));
				m_vDynamicBody.at(m_vDynamicBody.size()-1)->rotate = rotX * quat;
			}	
		}		
		delete [] pfValues;
	}
	else return RES_FAILED;

	return RES_SUCCEED;
}

ResourceResult BoundingBoxLoader::getMaterial( NodeSaver NodeSave )
{
	TiXmlNode* node = NodeSave.library_physics_materials;
	if(node)
	{
		node = node->FirstChildElement("physics_material");
		int i = -1;
		while(node && i < NodeSave.NumShape)
		{
			node = node->NextSibling();
			++i;
		}
	}

	if(node)
	{
		const char* pValues;
		float* pfValues = new float[4];

		node = node->FirstChildElement("technique_common");
		if(node)
		{
			node = node->FirstChildElement("dynamic_friction");
			if(node)
			{
				pValues = node->ToElement()->GetText();
				ConvertStringToFloatArrayV2(pValues,pfValues,1);
				m_vDynamicBody.at(m_vDynamicBody.size()-1)->dynamiqueFriction = pfValues[0];

				node = node->NextSibling("restitution");
				if(node)
				{
					pValues = node->ToElement()->GetText();
					ConvertStringToFloatArrayV2(pValues,pfValues,1);
					m_vDynamicBody.at(m_vDynamicBody.size()-1)->restitution = pfValues[0];

					node = node->NextSibling("static_friction");
					if(node)
					{
						pValues = node->ToElement()->GetText();
						ConvertStringToFloatArrayV2(pValues,pfValues,1);
						m_vDynamicBody.at(m_vDynamicBody.size()-1)->staticFriction = pfValues[0];
					}	
				}	
			}		
		}
		delete [] pfValues;
	}
	else return RES_FAILED;

	return RES_SUCCEED;

}

ResourceResult BoundingBoxLoader::getMass ( TiXmlNode* nodeShape )
{
	TiXmlNode* node = nodeShape;
	node = node->FirstChildElement("mass");
	if (node)
	{
		const char* pValues = node->ToElement()->GetText();
		float* pfValues = new float[3];
		pValues = node->ToElement()->GetText();
		ConvertStringToFloatArrayV2(pValues,pfValues,1);
		m_vDynamicBody.at(m_vDynamicBody.size()-1)->fMass = pfValues[0];
		delete [] pfValues;
	}	
	else return RES_FAILED;

	return RES_SUCCEED;

}
ResourceResult BoundingBoxLoader::getSize( TiXmlNode* nodeShape )
{
	TiXmlNode* node = nodeShape;
	node = node->FirstChild("extra");
	if (node)
	{
		node = node->NextSibling();
		if (node)
		{
			const char* pTypeShape = node->Value();
			const char* pValues = node->ToElement()->GetText();
			float* pfValues = new float[3];

			if ( strcmp(pTypeShape,"box") == 0)
			{
				m_vDynamicBody.at(m_vDynamicBody.size()-1)->type = BOX;
				node = node->FirstChild();
				if (node)
				{ 
					// BODYSIZE BOX
					pValues = node->ToElement()->GetText();
					ConvertStringToFloatArrayV2(pValues,pfValues,3);
					m_vDynamicBody.at(m_vDynamicBody.size()-1)->bodySize = D3DXVECTOR3(pfValues[0], pfValues[1], pfValues[2]);
				}
			}

			else if ( strcmp(pTypeShape,"sphere") == 0)
			{
				m_vDynamicBody.at(m_vDynamicBody.size()-1)->type = SPHERE;
				node = node->FirstChild();
				if (node)
				{ 
					// BODYSIZE SPHERE
					//Attention ici sa dimension est son rayon. C'est bodySize.x qui contient sa valeur
					pValues = node->ToElement()->GetText();
					ConvertStringToFloatArrayV2(pValues,pfValues,1);
					m_vDynamicBody.at(m_vDynamicBody.size()-1)->bodySize = D3DXVECTOR3(pfValues[0], 0.f, 0.f);
				}
			}

			else if ( strcmp(pTypeShape,"capsule") == 0)
			{
				m_vDynamicBody.at(m_vDynamicBody.size()-1)->type = CAPSULE;
				node = node->FirstChild();
				if (node)
				{ 
					// BODYSIZE CAPSULE
					//Dimension(s) de la capsule
					float radius, height;
					pValues = node->ToElement()->GetText();
					ConvertStringToFloatArrayV2(pValues,pfValues,3);
					radius = pfValues[0];
					node = node->NextSibling();
					pValues = node->ToElement()->GetText();
					ConvertStringToFloatArrayV2(pValues,pfValues,3);
					height = pfValues[0];
					m_vDynamicBody.at(m_vDynamicBody.size()-1)->bodySize = D3DXVECTOR3(radius, height, 0.0f);
				}
			}
			else
			{
				m_vDynamicBody.at(m_vDynamicBody.size()-1)->type = LOAD_ERROR;
			}
		}
	}
	else return RES_FAILED;

	return RES_SUCCEED;
}

ResourceResult BoundingBoxLoader::getDampingAndType	( TiXmlNode* nodeShape )
{
	TiXmlNode* node = nodeShape;
	node = node->Parent();  //On remonte à la balise <technique_common>
	if (node)
	{
		node = node->NextSibling("extra");
		if (node)
		{
			node = node->FirstChild("technique");
			if (node)
			{
				node = node->FirstChild("linearDamping");
				if (node)
				{
					const char* pValues = node->ToElement()->GetText();
					float pfValues;

					// LINEARDAMPING
					pValues = node->ToElement()->GetText();
					ConvertStringToFloatArrayV2(pValues,&pfValues,1);
					m_vDynamicBody.at(m_vDynamicBody.size()-1)->fLinearDamping = pfValues;

					node = node->NextSibling();
					if (node)
					{
						// ANGULARDAMPING
						pValues = node->ToElement()->GetText();
						ConvertStringToFloatArrayV2(pValues,&pfValues,1);
						m_vDynamicBody.at(m_vDynamicBody.size()-1)->fAngularDamping = pfValues;
					}

					//node = node->NextSibling("disable_collision");
					//if (node)
					//{
					//	// DYNAMIQUE / STATIC
					//	(node->ToElement()->GetText() == "true") ? m_vDynamicBody.at(m_vDynamicBody.size()-1)->bDisableCollision = true 
					//		: m_vDynamicBody.at(m_vDynamicBody.size()-1)->bDisableCollision = false;
					//}
				}
			}
		}
	}
	else return RES_FAILED;

	return RES_SUCCEED;
}



//La fonction V1 dans AnimLoader a quelques soucis avec l'exporter du plugin PhysX de max
//PROBLEME AVEC LES ENTIERS
void ConvertStringToFloatArrayV2(const char * Array, float * FloatArray, int iCount)
{
	ConvertStringToFloatArrayV3( Array, FloatArray, iCount);

	/*int id=0;
	int dec=0;

	float result	= 0;
	float signe		= 1;
	float mantisse	= 0.1f;
	bool IsFloat = false;
	bool stop = false;

	for (int i = 0 ; i < iCount ; i++)
	{
		while(Array[id+dec] != ' ' && Array[id+dec] != '\0' && !stop )
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

				IsFloat = true;
			}
			stop = Array[id+dec+1] == 0;
		}
		FloatArray[i]=result*signe;
		id=id+dec+1;
		dec=0, result=0, mantisse=0.1f, signe=1;
	}*/
}

void ConvertStringToFloatArrayV3( const char * Array, float * FloatArray, int iCount )
{
	float result = 0;
	int signe = 1;
	float mantisse = 0.1f;
	int i = 0, j = 0;
	while(Array[i] != 0) //Caractere de FIN
	{
		//Signe
		if( Array[i] == '-' )
		{
			signe = -1;
			++i;
		}

		//Partie entiere
		while(Array[i] != '.' && Array[i] != ' ' && Array[i] != 0)
		{
			result = result*10 + Array[i]-'0';
			++i;
		}
		if(Array[i] == '.') ++i;

		//Partie décimale
		while(Array[i] != ' ' && Array[i] != 0)
		{
			result = result + (Array[i]-'0')*mantisse;
			mantisse *= 0.1f;
			++i;
		}
		if(Array[i] != 0)++i;

		FloatArray[j++]= result != 0.0f ? result*signe : 0.0f; //Evite le -0.0
		result = 0;
		signe = 1;
		mantisse = 0.1f;
	}
}