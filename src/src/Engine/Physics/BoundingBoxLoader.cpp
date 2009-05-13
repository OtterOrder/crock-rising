#include "BoundingBoxLoader.h"


ResourceResult BoundingBoxLoader::Load(const std::string resource)
{

	std::string physXFilePath = "..\\..\\data\\models\\"+resource;
	TiXmlDocument physXFile( physXFilePath.c_str());

	if(!physXFile.LoadFile())
		return RES_FAILED;
	
	TiXmlNode *rootNode, *saveNode;
	rootNode = physXFile.FirstChild("COLLADA");
		
	TiXmlNode *node = rootNode;
	node = node->FirstChildElement("library_physics_models");
	if(node)
	{
		node = node->FirstChild("physics_model");
		if (node)
		{
			node = node->FirstChild("rigid_body");
			if (node)
			{
				//On compte le nombre de rigid_body
				TiXmlNode *rigidBodyNode = node;
				int nbRigidBody = 1; 
				while ( rigidBodyNode = rigidBodyNode->NextSibling("rigid_body") )
					nbRigidBody++;

				while ( nbRigidBody > 0)
				{
					saveNode = node;
					node = node->FirstChild("technique_common");
					if(node)
					{
						//Première vérification importante à effectuer, tester si l'objet est "dynamic" ou non
						TiXmlNode* saveNode = node; //Sauvegarde du noeud
						node = node->FirstChild("dynamic");
						if (node)
						{
						   const char* pTypeObject = node->FirstChild()->Value();
						   if ( strcmp(pTypeObject,"true") == 0) 
						   {
							   m_vDynamicBody.push_back(new DynamicBody());
							   fillDynamicObject(saveNode);
						   }
						   else
						   {
							   m_vStaticBody.push_back(new StaticBody());
							   fillStaticObject(saveNode);
						   }
						   
						}
					}
				    nbRigidBody--;
					node = saveNode;
					node = node->NextSibling("rigid_body");
				}
			}
		}
	}

	return RES_SUCCEED;
}


ResourceResult BoundingBoxLoader::fillDynamicObject( TiXmlNode* node )
{

	node = node->FirstChildElement("shape");
	TiXmlNode* saveNode = node;
	if (node)
	{
		//Translation 
		node = node->FirstChild("translate");
		if (node)
		{
			const char* pValues = node->ToElement()->GetText();
			float* pfValues = new float[3];
			ConvertStringToFloatArray(pValues,pfValues,3);
			m_vDynamicBody.at(m_vDynamicBody.size()-1)->translate = D3DXVECTOR3(pfValues[0],pfValues[1],pfValues[2]);

			//Rotation
			node = node->NextSibling("rotate");
			if (node)
			{
				pValues = node->ToElement()->GetText();
				ConvertStringToFloatArray(pValues,pfValues,3);
				m_vDynamicBody.at(m_vDynamicBody.size()-1)->rotate = D3DXVECTOR3(pfValues[0],pfValues[1],pfValues[2]);

				node = node->NextSibling("mass");
				if (node)
				{
					pValues = node->ToElement()->GetText();
					ConvertStringToFloatArray(pValues,pfValues,1);
					m_vDynamicBody.at(m_vDynamicBody.size()-1)->fMass = pfValues[0];
					
					//Type de la géométrie
					node = saveNode;
					node = node->FirstChild("extra");
					if (node)
					{
						node = node->NextSibling();
						if (node)
						{
							const char* pTypeShape = node->Value();

							if ( strcmp(pTypeShape,"box") == 0)
							{
								m_vDynamicBody.at(m_vDynamicBody.size()-1)->type = BOX;
								node = node->FirstChild();
								if (node)
								{ 
									//Dimension(s) de la "box"
									pValues = node->ToElement()->GetText();
									ConvertStringToFloatArray(pValues,pfValues,3);
									m_vDynamicBody.at(m_vDynamicBody.size()-1)->bodySize = D3DXVECTOR3(pfValues[0],pfValues[1],pfValues[2]);
								}
							}

							else if ( strcmp(pTypeShape,"sphere") == 0)
							{
								m_vDynamicBody.at(m_vDynamicBody.size()-1)->type = SPHERE;
								node = node->FirstChild();
								if (node)
								{ 
									//Dimension de la sphère
									//Attention ici sa dimension est son rayon. C'est bodySize.x qui contient sa valeur
									pValues = node->ToElement()->GetText();
									ConvertStringToFloatArray(pValues,pfValues,1);
									m_vDynamicBody.at(m_vDynamicBody.size()-1)->bodySize = D3DXVECTOR3(pfValues[0],0.f,0.f);
								}
							}

							else
							{
								m_vDynamicBody.at(m_vDynamicBody.size()-1)->type = CAPSULE;
								node = node->FirstChild();
								if (node)
								{ 
									//Dimension(s) de la capsule
									pValues = node->ToElement()->GetText();
									ConvertStringToFloatArray(pValues,pfValues,3);
									m_vDynamicBody.at(m_vDynamicBody.size()-1)->bodySize = D3DXVECTOR3(pfValues[0],pfValues[1],pfValues[2]);
								}
							}
						}
					}
				}
				
			}
			delete [] pfValues;
		}	 
	}

	node = saveNode;
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
					pValues = node->ToElement()->GetText();
					ConvertStringToFloatArray(pValues,&pfValues,1);
					m_vDynamicBody.at(m_vDynamicBody.size()-1)->fLinearDamping = pfValues;

					node = node->NextSibling();
					if (node)
					{
						pValues = node->ToElement()->GetText();
						ConvertStringToFloatArray(pValues,&pfValues,1);
						m_vDynamicBody.at(m_vDynamicBody.size()-1)->fAngularDamping = pfValues;
					}

					node = node->NextSibling("disable_collision");
					if (node)
					{
						(node->ToElement()->GetText() == "true") ? m_vDynamicBody.at(m_vDynamicBody.size()-1)->bDisableCollision = true 
							: m_vDynamicBody.at(m_vDynamicBody.size()-1)->bDisableCollision = false;
					}
				}
			}
		}
	}

	return RES_SUCCEED;
}


ResourceResult BoundingBoxLoader::fillStaticObject( TiXmlNode *node )
{
	node = node->FirstChildElement("shape");
	TiXmlNode* saveNode = node;
	if (node)
	{
		//Translation 
		 node = node->FirstChild("translate");
		 if (node)
		 {
			 const char* pValues = node->ToElement()->GetText();
			 float* pfValues = new float[3];
			 ConvertStringToFloatArray(pValues,pfValues,3);
			 m_vStaticBody.at(m_vStaticBody.size()-1)->translate = D3DXVECTOR3(pfValues[0],pfValues[1],pfValues[2]);

			 //Rotation
			 node = node->NextSibling("rotate");
			 if (node)
			 {
				pValues = node->ToElement()->GetText();
				ConvertStringToFloatArray(pValues,pfValues,3);
				m_vStaticBody.at(m_vStaticBody.size()-1)->rotate = D3DXVECTOR3(pfValues[0],pfValues[1],pfValues[2]);

				//Type de la géométrie
				node = saveNode;
				node = node->FirstChild("extra");
				if (node)
				{
					node = node->NextSibling();
					if (node)
					{
						const char* pTypeShape = node->Value();

						if ( strcmp(pTypeShape,"box") == 0)
						{
							m_vStaticBody.at(m_vStaticBody.size()-1)->type = BOX;
							node = node->FirstChild();
							if (node)
							{ 
								//Dimension(s) de la "box"
								pValues = node->ToElement()->GetText();
								ConvertStringToFloatArray(pValues,pfValues,3);
								m_vStaticBody.at(m_vStaticBody.size()-1)->bodySize = D3DXVECTOR3(pfValues[0],pfValues[1],pfValues[2]);
							}
						}

						else if ( strcmp(pTypeShape,"sphere") == 0)
						{
							m_vStaticBody.at(m_vStaticBody.size()-1)->type = SPHERE;
							node = node->FirstChild();
							if (node)
							{ 
								//Dimension de la sphère
								//Attention ici sa dimension est son rayon. C'est bodySize.x qui contient sa valeur
								pValues = node->ToElement()->GetText();
								ConvertStringToFloatArray(pValues,pfValues,1);
								m_vStaticBody.at(m_vStaticBody.size()-1)->bodySize = D3DXVECTOR3(pfValues[0],0.f,0.f);
							}
						}

						else
						{
							m_vStaticBody.at(m_vStaticBody.size()-1)->type = CAPSULE;
							node = node->FirstChild();
							if (node)
							{ 
								//Dimension(s) de la capsule
								pValues = node->ToElement()->GetText();
								ConvertStringToFloatArray(pValues,pfValues,3);
								m_vStaticBody.at(m_vStaticBody.size()-1)->bodySize = D3DXVECTOR3(pfValues[0],pfValues[1],pfValues[2]);
							}
						}
					}
				}
			 }
			 delete [] pfValues;
		 }	 
	}

	node = saveNode;
	node = node->Parent();  //On remonte à la balise <technique_common>
	if (node)
	{
		node = node->NextSibling("extra");
		if (node)
		{
			node = node->FirstChild("technique");
			if (node)
			{
				node = node->FirstChild("disable_collision");
				(node->ToElement()->GetText() == "true") ? m_vStaticBody.at(m_vStaticBody.size()-1)->bDisableCollision = true 
														 : m_vStaticBody.at(m_vStaticBody.size()-1)->bDisableCollision = false;
			}
		}
	}

	return RES_SUCCEED;
}
