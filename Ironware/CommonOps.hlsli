float3 map_normal(
    const in float3 tan,
    const in float3 bitan,
    const in float3 n,
    const in float2 tc,
    uniform Texture2D nmap,
    uniform SamplerState splr
)
{
    const float3x3 tanToTarget = float3x3( tan, bitan, n );
        
    const float3 sampledNMap = nmap.Sample( splr, tc ).xyz;
    const float3 tanN = 2.0f * sampledNMap - 1.0f;
    return normalize( mul( tanN, tanToTarget ) );
}

float calc_luminosity(
    uniform float attConst,
    uniform float attLin,
    uniform float attQuad,
    const in float distFragToL
)
{
    return 1.f / ( attConst + attLin * distFragToL + attQuad * ( distFragToL * distFragToL ) );
}

float3 calc_diffuse(
    uniform float3 diffuseColor,
    uniform float diffuseIntensity,
    const in float luminosity,
    const in float3 viewDirFragToL,
    const in float3 viewN
)
{
    return diffuseColor * diffuseIntensity * luminosity * max( 0.f, dot( viewDirFragToL, viewN ) );
}

float3 calc_speculate(
    const in float3 specularColor,
    uniform float specularIntensity,
    const in float3 viewN,
    const in float3 viewFragToL,
    const in float3 viewPos,
    const in float luminosity,
    const in float specularPower )
{
    // calculate reflected light vector
    const float3 w = viewN * dot( viewFragToL, viewN );
    const float3 r = normalize( w * 2.f - viewFragToL );
    // vector from camera to fragment (in view space)
    const float3 viewCamToFrag = normalize( viewPos );
    // calculate specular component color based on angle between
    // viewing vector and reflection vector, narrow with power function
    return luminosity * specularColor * specularIntensity * pow( max( 0.0f, dot( -r, viewCamToFrag ) ), specularPower );
}