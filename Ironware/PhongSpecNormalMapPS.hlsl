#include "CommonPointLightOps.hlsli"
#include "CommonOps.hlsli"
#include "CommonLightOps.hlsli"

Texture2D tex;
Texture2D specTex;
Texture2D nmap;

SamplerState splr;

// for object color
cbuffer NMapCbuf
{
    bool isNMapEnabled;
    bool isSpecMapEnabled;
    bool hasGloss;
    float specularPowerConst;
    float3 specularColor;
    float specularMapWeight;
};

float4 main( float3 viewPos : Position, float3 viewN : Normal, float2 tc : TexCoord, float3 viewTan : Tangent, float3 viewBitan : Bitangent ) : SV_Target
{
    viewN = normalize( viewN );
    
    if( isNMapEnabled )
    {
        viewN = map_normal( normalize( viewTan ), normalize( viewBitan ), viewN, tc, nmap, splr );
    }
    // fragment to light vector data
    LightVectorData lightVec = calc_light_vector_data( viewLightPos, viewPos );
	// attenuation
    const float luminosity = calc_luminosity( attConst, attLin, attQuad, lightVec.distToL );
	// diffuse intensity
    const float3 diffuse = calc_diffuse( diffuseColor, diffuseIntensity, luminosity, lightVec.dirToL, viewN );
    
    float3 specularReflectionColor;
    float specularPower = specularPowerConst;
    if( isSpecMapEnabled )
    {
        const float4 sampledSpec = specTex.Sample( splr, tc );
        specularReflectionColor = sampledSpec.rgb * specularMapWeight;
        if( hasGloss )
        {
            specularPower = pow( 2.f, sampledSpec.a * 13.f );
        }
    }
    else
    {
        specularReflectionColor = specularColor;
    }
    
    const float3 specular = calc_specular( specularReflectionColor, 1.f, viewN, lightVec.vToL, viewPos, luminosity, specularPower );
	// final color
    return float4( saturate( ( diffuse + ambient ) * tex.Sample( splr, tc ).rgb + specular ), 1.f );
}