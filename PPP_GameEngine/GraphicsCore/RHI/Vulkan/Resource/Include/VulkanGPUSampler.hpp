//////////////////////////////////////////////////////////////////////////////////
///             @file   VulkanGPUSampler.hpp
///             @brief  Sampler State 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_GPU_SAMPLER_HPP
#define Vulkan_GPU_SAMPLER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUSampler.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	/****************************************************************************
	*				  			GPUSampler
	*************************************************************************//**
	*  @class     GPUSampler
	*  @brief     Sampler state
	*****************************************************************************/
	class GPUSampler : public core::GPUSampler
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const VkSampler& GetSampler() const noexcept { return _sampler; }
		
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUSampler() = default;

		~GPUSampler();

		explicit GPUSampler(const std::shared_ptr<core::RHIDevice>& device, const core::SamplerInfo& samplerInfo);
	
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VkSampler _sampler = {};
	};

	
}
#endif