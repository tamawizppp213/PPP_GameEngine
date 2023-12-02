//////////////////////////////////////////////////////////////////////////////////
//              Title:  ShaderCore.hlsli
//            Content:  シェーダーでよく使う関数をまとめています.
//             Author:  Toide Yutaro
//             Create:  2020_04_24
//////////////////////////////////////////////////////////////////////////////////
#ifndef SHADER_CORE_HLSLI
#define SHADER_CORE_HLSLI
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                             Define
/////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                            Implement
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*                     LinearizeDepth
*************************************************************************//**
*  @fn        float LinearizeDepth(const float depth, const float near, const float far)
*
*  @brief     非線形なClipping空間におけるdepthを線形に変化させます
*
*  @param[in] const float projectionDepth
*  @param[in] const float nearZ
*  @param[in] const float FarZ
*
*  @return    void
*****************************************************************************/
float LinearizeDepth(const float depth, const float near, const float far)
{
    return (2.0f * near) / (far + near - depth * (far - near));
}


#endif