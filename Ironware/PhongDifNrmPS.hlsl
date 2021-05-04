Texture2D tex;
Texture2D nmap : register( t2 );

SamplerState splr;

// for global dynamic light
#include "CommonPointLightOps.hlsli"
#include "CommonLightOps.hlsli"
#include "CommonOps.hlsli"

// for object color
cbuffer SpecularCBuf
{
    float3 specularColor;
    float specularWeight;
    float specularGloss;
    bool useNormalMap;
    float normalMapWeight;
};

#include "CommonTransforms.hlsli"

float4 main( float3 viewPos : Position, float3 viewN : Normal, float2 tc : TexCoord, float3 viewTan : Tangent, float3 viewBitan : Bitangent ) : SV_Target
{
    if( useNormalMap )
    {
        const float3x3 tanToView = float3x3(
            normalize( viewTan ),
            normalize( viewBitan ),
            normalize( viewN )
        );
        
        const float3 sampledNMap = (float3)nmap.Sample( splr, tc );
        viewN = 2.0f * sampledNMap - 1.0f;
        viewN.y = -viewN.y;
        viewN = normalize( mul( viewN, tanToView ) );
    }
    // fragment to light vector data
    const LightVectorData lightVec = calc_light_vector_data( viewLightPos, viewPos );
	// attenuation
    const float luminosity = calc_luminosity( attConst, attLin, attQuad, lightVec.distToL );
	// diffuse intensity
    const float3 diffuse = calc_diffuse( diffuseColor, diffuseIntensity, luminosity, lightVec.dirToL, viewN );
    // reflected light vector
    const float3 specular = calc_specular(
    diffuseColor * diffuseIntensity * specularColor,
    specularWeight, viewN, lightVec.vToL, viewPos, luminosity, specularGloss
    );
	// final color
    return float4( saturate( ( diffuse + ambient ) * tex.Sample( splr, tc ).rgb + specular ), 1.f );
}