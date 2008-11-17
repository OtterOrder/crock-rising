#include "Mesh.h"

//******************************************************************

/***********************************************************
 * Constructeur.
 **********************************************************/
Mesh::Mesh()
{
	m_pDevice = NULL;

	m_iNbVertices = 0;

	m_Positions	= NULL;
	m_Normals	= NULL;
	m_TexCoords	= NULL;


	m_pVB = NULL;
	m_pIB = NULL;
}

/***********************************************************
 * Destructeur.
 **********************************************************/
Mesh::~Mesh()
{
}


/***********************************************************
 * Supprime la ressource.
 * @param[in]	resource : crc32 de la ressource
 * @return	le r�sultat du chargement
 **********************************************************/
ResourceResult Mesh::Load(crc32 resource)
{			
	m_pDevice = NULL;//Renderer::GetDevice();
	//Renderer::GetInstance()->m_pd3dDevice;

	char strMeshName[128];
	sprintf(strMeshName, "..\\..\\data\\test\\%u.DAE", resource);

	TiXmlDocument meshFile( strMeshName );
	if (!meshFile.LoadFile ())
		return RES_FAILED;

	TiXmlNode* rootNode;

	rootNode =  meshFile.FirstChildElement( "COLLADA" );  // Get Root Node

	FillArrays (rootNode);

	return RES_SUCCEED;
}


/***********************************************************
 * Rempli les tableaux de donn�es.
 * @param[in]	rootNode : balise racine du fichier XML
 * @return	le r�sultat du chargement
 **********************************************************/
ResourceResult	 Mesh::FillArrays	(TiXmlNode* rootNode)
{
	TiXmlNode* node;

	if(rootNode) {
		node =  rootNode->FirstChildElement( "library_geometries" );
		if(node) {
			node =  node->FirstChildElement( "geometry" );
			if(node) {
				node =  node->FirstChildElement( "mesh" );
				if(node) {
					node =  node->FirstChildElement( "source" );		// Recherche la premi�re balise "source"
					if(node) {
						//node->ToElement()->Attribute("id");		// v�rification de l'id
						ExtractArrayDatas (node, m_Normals);
					}
					node = node->NextSibling( "source" );				// Recherche la prochaine balise "source"
					node = node->NextSibling( "source" );				// Recherche la prochaine balise "source"
					if(node) {
						ExtractArrayDatas (node, m_TexCoords);
					}
				}
			}
		}
	}

	return RES_FAILED;
}

/***********************************************************
 * Etrait les donn�es l�es � un tableau.
 * @param[in]	sourceNode : balise source (source des donn�es) du fichier XML
 *				Array : tableau � remplir
 * @return	le r�sultat du chargement
 **********************************************************/
ResourceResult	 Mesh::ExtractArrayDatas	(TiXmlNode* sourceNode, float** Array)
{
	TiXmlNode* node = sourceNode;			// Enregistrement de la balise source
	TiXmlString sTemp;

	char* text;

	int iStride = 0;


	node =  node->FirstChildElement( "technique_common" );
	if(node) {
		node =  node->FirstChildElement( "accessor" );
		if(node) {
			if (m_iNbVertices == 0)		// si le nombre de vertices n'a pas encore �t� obtenu
			{
				sTemp = node->ToElement()->Attribute("count");
				m_iNbVertices = atoi(sTemp.c_str());			// Stockage du nombre de vertices
			}

			sTemp = node->ToElement()->Attribute("stride");
			iStride = atoi(sTemp.c_str());					// Stockage de la taille du tableau

			if (!Array && m_iNbVertices && iStride)			// Si le tableau n'a pas �t� allou� et que le nombre de vertices et la taille sont connus
			{
				Array = new float*[m_iNbVertices];				// Le tableau est allou�
				for (int i=0 ; i<m_iNbVertices ; i++)
					Array[i] = new float[iStride];
			}
		}
	}					

	node = sourceNode;
	node =  node->FirstChildElement( "float_array" );
	if(node) {
		text = (char*)node->ToElement()->GetText();

		ConvertTextToArray(text, Array, m_iNbVertices, iStride);

		//delete text;
	}

	return RES_FAILED;
}

/***********************************************************
 * Copie les donn�es d'une cha�ne caract�res provenant du fichier XML dans un tableau
 * @param[in]	text	: donn�es sous forme de caract�res
 *				Array	: tableau � remplir
 *				iCount	: nombre de cases du tableau
 *				iStride	: nombre de donn�es par case
 * @return	le r�sultat du chargement
 **********************************************************/
ResourceResult	 Mesh::ConvertTextToArray	(char* text, float** Array, int iCount, int iStride)
{
	char* cData = new char[128];

	int iStartData = 0;
	int iDataSize = 0;

	for (int i = 0 ; i < iCount ; i++)
	{
		for (int j = 0 ; j < iStride ; j++)
		{
			iDataSize = 0;
			while (text[iStartData+iDataSize] != ' ')
				iDataSize ++;

			memcpy((void*)cData, (void*)(text+iStartData), (iStartData+iDataSize)*sizeof(char));

			cData[iDataSize] = '\0';

			Array[i][j] = (float)atof(cData);

			iStartData += iDataSize+1;
		}
	}

	//delete cData;

	return RES_FAILED;
}