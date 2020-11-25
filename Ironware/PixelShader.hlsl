cbuffer CBuf
{
    float4 face_colors[6];
    float taramba;    
};

float4 main(uint tid : SV_PrimitiveID) : SV_TARGET
{
    return face_colors[(tid / 2) % 6];
}