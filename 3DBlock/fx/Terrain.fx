uniform extern float g_time;

uniform extern float4x4 g_WVP;
uniform extern float3 g_dirToSunW;
uniform extern texture g_texSand;
uniform extern texture g_texGrass;
uniform extern texture g_texStone;
uniform extern texture g_texBlendmap;
static float g_texScale = 32;


sampler texSand = sampler_state
{
    Texture = <g_texSand>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler texGrass = sampler_state
{
    Texture = <g_texGrass>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler texStone = sampler_state
{
    Texture = <g_texStone>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler texBlendmap = sampler_state
{
    Texture = <g_texBlendmap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

struct OutputVS
{
    float4 posH : POSITION0;
    float2 tiledTexC : TEXCOORD0;
    float2 nonTiledTexC : TEXCOORD1;
    float shade : TEXCOORD2;
};
//shadeが大きい→明るい

OutputVS TerrainVS(float3 posW : POSITION0,
                   float3 normalW : NORMAL0,
                   float2 tex : TEXCOORD0)
{
    OutputVS outVS = (OutputVS) 0;

    outVS.posH = mul(float4(posW, 1.f), g_WVP);

    outVS.tiledTexC = tex * g_texScale;
    outVS.nonTiledTexC = tex;

    outVS.shade = saturate(max(0.f, dot(normalW, g_dirToSunW)) + 0.3f);

    return outVS;
}


float4 TerrainPS(float2 tiledTexC : TEXCOORD0,
                 float2 nonTiledTexC : TEXCOORD1,
                 float shade : TEXCOORD2) : COLOR
{
    float3 cSand = tex2D(texSand, tiledTexC).rgb;
    float3 cGrass = tex2D(texGrass, tiledTexC).rgb;
    float3 cStone = tex2D(texStone, tiledTexC).rgb;

    float3 B = tex2D(texBlendmap, nonTiledTexC).rgb;

    float totalInverse = 1.f / (B.r + B.g + B.b);

    cSand *= B.g * totalInverse;
    cGrass *= B.r * totalInverse;
    cStone *= B.b * totalInverse;

    float3 final = cSand + cGrass + cStone;
    //final.gb -= float2(0.1f, 0.1f);
    final.g += 0.8f * sin(g_time);
    //final.b += 0.2f*cos(g_time);
    return float4(final * shade, 1.f);

}









Technique TerrainTech
{
    pass P0
    {
        VertexShader = compile vs_2_0 TerrainVS();
        PixelShader = compile ps_2_0 TerrainPS();

    }
}


//=============================================================================
// Terrain.fx by Frank Luna (C) 2004 All Rights Reserved.
//
// Blends three textures together with a blend map.
//=============================================================================

/*
uniform extern float4x4 g_WVP;
uniform extern float3 g_dirToSunW;
uniform extern texture g_texSand;
uniform extern texture g_texGrass;
uniform extern texture g_texStone;
uniform extern texture g_texBlendmap;

static float gTexScale = 24.0f;

sampler Tex0S = sampler_state
{
    Texture = <g_texSand>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler Tex1S = sampler_state
{
    Texture = <g_texGrass>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler Tex2S = sampler_state
{
    Texture = <g_texStone>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

sampler BlendMapS = sampler_state
{
    Texture = <g_texBlendmap>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};
 
struct OutputVS
{
    float4 posH : POSITION0;
    float2 tiledTexC : TEXCOORD0;
    float2 nonTiledTexC : TEXCOORD1;
    float shade : TEXCOORD2;
};

OutputVS TerrainVS(float3 posW : POSITION0, // We assume terrain geometry is specified
                   float3 normalW : NORMAL0, // directly in world space.
                   float2 tex0 : TEXCOORD0)
{
    // Zero out our output.
    OutputVS outVS = (OutputVS) 0;
	
	// Just compute a grayscale diffuse and ambient lighting 
	// term--terrain has no specular reflectance.  The color 
	// comes from the texture.
    outVS.shade = saturate(max(0.0f, dot(normalW, g_dirToSunW)) + 0.3f);
    
	// Transform to homogeneous clip space.
    outVS.posH = mul(float4(posW, 1.0f), g_WVP);
	
	// Pass on texture coordinates to be interpolated in rasterization.
    outVS.tiledTexC = tex0 * gTexScale; // Scale tex-coord to tile.
    outVS.nonTiledTexC = tex0; // Blend map not tiled.
	
	// Done--return the output.
    return outVS;
}

float4 TerrainPS(float2 tiledTexC : TEXCOORD0,
                 float2 nonTiledTexC : TEXCOORD1,
                 float shade : TEXCOORD2) : COLOR
{
	// Layer maps are tiled
    float3 c0 = tex2D(Tex0S, tiledTexC).rgb;
    float3 c1 = tex2D(Tex1S, tiledTexC).rgb;
    float3 c2 = tex2D(Tex2S, tiledTexC).rgb;
    
    // Blendmap is not tiled.
    float3 B = tex2D(BlendMapS, nonTiledTexC).rgb;

	// Find the inverse of all the blend weights so that we can
	// scale the total color to the range [0, 1].
    float totalInverse = 1.0f / (B.r + B.g + B.b);
    
    // Scale the colors by each layer by its corresponding weight
    // stored in the blendmap.  
    c0 *= B.r * totalInverse;
    c1 *= B.g * totalInverse;
    c2 *= B.b * totalInverse;
    
    // Sum the colors and modulate with the shade to brighten/darken.
    float3 final = (c0 + c1 + c2) * shade;
    
    return float4(final, 1.0f);
}

technique TerrainTech
{
    pass P0
    {
        vertexShader = compile vs_2_0 TerrainVS();
        pixelShader = compile ps_2_0 TerrainPS();
    }
}

*/

/*
uniform extern float4x4 g_world;
uniform extern float4x4 g_WVP;
uniform extern float4x4 g_worldInvTranspose;



uniform extern float4 g_diffuseMtrl;
uniform extern float4 g_diffuseLight;
uniform extern float4 g_ambientMtrl;
uniform extern float4 g_ambientLight;
uniform extern float4 g_specularMtrl;
uniform extern float4 g_specularLight;
uniform extern float g_specularPower;
uniform extern float3 g_lightVec;

uniform extern float3 g_eyePosW; //カメラのワールド座標

struct OutputVS
{
    float4 posH : POSITION0;
    float4 color : COLOR0;
};

//diffuse , ambient , specularの順で計算します

OutputVS transformVS(float3 posL : POSITION0,
	float3 normalL : NORMAL)
{
    OutputVS outVS = (OutputVS) 0;
	//頂点座標をスクリーン座標に
    outVS.posH = mul(float4(posL, 1.f), g_WVP);

    float3 normalW = mul(float4(normalL, 0.f), g_worldInvTranspose).xyz;
    normalW = normalize(normalW);

	//diffuse : mtrl*lightColor
    float4 diffuse = g_diffuseMtrl * g_diffuseLight;
    float4 ratio = max(dot(normalW, g_lightVec), 0);
    diffuse *= ratio;

	//ambient
    float4 ambient = g_ambientMtrl * g_ambientLight;


	//specular
	//頂点座標をワールド座標に
    float3 posW = mul(float4(posL, 1.f), g_world).xyz;
    float3 toEye = normalize(g_eyePosW - posW);
    float3 r = reflect(-g_lightVec, normalW);

    float power = pow(max(dot(toEye, r), 0.f), g_specularPower);
    float4 specular = power * (g_specularMtrl * g_specularLight);

    outVS.color.rgb = diffuse.rgb + ambient.rgb + specular.rgb;
    outVS.color.a = g_diffuseMtrl.a;

    return outVS;
}

float4 transformPS(float4 c : COLOR0) : COLOR
{
    return c;
}


technique Tech
{
    pass P0
    {

        vertexShader = compile vs_2_0 transformVS();
        pixelShader = compile ps_2_0 transformPS();

        FillMode = Solid;
    }
}



*/