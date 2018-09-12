uniform extern float4x4 g_world;
uniform extern float4x4 g_proj;
uniform extern texture g_tex;

uniform extern float2 g_pictureSize;//回転のためのピボット座標を得るために必要
uniform extern float2 g_UVsize;//UV空間内でのサイズ
uniform extern float2 g_UVoffset;
uniform extern float4 g_color;

sampler texS = sampler_state
{
    texture = <g_tex>;
};


struct OutVS
{
    float4 posH : POSITION0;
    float2 texUV : TEXCOORD0;
};


OutVS imageVS(float3 posL : POSITION0,
              float2 uv:TEXCOORD0)
{
    OutVS outVS = (OutVS) 0;
    //ピボット分移動
    posL -= float3(g_pictureSize.x / 2.f, g_pictureSize.y / 2.f, 0.f);
    outVS.posH = mul(float4(posL, 1.f), g_world);
    outVS.posH += float4(g_pictureSize.x / 2.f, g_pictureSize.y / 2.f, 0.f,0.f);
    outVS.posH= mul(outVS.posH, g_proj);
    outVS.texUV = uv * g_UVsize + g_UVoffset;
    return outVS;
}



float4 imagePS(float2 uv : TEXCOORD0) : COLOR0
{
    float4 c = tex2D(texS, uv);
    c *= g_color;
    
    return c;

}


Technique tech
{
    pass P0
    {
        VertexShader = compile vs_2_0 imageVS();
        PixelShader = compile ps_2_0 imagePS();
/*
        AlphaBlendEnable = true;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;
*/
    }
}