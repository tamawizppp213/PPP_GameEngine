//////////////////////////////////////////////////////////////////////////////////
///             @file   RayTracingShaderTable.hpp
///             @brief  RayTracing shader table
///             @author Toide Yutaro
///             @date   2023_01_15
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/RayTracingShaderTable.hpp"
#include "GraphicsCore/RHI/InterfaceCore/PipelineState/Include/GPUShaderState.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
RayTracingShaderTable::~RayTracingShaderTable()
{
	_shaderList.clear(); _shaderList.shrink_to_fit();

	_buffer.reset();
}
#pragma endregion Constructor and Destructor

#pragma region Protected Function
std::uint32_t RayTracingShaderTable::RoundUp(const std::uint64_t size, const std::uint32_t alignment)
{
	return std::uint32_t(size + alignment - 1) & ~(alignment - 1);
}


#pragma endregion Protected Function