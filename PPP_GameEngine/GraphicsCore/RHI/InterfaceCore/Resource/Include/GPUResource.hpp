//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUResource.hpp
///             @brief  GPU Resource 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_RESOURCE_HPP
#define GPU_RESOURCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::core
{
	class RHIDevice;
	/****************************************************************************
	*				  			GPUResource
	*************************************************************************//**
	*  @class     GPUResource 
	*  @brief     Resource (後でNoncopyableに変更する)
	*****************************************************************************/
	class GPUResource : public std::enable_shared_from_this<GPUResource>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResource() = default;
		~GPUResource() = default;
		explicit GPUResource(const std::shared_ptr<RHIDevice>& device) : _device(device) {};
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;
	}; 
}


#endif