//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RayTracingGeometry.hpp
///             @brief  RayTracing geometry descriptor
///             @author Toide Yutaro
///             @date   2022_11_22
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingGeometry.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
RayTracingGeometry::RayTracingGeometry(const gu::SharedPointer<core::RHIDevice>& device,
	const core::RayTracingGeometryFlags flags,
	const gu::SharedPointer<core::GPUBuffer>& vertexBuffer,
	const gu::SharedPointer<core::GPUBuffer>& indexBuffer) :  
	rhi::core::RayTracingGeometry(device, flags, vertexBuffer, indexBuffer)
{
	const auto dxVertexBuffer = gu::StaticPointerCast<directX12::GPUBuffer>(_vertexBuffer);
	const auto dxIndexBuffer  = gu::StaticPointerCast<directX12::GPUBuffer>(_indexBuffer);
	
	_geometryDesc.Type  = D3D12_RAYTRACING_GEOMETRY_TYPE_TRIANGLES;
	_geometryDesc.Flags = EnumConverter::Convert(flags);
	_geometryDesc.Triangles.VertexBuffer.StartAddress  = dxVertexBuffer->GetResource()->GetGPUVirtualAddress();
	_geometryDesc.Triangles.VertexBuffer.StrideInBytes = dxVertexBuffer->GetElementByteSize();
	_geometryDesc.Triangles.VertexFormat = DXGI_FORMAT_R32G32B32_FLOAT; // 後で変更する
	
	if (_indexBuffer)
	{
		_geometryDesc.Triangles.IndexBuffer = dxIndexBuffer->GetResource()->GetGPUVirtualAddress();
		_geometryDesc.Triangles.IndexCount  = static_cast<UINT>(dxIndexBuffer->GetElementCount());
		_geometryDesc.Triangles.IndexFormat = DXGI_FORMAT_R32_UINT; // 後で変更する
	}

}