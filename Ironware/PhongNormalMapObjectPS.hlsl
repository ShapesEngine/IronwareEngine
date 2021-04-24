Texture2D tex;
Texture2D nmap;

SamplerState splr;

// for global dynamic light
#include "CommonOps.hlsli"
#include "CommonLightOps.hlsli"

#include "CommonPointLightOps.hlsli"

// for object color
cbuffer ObjectCBuf
{
    float specularIntensity;
    float specularPower;
    bool isNMapEnabled;
};

#include "CommonTransforms.hlsli"

float4 main( float3 viewPos : Position, float3 viewN : Normal, float2 tc : TexCoord ) : SV_Target
{
    if( isNMapEnabled )
    {
        const float3 sampledNMap = (float3)nmap.Sample( splr, tc );
        viewN = 2.f * sampledNMap - 1.f;
        viewN = normalize( mul( viewN, (float3x3)modelView ) );
    }
    // fragment to light vector data
    LightVectorData lightVec = calc_light_vector_data( viewLightPos, viewPos );
	// attenuation
    const float luminosity = calc_luminosity( attConst, attLin, attQuad, lightVec.distToL );
	// diffuse intensity
    const float3 diffuse = calc_diffuse( diffuseColor, diffuseIntensity, luminosity, lightVec.dirToL, viewN );
    const float3 specular = calc_specular( specularIntensity.rrr, 1.f, viewN, lightVec.vToL, viewPos, luminosity, specularPower );
	// final color
    return float4( saturate( ( diffuse + ambient ) * tex.Sample( splr, tc ).rgb + specular ), 1.f );
}