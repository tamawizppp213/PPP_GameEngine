//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanSwapchain.hpp
///             @brief  Swapchain
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_SWAP_CHAIN_HPP
#define VULKAN_SWAP_CHAIN_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHISwapchain.hpp"
#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan/vulkan.h>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Fence
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			TemplateClass
	*************************************************************************//**
	*  @class     TemplateClass
	*  @brief     temp
	*****************************************************************************/
	class RHISwapchain : public rhi::core::RHISwapchain
	{
		
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Display front buffer*/
		void Present() override ;
		/* @brief : Resize screen size. (set resized swapchain buffers )*/
		void Resize(const size_t width, const size_t height) override ;
		/* @brief : Return current frame buffer*/
		size_t GetCurrentBufferIndex() const override ;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		VkSwapchainKHR GetSwapchain() { return _swapchain; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHISwapchain() = default;
		explicit RHISwapchain(const std::shared_ptr<rhi::core::RHIDevice>& device,
			const std::shared_ptr<rhi::core::RHICommandQueue>& commandQueue,
			const core::WindowInfo& windowInfo,
			const core::PixelFormat& pixelFormat,
			VkSurfaceKHR surface = nullptr,
			const size_t frameBufferCount = 2,std::uint32_t vsync = 0);
		~RHISwapchain();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkSwapchainKHR _swapchain = nullptr;
		VkSurfaceKHR   _surface   = nullptr;
		VkSemaphore    _semaphore = nullptr;
		std::uint32_t _currentBufferIndex = 0;
		std::vector<VkImage> _images;
	private:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void InitializeSwapchain();
		void UpdateCurrentFrameIndex();
		/** @brief Select color format and color space format*/
		VkSurfaceFormatKHR SelectSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& format);
		/** @brief Select vsync mode*/
		VkPresentModeKHR   SelectSwapchainPresentMode(const std::vector<VkPresentModeKHR>& presentMode);
		/** @brief Select swapchain screen size*/
		VkExtent2D         SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	};
}
#endif