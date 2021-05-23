Texture2D tex : register( t0 );
Texture2D nmap : register( t2 );

SamplerState splr : register( s0 );

// for global dynamic light
#include "CommonPointLightOps.hlsli"
#include "CommonLightOps.hlsli"
#include "CommonOps.hlsli"
#include "CommonShadowOps_PO.hlsli"

// for object color
cbuffer SpecularCBuf : register( b1 )
{
    float3 specularColor;
    float specularWeight;
    float specularGloss;
    bool useNormalMap;
    float normalMapWeight;
};

#include "CommonTransforms.hlsli"

float4 main( float3 viewPos : Position, float3 viewN : Normal, float2 tc : TexCoord, float3 viewTan : Tangent, float3 viewBitan : Bitangent, float4 spos : ShadowPosition ) : SV_Target
{
    float3 diffuse;
    float3 specular;
    
    const float shadowLevel = shadow( spos );
    if( shadowLevel != 0.0f )
    {
        
        viewN = normalize( viewN );
    
        if( useNormalMap )
        {
            const float3 mappedNormal = map_normal( normalize( viewTan ), normalize( viewBitan ), viewN, tc, nmap, splr );
            viewN = lerp( viewN, mappedNormal, normalMapWeight );
        }
        // fragment to light vector data
        const LightVectorData lightVec = calc_light_vector_data( viewLightPos, viewPos );
	    // attenuation
        const float luminosity = calc_luminosity( attConst, attLin, attQuad, lightVec.distToL );
	    // diffuse intensity
        diffuse = calc_diffuse( diffuseColor, diffuseIntensity, luminosity, lightVec.dirToL, viewN );
        // reflected light vector
        specular = calc_specular(
            diffuseColor * diffuseIntensity * specularColor,
            specularWeight, viewN, lightVec.vToL, viewPos, luminosity, specularGloss
            );
        // scale by shadow level
        diffuse *= shadowLevel;
        specular *= shadowLevel;
    }
    else
    {
        diffuse = specular = 0.f;
    }
	// final color
    return float4( saturate( ( diffuse + ambient ) * tex.Sample( splr, tc ).rgb + specular ), 1.f );
}