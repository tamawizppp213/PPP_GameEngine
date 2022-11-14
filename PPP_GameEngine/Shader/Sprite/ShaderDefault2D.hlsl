//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderDefault.hlsl
//            Content:  Create Skybox
//             Author:  Toide Yutaro
//             Create:  2020_04_24
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_DEFAULT2D_HLSL
#define SHADER_DEFAULT2D_HLSL
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Core/ShaderVertexType.hlsli"
#define VertexIn VSInputPositionNormalColorTexture
#define VertexOut VSOutputPositionNormalColorTexture
//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////
SamplerState SamplerLinearWrap : register(s0);
Texture2D    DestDiffuseMap : register(t0);
//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
VertexOut VSMain(VertexIn vertexIn)
{
    VertexOut result;
    result.Position = vertexIn.Position;
    result.UV       = vertexIn.UV;
    result.Normal   = vertexIn.Normal;
    result.Color    = vertexIn.Color;
    
    return result;
}

float4 PSMain(VertexOut input) : SV_Target
{
    return float4(DestDiffuseMap.Sample(SamplerLinearWrap, input.UV)) * input.Color;
}

float4 PSTest(VertexOut input) : SV_Target
{
    return float4(1, 1, 1, 1);
}
#endif