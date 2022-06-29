//////////////////////////////////////////////////////////////////////////////////
//              @file   VulkanFence.cpp
///             @brief  Fence
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanSwapchain.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanDevice.hpp"
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanCommandQueue.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include <algorithm>
#include <stdexcept>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::vulkan;
using namespace rhi;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Swapchain Support 
namespace
{

	/****************************************************************************
	*				  			SwapchainSupportDetails
	*************************************************************************//**
	*  @class     SwapchainSupportDetails
	*  @brief     Swapchain support
	*****************************************************************************/
	struct SwapchainSupportDetails
	{
		VkSurfaceCapabilitiesKHR         Capabilities;
		std::vector<VkSurfaceFormatKHR>  Formats;
		std::vector<VkPresentModeKHR>    PresentModes;

		static SwapchainSupportDetails Query(VkPhysicalDevice device, VkSurfaceKHR surface)
		{
			/*-------------------------------------------------------------------
			-               Acquire Surface Capabilities
			---------------------------------------------------------------------*/
			SwapchainSupportDetails details = {};
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);
			/*-------------------------------------------------------------------
			-               Acquire Surface Formats
			---------------------------------------------------------------------*/
			UINT32 formatCount = 0;
			vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);
			if (formatCount != 0)
			{
				details.Formats.resize(formatCount);
				vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, details.Formats.data());
			}
			/*-------------------------------------------------------------------
			-               Acquire Surface Formats
			---------------------------------------------------------------------*/
			UINT32 presentModeCount = 0;
			vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);
			if (presentModeCount != 0)
			{
				details.PresentModes.resize(presentModeCount);
				vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.PresentModes.data());
			}
			return details;
		}
	};
}
#pragma endregion Swapchain Support

RHISwapchain::RHISwapchain(const std::shared_ptr<rhi::core::RHIDevice>& device, const std::shared_ptr<rhi::core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, VkSurfaceKHR surface,
	const size_t frameBufferCount, std::uint32_t vsync) : rhi::core::RHISwapchain(device, commandQueue, windowInfo, pixelFormat, frameBufferCount, vsync)
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	
	/*-------------------------------------------------------------------
	-          Acquire surface
	---------------------------------------------------------------------*/
	if (surface)
	{
		_surface = surface;
	}
#ifdef _WIN32
	else
	{
		VkWin32SurfaceCreateInfoKHR createInfo = {};                            // Window32 Surface. (! not cross platform)
		createInfo.hinstance = (HINSTANCE)windowInfo.HInstance;                 // hInstance
		createInfo.hwnd      = (HWND)windowInfo.Handle;                         // hwnd 
		createInfo.hinstance = GetModuleHandle(nullptr);                        // hInstance
		createInfo.sType     = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR; // Structure type
		VkResult result      = vkCreateWin32SurfaceKHR(vulkan::RHIDevice::GetInstance(), &createInfo, nullptr, &_surface);
		if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to prepare surface.");
		}
	}
#endif
	/*-------------------------------------------------------------------
	-         null check
	---------------------------------------------------------------------*/
	if (_surface == nullptr) { throw std::runtime_error("failed to prepare surface"); }
	/*-------------------------------------------------------------------
	-          Set Swapchain
	---------------------------------------------------------------------*/
	InitializeSwapchain();
}
RHISwapchain::~RHISwapchain()
{
	const auto vkDevice = std::static_pointer_cast<vulkan::RHIDevice>(_device);
	vkDestroySwapchainKHR(vkDevice->GetDevice(), _swapchain, nullptr);
	vkDestroySemaphore(vkDevice->GetDevice(), _semaphore, nullptr);
	vkDestroySurfaceKHR(vulkan::RHIDevice::GetInstance(), _surface, nullptr);
}
/****************************************************************************
*                     Present
*************************************************************************//**
*  @fn        void RHISwapchain::Present()
*  @brief     Display front buffer
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHISwapchain::Present()
{
	/*-------------------------------------------------------------------
	-               Set Present Infomation
	---------------------------------------------------------------------*/
	VkPresentInfoKHR presentInfo = {};
	presentInfo.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.swapchainCount     = 1;
	presentInfo.pSwapchains        = &_swapchain;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores    = &_semaphore;
	presentInfo.pImageIndices      = &_currentBufferIndex;
	/*-------------------------------------------------------------------
	-               Get vk present queue
	---------------------------------------------------------------------*/
	const auto vkQueue = std::static_pointer_cast<rhi::vulkan::RHICommandQueue>(_commandQueue)->GetQueue();
	/*-------------------------------------------------------------------
	-               Present front buffer
	---------------------------------------------------------------------*/
	auto result = vkQueuePresentKHR(vkQueue, &presentInfo);
	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("failed to present front buffer.");
	}
	/*-------------------------------------------------------------------
	-               Go to next frame 
	---------------------------------------------------------------------*/
	UpdateCurrentFrameIndex();
}
/****************************************************************************
*                     Resize
*************************************************************************//**
*  @fn        void RHISwapchain::Resize(const size_t width, const size_t height)
*  @brief     Resize screen size. (set resized swapchain buffers )
*  @param[in] const size_t width
*  @param[in] const size_t height
*  @return 　　void
*****************************************************************************/
void RHISwapchain::Resize(const size_t width, const size_t height)
{
	/*-------------------------------------------------------------------
	-          If the size is not change, we do nothing
	---------------------------------------------------------------------*/
	if (_windowInfo.Width == width && _windowInfo.Height) { return; }
	/*-------------------------------------------------------------------
	-         Window size check
	---------------------------------------------------------------------*/
	if (_windowInfo.Width == 0 || _windowInfo.Height == 0) { throw std::runtime_error("Width or height is zero."); }
	/*-------------------------------------------------------------------
	-         Set window size 
	---------------------------------------------------------------------*/
	_windowInfo.Width  = width;
	_windowInfo.Height = height;
	/*-------------------------------------------------------------------
	-         Destroy swapchain and present semaphore
	---------------------------------------------------------------------*/
	const auto vkDevice = std::static_pointer_cast<rhi::vulkan::RHIDevice>(_device).get()->GetDevice();
	// buffer reset が入る予定
	vkDestroySwapchainKHR(vkDevice, _swapchain, nullptr);
	vkDestroySemaphore(vkDevice, _semaphore, nullptr);
	/*-------------------------------------------------------------------
	-         Reset swapchain
	---------------------------------------------------------------------*/
	InitializeSwapchain();
}
/****************************************************************************
*                     GetCurrentBufferIndex
*************************************************************************//**
*  @fn        size_t RHISwapchain::GetCurrentBufferIndex() const
*  @brief     Return current frame buffer
*  @param[in] void
*  @return 　　size_t
*****************************************************************************/
size_t RHISwapchain::GetCurrentBufferIndex() const
{
	return static_cast<std::uint32_t>(_currentBufferIndex);
}
/****************************************************************************
*                     InitializeSwapchain
*************************************************************************//**
*  @fn        void RHISwapchain::InitializeSwapchain()
*  @brief     Initialize swapchain
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHISwapchain::InitializeSwapchain()
{
	const auto vkDevice = std::static_pointer_cast<rhi::vulkan::RHIDevice>(_device);
	/*-------------------------------------------------------------------
	-               Acquire Surface infomation
	---------------------------------------------------------------------*/
	SwapchainSupportDetails details       = SwapchainSupportDetails::Query(vkDevice->GetPhysicalDevice(), _surface);
	VkSurfaceFormatKHR      surfaceFormat = SelectSwapchainFormat(details.Formats);
	VkPresentModeKHR        presentMode   = SelectSwapchainPresentMode(details.PresentModes);
	VkExtent2D              extent        = SelectSwapExtent(details.Capabilities);
	
	/*-------------------------------------------------------------------
	-               Acquire Swapchain frame buffer count
	---------------------------------------------------------------------*/
	UINT32 imageCount = _frameBufferCount;
	if (details.Capabilities.minImageCount > 0 && imageCount < details.Capabilities.minImageCount)
	{
		imageCount = details.Capabilities.minImageCount;
	}
	if (details.Capabilities.maxImageCount > 0 && imageCount > details.Capabilities.maxImageCount)
	{
		imageCount = details.Capabilities.maxImageCount;
	}

	/*-------------------------------------------------------------------
	-               Acquire Swapchain create infomation
	---------------------------------------------------------------------*/
	VkSwapchainCreateInfoKHR createInfo = {};
	createInfo.sType                 = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;   // structure type
	createInfo.surface               = _surface;                                      // display surface 
	createInfo.minImageCount         = imageCount;                                    // frame buffer image count
	createInfo.imageFormat           = surfaceFormat.format;                          // pixel format : SDR B8G8R8A8 HDR : Float
	createInfo.imageColorSpace       = surfaceFormat.colorSpace;                      // color space 
	createInfo.imageExtent           = extent;
	createInfo.imageArrayLayers      = 1;
	createInfo.imageUsage            = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;        
	createInfo.imageSharingMode      = VK_SHARING_MODE_EXCLUSIVE;                     // access to any range or image subresource of the object will be exclusive to a single queue family at a time.
	createInfo.clipped               = VK_FALSE;                                      // not clipped 
	createInfo.queueFamilyIndexCount = 0;
	createInfo.pQueueFamilyIndices   = nullptr;
	createInfo.oldSwapchain          = VK_NULL_HANDLE; 
	createInfo.preTransform          = details.Capabilities.currentTransform;
	createInfo.pNext                 = nullptr;
	createInfo.compositeAlpha        = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;             // alpha : 1.0 (ひとまず)
	/*-------------------------------------------------------------------
	-               Composite Alpha Flags
	---------------------------------------------------------------------*/
	const VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[] = {
			VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
			VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
			VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR
	};
	for (size_t index = 0; index < _countof(compositeAlphaFlags); index++)
	{
		if (details.Capabilities.supportedCompositeAlpha & compositeAlphaFlags[index])
		{
			createInfo.compositeAlpha = compositeAlphaFlags[index]; break;
		}
	}
	/*-------------------------------------------------------------------
	-               Create Swapchain KHR
	---------------------------------------------------------------------*/
	vkCreateSwapchainKHR(vkDevice->GetDevice(), &createInfo, nullptr, &_swapchain);
	_images.resize(imageCount);
	vkGetSwapchainImagesKHR(vkDevice->GetDevice(), _swapchain, &imageCount, _images.data());

	/*-------------------------------------------------------------------
	-               Create Semaphore
	---------------------------------------------------------------------*/
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.flags = 0;
	vkCreateSemaphore(vkDevice->GetDevice(), &semaphoreCreateInfo, nullptr, &_semaphore);

	/*-------------------------------------------------------------------
	-               Update current frame index
	---------------------------------------------------------------------*/
	UpdateCurrentFrameIndex();
}
/****************************************************************************
*                     UpdateCurrentFrameIndex
*************************************************************************//**
*  @fn        void RHISwapchain::EndDrawFrame()
*  @brief     Display front buffer
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void RHISwapchain::UpdateCurrentFrameIndex()
{
	const auto vkDevice       = std::static_pointer_cast<rhi::vulkan::RHIDevice>(_device);
	
	/*-------------------------------------------------------------------
	-               Get next frame buffer index
	---------------------------------------------------------------------*/
	UINT32     nextFrameIndex = 0;
	auto result = vkAcquireNextImageKHR(vkDevice.get()->GetDevice(), _swapchain, UINT64_MAX, _semaphore, VK_NULL_HANDLE, &nextFrameIndex);
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
	{
		throw std::runtime_error("failed to go to next frame");
	}
	/*-------------------------------------------------------------------
	-               Set up
	---------------------------------------------------------------------*/
	_currentBufferIndex = nextFrameIndex;

}

/****************************************************************************
*                     SelectSwapchainFormat
*************************************************************************//**
*  @fn        VkSurfaceFormatKHR RHISwapchain::SelectSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& format)
*  @brief     Select swapchain format
*  @param[in] const std::vector<VkSurfaceFormatKHR>& format
*  @return 　　VkSurfaceFormatKHR
*****************************************************************************/
VkSurfaceFormatKHR RHISwapchain::SelectSwapchainFormat(const std::vector<VkSurfaceFormatKHR>& formats)
{
	const auto vkDevice = std::static_pointer_cast<rhi::vulkan::RHIDevice>(_device);

	for (const auto& format : formats)
	{
		/*-------------------------------------------------------------------
		-               Find HDR Format
		---------------------------------------------------------------------*/
		if (vkDevice->IsHDRSupport())
		{
			if ((format.format == VK_FORMAT_R16G16B16A16_SFLOAT) ||
				(format.format == VK_FORMAT_A2B10G10R10_UNORM_PACK32))
			{
			}
			else { continue; }

			if ((format.colorSpace == VK_COLOR_SPACE_EXTENDED_SRGB_LINEAR_EXT) ||
				(format.colorSpace == VK_COLOR_SPACE_HDR10_ST2084_EXT) ||
				(format.colorSpace == VK_COLOR_SPACE_HDR10_HLG_EXT) ||
				(format.colorSpace == VK_COLOR_SPACE_DOLBYVISION_EXT))
			{
			}
			else { continue; }

			return format;
		}
		/*-------------------------------------------------------------------
		-               Find SDR Format
		---------------------------------------------------------------------*/
		else
		{
			if ((format.format == VK_FORMAT_B8G8R8A8_UNORM) ||
				(format.format == VK_FORMAT_B8G8R8A8_SRGB))
			{
			}
			else { continue; }

			if ((format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)) {}
			else { continue; }

			return format;
		}
	}
	return formats[0];
}
/****************************************************************************
*                     SelectSwapchainPresentMode
*************************************************************************//**
*  @fn        VkPresentModeKHR GraphicsDeviceVulkan::SelectSwapchainPresentMode(const std::vector<VkPresentModeKHR>& presentMode)
*  @brief     Select vsync mode
*  @param[in] const std::vector<VkPresentModeKHR>& presentMode
*  @return 　　VkPresentModeKHR
*****************************************************************************/
VkPresentModeKHR RHISwapchain::SelectSwapchainPresentMode(const std::vector<VkPresentModeKHR>& presentModes)
{
	for (const auto& presentMode : presentModes)
	{
		if (_vsync == 0 && presentMode == VK_PRESENT_MODE_IMMEDIATE_KHR) { return presentMode; }
		if (_vsync > 0  && presentMode == VK_PRESENT_MODE_MAILBOX_KHR)   { return presentMode; }
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}
/****************************************************************************
*                     SelectSwapExtent
*************************************************************************//**
*  @fn        VkExtent2D  GraphicsDeviceVulkan::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
*  @brief     Select swapchain screen size
*  @param[in] const std::vector<VkPresentModeKHR>& presentMode
*  @return 　　VkExtent2D
*****************************************************************************/
VkExtent2D RHISwapchain::SelectSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities)
{
	// https://wasuregasa.3rin.net/%E3%82%A2%E3%83%9E%E3%82%B0%E3%83%A9%E3%83%9F%E3%83%B3%E3%82%B0/c--%20%E3%81%AE%20std--numeric_limits
	if (capabilities.currentExtent.width != (std::numeric_limits<unsigned int>::max)())
	{
		return capabilities.currentExtent;
	}
	else
	{
		int width               = static_cast<int>(Screen::GetScreenWidth());
		int height              = static_cast<int>(Screen::GetScreenHeight());
		VkExtent2D actualExtent = { static_cast<UINT32>(width), static_cast<UINT32>(height) };
		actualExtent.width      = std::clamp(actualExtent.width , capabilities.minImageExtent.width , capabilities.maxImageExtent.width);
		actualExtent.height     = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);
		return actualExtent;
	}
}