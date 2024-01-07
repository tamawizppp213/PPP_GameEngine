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
#include "GameUtility/Base/Include/GUSmartPointer.hpp"

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
	class GPUResource 
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void TransitionResourceState(const core::ResourceState after) = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : GPU resourceのデバッグ名を設定します
		/*----------------------------------------------------------------------*/
		virtual void SetName(const gu::wstring& name) = 0;

		/*----------------------------------------------------------------------
		*  @brief : GPU resourceの扱い方について取得します
		/*----------------------------------------------------------------------*/
		virtual core::ResourceState GetResourceState() const noexcept = 0;
		
		/*----------------------------------------------------------------------
		*  @brief : GPU resourceをテクスチャとして使用する
		/*----------------------------------------------------------------------*/
		bool IsTexture() const { return _isTexture; }

		/*----------------------------------------------------------------------
		*  @brief : GPU resourceをバッファとして使用する
		/*----------------------------------------------------------------------*/
		bool IsBuffer() const { return !_isTexture; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUResource() = default;

		virtual ~GPUResource()
		{
			if (_device) { _device.Reset(); }
		}

		explicit GPUResource(const gu::SharedPointer<RHIDevice>& device) : _device(device) {};
		
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		// @brief : テクスチャかバッファか
		bool _isTexture = true;
	}; 
}


#endif