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
    bool hasGloss;
    bool useSpecMap;
    float3 specularColor;
    float specularMapWeight;
    float specularGloss;
    bool isNMapEnabled;
    float normalMapWeight;
};

float4 main( float3 viewPos : Position, float3 viewN : Normal, float2 tc : TexCoord, float3 viewTan : Tangent, float3 viewBitan : Bitangent ) : SV_Target
{
    float4 sampledDiff = tex.Sample( splr, tc );
    
#ifdef MASKING
    clip( sampledDiff.a < 0.1f ? -1.f : 1.f );
    
    if( dot( viewN, viewPos ) >= 0.f )
    {
        viewN = -viewN;
    }
#endif
    
    viewN = normalize( viewN );
    
    if( isNMapEnabled )
    {
        const float3 mappedN = map_normal( normalize( viewTan ), normalize( viewBitan ), viewN, tc, nmap, splr );
        viewN = lerp( viewN, mappedN, normalMapWeight );
    }
    // fragment to light vector data
    LightVectorData lightVec = calc_light_vector_data( viewLightPos, viewPos );
	// attenuation
    const float luminosity = calc_luminosity( attConst, attLin, attQuad, lightVec.distToL );
	// diffuse intensity
    const float3 diffuse = calc_diffuse( diffuseColor, diffuseIntensity, luminosity, lightVec.dirToL, viewN );
    
    float3 specularReflectionColor;
    float specularPower = specularGloss;
    const float4 sampledSpec = specTex.Sample( splr, tc );
    if( useSpecMap )
    {
        specularReflectionColor = sampledSpec.rgb;
    }
    else
    {
        specularReflectionColor = specularColor;
    }
    
    if( hasGloss )
    {
        specularPower = pow( 2.f, sampledSpec.a * 13.f );
    }
    
    const float3 specular = calc_specular( diffuseColor * diffuseIntensity * specularReflectionColor, specularMapWeight, viewN, lightVec.vToL, viewPos, luminosity, specularPower );
	// final color
    return float4( saturate( ( diffuse + ambient ) * sampledDiff.rgb + specular ), 1.f );
}