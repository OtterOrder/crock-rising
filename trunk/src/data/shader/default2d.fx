
//******************************************************************
// Variables uniformes
//******************************************************************
float4	g_Color;					// Couleur de l'objet

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
// Vertex shader.
// En 2D il est impossible/interdit d'en avoir un, la position
// étant déjà transformée..
//******************************************************************

//******************************************************************
// Entrée pixel shader
//******************************************************************
struct PS_INPUT
{
	float4 vPosition	: POSITION;
	float2 texCoord		: TEXCOORD0;
};

//******************************************************************
// Pixel shader.
// @param[in]	IN : VS_OUTPUT
// @return	Couleur du pixel
//******************************************************************
float4 PixelShader2D( PS_INPUT IN ) : COLOR0
{ 
    float4 colorOut = g_Color;
    
    if( g_IsTextured )
    {
		colorOut *= tex2D( TextureSampler, IN.texCoord );
    }
    
    return colorOut;
}

//******************************************************************
// Techniques
//******************************************************************
technique RenderScene
{
	pass P0
	{
		PixelShader = compile ps_3_0 PixelShader2D();
	}
}
