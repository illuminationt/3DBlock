uniform extern float g_time;

uniform extern float4x4 g_WVP;
uniform extern float3 g_pointLightPosW;
uniform extern float3 g_pointLightPosW2;
uniform extern float3 g_pointLightPosW3;
//ポイントライト1,2,3の減衰率がx,y,z成分に入ってる
uniform extern float3 g_pointLightAttenuation;
uniform extern texture g_tex;

float toon(float c)
{
    float t = 0;
    if (c < 0.3f)
    {
        t = 0.15f;
    }
    else if (c < 0.7f)
    {
        t = 0.55f;
    }
    else
    {
    
        t = 0.85f;
    }
    return t;
}

sampler texS = sampler_state
{
    Texture = <g_tex>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};



struct OutputVS
{
    float4 posH : POSITION0;
    float2 texC : TEXCOORD0;
    float shade : TEXCOORD1;
};
//shadeが大きい→明るい

OutputVS TerrainVS(float3 posW : POSITION0,
                   float3 normalW : NORMAL0,
                   float2 tex : TEXCOORD0)
{
    OutputVS outVS = (OutputVS) 0;

    float3 lightVec = g_pointLightPosW - posW;
    float3 lightVec2 = g_pointLightPosW2 - posW;
    float3 lightVec3 = g_pointLightPosW3 - posW;
    float distance = length(lightVec);
    float distance2 = length(lightVec2);
    float distance3 = length(lightVec3);

    outVS.posH = mul(float4(posW, 1.f), g_WVP);

    outVS.texC = tex;

    float shade = saturate(max(0.f, dot(normalW, lightVec)) + 0.3f);
    float shade2 = saturate(max(0.f, dot(normalW, lightVec2)) + 0.3f);
    float shade3 = saturate(max(0.f, dot(normalW, lightVec3)) + 0.3f);
    
    shade /= (0.3f+distance * g_pointLightAttenuation.x);
    shade2 /= (0.3f+distance2 * g_pointLightAttenuation.y);
    shade3 /= (0.3f+distance3 * g_pointLightAttenuation.z);

    outVS.shade = shade + shade2 + shade3;
   
	
	return outVS;
}


float4 TerrainPS(float2 tex : TEXCOORD0,
                 float shade : TEXCOORD1) : COLOR
{

    float3 c = tex2D(texS, tex).rgb;
   
    c *= shade;
    /*
c.r = toon(c.r);
    c.g = toon(c.g);
    c.b = toon(c.b);
*/
   
    return float4(c * shade, 1.f);

}



Technique TerrainTech
{
    pass P0
    {
        VertexShader = compile vs_2_0 TerrainVS();
        PixelShader = compile ps_2_0 TerrainPS();

    }
}

