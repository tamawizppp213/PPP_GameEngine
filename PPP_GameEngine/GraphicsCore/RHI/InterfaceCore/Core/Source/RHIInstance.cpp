//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIInstance.hpp
///             @brief  Select device api (���̃G���W�����g�p���ŏ��ɌĂяo��.) 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIInstance.hpp"
// DirectX12
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Instance.hpp"
// Vulkan
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanInstance.hpp"

// common
#include <stdexcept>
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<RHIInstance> rhi::core::RHIInstance::CreateInstance(const rhi::core::APIVersion version, bool enableCPUDebugger, bool enableGPUDebugger)
{
	switch (version)
	{
		case APIVersion::DirectX12: { std::cout << "DirectX12" << std::endl; return std::make_shared<rhi::directX12::RHIInstance>(enableCPUDebugger,enableGPUDebugger);}
		case APIVersion::Vulkan:    { std::cout << "Vulkan"    << std::endl; return std::make_shared<rhi::vulkan::RHIInstance>(enableCPUDebugger, enableGPUDebugger); }
		default: { throw std::runtime_error("Unknown API."); }
	}
}
