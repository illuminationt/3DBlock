uniform extern float g_time;

uniform extern float4x4 g_WVP;
uniform extern float3 g_dirToSunW;
uniform extern texture g_tex;


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
//shadeÇ™ëÂÇ´Ç¢Å®ñæÇÈÇ¢

OutputVS TerrainVS(float3 posW : POSITION0,
                   float3 normalW : NORMAL0,
                   float2 tex : TEXCOORD0)
{
    OutputVS outVS = (OutputVS) 0;

    outVS.posH = mul(float4(posW, 1.f), g_WVP);

    outVS.texC = tex;

    outVS.shade = saturate(max(0.f, dot(normalW, g_dirToSunW)) + 0.3f);

    return outVS;
}


float4 TerrainPS(float2 tex:TEXCOORD0,
                 float shade : TEXCOORD1) : COLOR
{

    float3 c = tex2D(texS, tex).rgb;
    float3 d = float3(0.3f, 0.3f, 0.3f);
    return float4(c*shade, 1.f);

}



Technique TerrainTech
{
    pass P0
    {
        VertexShader = compile vs_2_0 TerrainVS();
        PixelShader = compile ps_2_0 TerrainPS();

    }
}

