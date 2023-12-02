//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanErrorCode.cpp
///             @brief  LogErrorCode
///             @author Toide Yutaro
///             @date   2023_02_20
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/VulkanErrorCode.hpp"
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::vulkan::error;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
void rhi::vulkan::error::Log(VkResult result)
{
	printf("%d", (int)result);
}