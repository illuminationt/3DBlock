uniform extern float4 g_diffuseLight;
uniform extern float4 g_ambientLight;
uniform extern float4 g_specularLight;

uniform extern float4 g_diffuseMtrl;
uniform extern float4 g_ambientMtrl;
uniform extern float4 g_specularMtrl;
uniform extern float g_specularPower;

uniform extern float3 g_pointLightPosW;
uniform extern float3 g_pointLightPosW2;
uniform extern float3 g_pointLightPosW3;
uniform extern float3 g_pointLightAttenuation;
uniform extern float3 g_specularAttenuation;
uniform extern float3 g_eyePosW;

uniform extern texture g_tex;

uniform extern float4x4 g_world;
uniform extern float4x4 g_WVP;
uniform extern float4x4 g_worldInvTranspose;

float toon(float c)
{
    float x,y = 0;
    x = max(0.85f, c);
    y = min(0.45f, c);
    float t = lerp(0.f, 1.f, x - y);
    return t;
}

struct OutputVS
{
    float4 posH : POSITION0;
    float4 color : COLOR0;
};

OutputVS geometryVS(float3 posL : POSITION0,
                    float3 normalL : NORMAL0)
{
    OutputVS outVS = (OutputVS) 0;

    outVS.posH = mul(float4(posL, 1.f), g_WVP);
    
    float3 normalW = mul(float4(normalL, 0.f), g_worldInvTranspose).xyz;
    normalW = normalize(normalW);

    //光の向きと法線のコサイン計算
    float3 posW = mul(float4(posL, 1.f), g_world).xyz;
    float3 lightVec = g_pointLightPosW - posW;
    float3 lightVec2 = g_pointLightPosW2 - posW;
    float3 lightVec3 = g_pointLightPosW3 - posW;
 
    float distance = length(lightVec);
    float distance2 = length(lightVec2);
    float distance3 = length(lightVec3);

    float ratio = max(dot(normalize(lightVec), normalW), 0.f);
    float ratio2 = max(dot(normalize(lightVec2), normalW), 0.f);
    float ratio3 = max(dot(normalize(lightVec3), normalW), 0.f);

    ratio /= (distance * g_pointLightAttenuation.x);
    ratio2 /= (distance2 * g_pointLightAttenuation.y);
    ratio3 /= (distance3 * g_pointLightAttenuation.z);

    ratio += (ratio2 + ratio3);

    //diffuse:
    float4 diffuse = g_diffuseLight * g_diffuseMtrl;
    diffuse *= ratio;

    //ambient
    float4 ambient = g_ambientLight * g_ambientMtrl;
    
    //specular
    float4 specular = g_specularLight * g_specularMtrl;
    //スペキュラ計算は大変。
    
    float3 toEye = normalize(g_eyePosW - posW);
    float3 r = reflect(-normalize(lightVec), normalW);
    float3 r2 = reflect(-normalize(lightVec2), normalW);
    float3 r3 = reflect(-normalize(lightVec3), normalW);

    float power = pow(max(dot(toEye, r), 0.f), g_specularPower);
    float power2 = pow(max(dot(toEye, r2), 0.f), g_specularPower);
    float power3 = pow(max(dot(toEye, r3), 0.f), g_specularPower);
    power  /= distance*g_specularAttenuation.x;
    power2 /= distance2*g_specularAttenuation.y;
    power3 /= distance3*g_specularAttenuation.z;
    power += (power2 + power3);
    specular *= power;

    outVS.color.rgb = (diffuse + ambient + specular).rgb;
    outVS.color.a = g_diffuseMtrl.a;

    return outVS;
}

float4 geometryPS(float4 c : COLOR0) : COLOR0
{
    
    return c;

}




Technique GeometryTech
{
    pass P0
    {
        VertexShader = compile vs_2_0 geometryVS();
        PixelShader = compile ps_2_0 geometryPS();

        FillMode = Solid;
    }
}
