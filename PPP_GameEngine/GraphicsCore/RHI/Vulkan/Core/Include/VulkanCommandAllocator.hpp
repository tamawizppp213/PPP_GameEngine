//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanCommandAllocator.hpp
///             @brief  Command Allocator
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_COMMAND_ALLOCATOR_HPP
#define VULKAN_COMMAND_ALLOCATOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommandAllocator.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Fence
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHICommandAllocator
	*************************************************************************//**
	*  @class     RHICommandAllocator
	*  @brief     Execute GPU Command list
	*****************************************************************************/
	class RHICommandAllocator : public rhi::core::RHICommandAllocator
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Reset() override;
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkCommandPool GetCommandAllocator() const noexcept { return _commandPool; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHICommandAllocator() = default;

		// for vulkan constructor
		explicit RHICommandAllocator(const std::shared_ptr<rhi::core::RHIDevice>& device, const core::CommandListType type, const std::uint32_t queueFamilyIndex);

		~RHICommandAllocator();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkCommandPool _commandPool = nullptr;
	};
}
#endif