//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RayTracingBLASBuffer.hpp
///             @brief  BLAS Buffer
///             @author Toide Yutaro
///             @date   2022_11_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RAYTRACING_BLAS_BUFFER_HPP
#define DIRECTX12_RAYTRACING_BLAS_BUFFER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/RayTracing/Include/RayTracingBLASBuffer.hpp"
#include <d3d12.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

namespace rhi::directX12
{
	/****************************************************************************
	*				  			BLASBuffer
	*************************************************************************//**
	*  @struct    BLASBuffer
	*  @brief     Bottom Level Acceleration Structure Buffer
	*****************************************************************************/
	class BLASBuffer : public core::BLASBuffer
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Build(const gu::SharedPointer<core::RHICommandList>& commandList) override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		D3D12_GPU_VIRTUAL_ADDRESS GetGPUAddress() const noexcept { return _rayTracingASDesc.DestAccelerationStructureData; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		BLASBuffer() = default;
		~BLASBuffer();
		BLASBuffer(const gu::SharedPointer<core::RHIDevice>& device,
			const gu::DynamicArray<gu::SharedPointer<core::RayTracingGeometry>>& geometryDesc,
			const core::BuildAccelerationStructureFlags flags);
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_DESC _rayTracingASDesc = {};
	};
}
#endif