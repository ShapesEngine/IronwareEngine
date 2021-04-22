cbuffer LightCBuf
{
    float3 lightPos;
    float3 ambient;
    float3 diffuseColor;
    float diffuseIntensity;
    float attConst;
    float attLin;
    float attQuad;
};

cbuffer ObjectCBuf
{
    float3 materialColor;
    float specularIntensity;
    float specularPower;
};


float4 main( float3 viewPos : Position, float3 viewN : Normal ) : SV_Target
{
	// fragment to light vector data
    const float3 vToL = lightPos - viewPos;
    const float distToL = length( vToL );
    const float3 dirToL = vToL / distToL;
	// attenuation
    const float att = 1.0f / ( attConst + attLin * distToL + attQuad * ( distToL * distToL ) );
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max( 0.0f, dot( dirToL, viewN ) );
	// reflected light vector
    const float3 w = viewN * dot( vToL, viewN );
    const float3 r = w * 2.0f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    const float3 specular = att * ( diffuseColor * diffuseIntensity ) * specularIntensity * pow( max( 0.0f, dot( normalize( -r ), normalize( viewPos ) ) ), specularPower );
	// final color
    return float4( saturate( ( diffuse + ambient ) * materialColor + specular ), 1.0f );
}