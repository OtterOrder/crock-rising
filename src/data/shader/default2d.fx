
//******************************************************************
// Variables uniformes
//******************************************************************
//float4x4 g_mWorld;                  // World matrix
//float4x4 g_mWorldViewProjection;    // World * View * Projection matrix
//matrix	 g_mWorldView;				// World * View matrix
//matrix	 g_mView;					// View matrix

float	g_Opacity;					// Opacité de l'objet

bool	g_IsTextured = false;		// Si l'objet est texturé
texture	g_Texture;					// Texture

//******************************************************************
// Texture samplers
//******************************************************************
sampler TextureSampler =
sampler_state
{
	Texture		= <g_Texture>;
	AddressU	= WRAP;
	AddressV	= WRAP;
	MipFilter	= LINEAR;
	MinFilter	= LINEAR;
	MagFilter	= LINEAR;
};

//******************************************************************
// Sortie vertex shader
//******************************************************************
struct VS_OUTPUT
{
	float4 position	: POSITION;
	float2 texCoord	: TEXCOORD0;
	float4 color	: TEXCOORD1;
};

//******************************************************************
// Ce shader ne fait rien.
// @param[in]	position	: Position (déjà dans le repère view)
// @param[in]	texCoord	: Coordonnées de texture
// @param[in]	color		: Colour de point
// @return	VS_OUTPUT
//******************************************************************
VS_OUTPUT RenderSceneVS( float4 vPosition	: POSITION,
						float2 texCoord		: TEXCOORD0,
						float4 color		: COLOR0 )
{
	VS_OUTPUT OUT;
    
    OUT.position	= vPosition;
    OUT.texCoord	= texCoord;
    OUT.color		= color;
    
    return OUT;
}

//******************************************************************
// Gère l'opacité.
//******************************************************************
float4 RenderScenePS( VS_OUTPUT IN ) : COLOR0
{ 
    float4 colorOut;
    
    colorOut = ( g_IsTextured )
		? tex2D( TextureSampler, IN.texCoord.xy )
		: IN.color;
		
	colorOut.a *= g_Opacity;
	return colorOut;
}

//******************************************************************
// Techniques
//******************************************************************
technique RenderScene
{
	pass P0
	{
		VertexShader	= compile vs_3_0 RenderSceneVS();
		PixelShader		= compile ps_3_0 RenderScenePS();
	}
}
