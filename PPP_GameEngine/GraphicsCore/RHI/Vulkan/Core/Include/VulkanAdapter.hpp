//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIAdapter.hpp
///             @brief  Physical Device (adapter), Describe gpu information 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_ADAPTER_HPP
#define VULKAN_ADAPTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIAdapter.hpp"
#include <vulkan/vulkan.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			RHIAdapter
	*************************************************************************//**
	*  @class     RHIAdapter
	*  @brief     Physical Device (Adapter)  Describe gpu information
	*****************************************************************************/
	class RHIDisplayAdapter : public rhi::core::RHIDisplayAdapter
	{
	public:
		struct PhysicalDeviceInfo
		{
			// memory 
			VkPhysicalDeviceMemoryProperties MemoryProperties = {};

			std::vector<VkQueueFamilyProperties> QueueFamilyProperties = {};

			// vulkanのversionに合わせたFeature list
			VkPhysicalDeviceFeatures         Features10 = {};
			VkPhysicalDeviceVulkan11Features Features11 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES};
			VkPhysicalDeviceVulkan12Features Features12 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
			VkPhysicalDeviceVulkan13Features Features13 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };

			// vulkanのversionに合わせたproperty list
			VkPhysicalDeviceProperties         Properties10 = {};
			VkPhysicalDeviceVulkan11Properties Properties11 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_PROPERTIES };
			VkPhysicalDeviceVulkan12Properties Properties12 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_PROPERTIES };
			VkPhysicalDeviceVulkan13Properties Properties13 = { VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_PROPERTIES };
		};

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* return logical device shared pointer. frame count is used for the command allocators*/
		std::shared_ptr<core::RHIDevice> CreateDevice() override;
		
		/* Describe physical device name and spec(future work) */
		void PrintInfo() override; 
		
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const VkPhysicalDevice GetPhysicalDevice() const noexcept { return _physicalDevice; }

		/* return all physical device information(memory, feature, property)*/
		PhysicalDeviceInfo& GetPhysicalDeviceInfo() { return _physicalDeviceInfo; }
		const PhysicalDeviceInfo& GetPhysicalDeviceInfo() const noexcept { return _physicalDeviceInfo; }
		
		/* return physical device characteristics (device id, vender id...)*/
		VkPhysicalDeviceProperties GetProperties() const noexcept;

		// return physical device limits
		VkPhysicalDeviceLimits GetLimits() const noexcept;
		
		/* return physical device support list (ex. can use geometry shader...?)*/
		VkPhysicalDeviceFeatures GetSupports() const noexcept;
		
		/* return format available properties*/
		VkFormatProperties GetFormatProperties(const VkFormat format) const noexcept;

		// return memory type, memory heap.
		VkPhysicalDeviceMemoryProperties     GetMemoryProperties() const noexcept;
		
		/* return all available extension name list*/
		std::vector<VkExtensionProperties>   GetExtensionProperties  () const noexcept;
		std::vector<std::string>             GetExtensionNameList    () const noexcept;
		
		/* return queue family characteristics*/
		std::vector<VkQueueFamilyProperties> GetQueueFamilyProperties() const noexcept;
		
		bool IsPresentSupported(VkSurfaceKHR surface, std::uint32_t queueFamilyIndex) const;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIDisplayAdapter() = default;
		
		~RHIDisplayAdapter();
		
		RHIDisplayAdapter(const std::shared_ptr<core::RHIInstance>& instance, const VkPhysicalDevice physicalDevice);
	
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkPhysicalDevice _physicalDevice = nullptr;

		PhysicalDeviceInfo _physicalDeviceInfo = {};
	};
}
#endif