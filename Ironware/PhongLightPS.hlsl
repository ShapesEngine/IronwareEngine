// for global dynamic light
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

// for object color
cbuffer ObjectCBuf
{
    float3 materialColor;
    float specularIntensity;
    float specularPower;
};

float4 main(float3 worldPos : Position, float3 n : Normal) : SV_Target
{
	// fragment to light vector data
    const float3 vToL = lightPos - worldPos;
    const float distToL = length(vToL);
    const float3 dirToL = vToL / distToL;
	// attenuation
    const float att = 1.f / (attConst + attLin * distToL + attQuad * (distToL * distToL));
	// diffuse intensity
    const float3 diffuse = diffuseColor * diffuseIntensity * att * max(0.f, dot(dirToL, n));
    // reflected light vector
    const float3 w = n * dot(vToL, n);
    const float3 r = w * 2.f - vToL;
	// calculate specular intensity based on angle between viewing vector and reflection vector, narrow with power function
    // multiplying by attenuation because we are using point light here
    const float3 specular = att * ( diffuseColor * diffuseIntensity ) * 
                            specularIntensity * pow( max( 0.f, dot( normalize( -r ), normalize( worldPos ) ) ), specularPower );
	// final color
    return float4(saturate((diffuse + ambient + specular) * materialColor), 1.f);
}