#include	"PostEffect.h"

#include	"Resources/ResourceManager.h"
#include	<assert.h>

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
PostEffect::PostEffect(void)
{
	m_pShader = NULL;
}

//----------------------------------------------------------------------------------------------
PostEffect::~PostEffect(void)
{
	if (m_pShader)
	{
		delete m_pShader;
		m_pShader = NULL;
	}
}

//----------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------
void PostEffect::SetShader (string _shaderFileName)
{
	m_pShader = ResourceManager::GetInstance()->Load<Shader>(_shaderFileName);
	assert(m_pShader);
}