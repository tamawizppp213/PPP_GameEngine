//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RayTracingTLASBuffer.hpp
///             @brief  TLAS Buffer
///             @author Toide Yutaro
///             @date   2022_11_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RAYTRACING_TLAS_BUFFER_HPP
#define DIRECTX12_RAYTRACING_TLAS_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/RayTracing/Include/RayTracingTLASBuffer.hpp"
#include <vulkan/vulkan.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable : 4100)
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::vulkan
{
	/****************************************************************************
	*				  			TLASBuffer
	*************************************************************************//**
	*  @struct    TLASBuffer
	*  @brief     Bottom Level Acceleration Structure Buffer
	*****************************************************************************/
	class TLASBuffer : public core::TLASBuffer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Build(const gu::SharedPointer<core::RHICommandList>& commandList) override {};
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		TLASBuffer() = default;
		~TLASBuffer();
		TLASBuffer(const gu::SharedPointer<core::RHIDevice>& device,
			const gu::DynamicArray<gu::SharedPointer<core::ASInstance>>& blasBuffers,
			const core::BuildAccelerationStructureFlags flags);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/

	};
}
#endif