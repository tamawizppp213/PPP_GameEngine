//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12RayTracingBLASBuffer.hpp
///             @brief  BLAS Buffer
///             @author Toide Yutaro
///             @date   2022_11_23
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingTLASBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingBLASBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingASInstance.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingGeometry.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
TLASBuffer::~TLASBuffer()
{

}
TLASBuffer::TLASBuffer(const std::shared_ptr<core::RHIDevice>& device,
	const std::vector<std::shared_ptr<core::ASInstance>>& asInstance,
	const core::BuildAccelerationStructureFlags flags,
	const std::shared_ptr<core::RHIDescriptorHeap>& customHeap)
	: core::TLASBuffer(device, asInstance, flags, customHeap)
{
	const auto dxDevice = std::static_pointer_cast<directX12::RHIDevice>(_device);

	const auto numInstance = static_cast<std::uint32_t>(asInstance.size());
	/*-------------------------------------------------------------------
	-         Set up acceleration structure inputs
	---------------------------------------------------------------------*/
	D3D12_BUILD_RAYTRACING_ACCELERATION_STRUCTURE_INPUTS inputs = {};
	inputs.DescsLayout    = D3D12_ELEMENTS_LAYOUT_ARRAY;
	inputs.Flags          = EnumConverter::Convert(_flags);                         // build flag
	inputs.NumDescs       = numInstance;                                            
	inputs.Type           = D3D12_RAYTRACING_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL; // TLAS

	/*-------------------------------------------------------------------
	-         Get the size requirements for the scratch and AS buffers
	---------------------------------------------------------------------*/
	D3D12_RAYTRACING_ACCELERATION_STRUCTURE_PREBUILD_INFO preBuildInfo = {};
	dxDevice->GetDevice()->GetRaytracingAccelerationStructurePrebuildInfo(&inputs, &preBuildInfo);

	/*-------------------------------------------------------------------
	-         Set up Scratch Buffer
	---------------------------------------------------------------------*/
	{
		core::GPUBufferMetaData metaData = core::GPUBufferMetaData::DefaultBuffer(preBuildInfo.ScratchDataSizeInBytes, 1);
		metaData.ResourceUsage = core::ResourceUsage::UnorderedAccess;
		metaData.State = core::ResourceState::Common;
		_scratch = dxDevice->CreateBuffer(metaData);
	}

	/*-------------------------------------------------------------------
	-         Set up Destination Buffer
	---------------------------------------------------------------------*/
	{
		core::GPUBufferMetaData metaData = core::GPUBufferMetaData::DefaultBuffer(preBuildInfo.ResultDataMaxSizeInBytes, 1);
		metaData.ResourceUsage = core::ResourceUsage::UnorderedAccess;
		metaData.State = core::ResourceState::RayTracingAccelerationStructure;
		_destination = dxDevice->CreateBuffer(metaData);
	}

	/*-------------------------------------------------------------------
	-         Instance Desc
	---------------------------------------------------------------------*/
	std::vector<D3D12_RAYTRACING_INSTANCE_DESC> dxDescs(numInstance);
	for (std::uint64_t i = 0; i < numInstance; ++i)
	{
		dxDescs[i] = std::static_pointer_cast<directX12::ASInstance>(_asInstance[i])->GetDesc();
	}

	/*-------------------------------------------------------------------
	-         Instance Buffer
	---------------------------------------------------------------------*/
	{
		core::GPUBufferMetaData metaData = core::GPUBufferMetaData::UploadBuffer(
			sizeof(D3D12_RAYTRACING_INSTANCE_DESC), numInstance);
		metaData.State = core::ResourceState::GeneralRead;
		_asInstanceDescsBuffer = dxDevice->CreateBuffer(metaData);
		_asInstanceDescsBuffer->Pack(dxDescs.data(), nullptr); // upload buffer�̂��߁Anullptr
	}

	/*-------------------------------------------------------------------
	-         Create TLAS
	---------------------------------------------------------------------*/
	_rayTracingASDesc.Inputs = inputs;
	_rayTracingASDesc.Inputs.InstanceDescs             = std::static_pointer_cast<directX12::GPUBuffer>(_asInstanceDescsBuffer)->GetResource()->GetGPUVirtualAddress();   //�@��ŏC������
	_rayTracingASDesc.ScratchAccelerationStructureData = std::static_pointer_cast<directX12::GPUBuffer>(_scratch)->GetResource()->GetGPUVirtualAddress();     // TLAS Scratch Buffer
	_rayTracingASDesc.DestAccelerationStructureData    = std::static_pointer_cast<directX12::GPUBuffer>(_destination)->GetResource()->GetGPUVirtualAddress(); //�i�[��
	
}

#pragma endregion Constructor and Destructor
#pragma region Build Function
void TLASBuffer::Build(const std::shared_ptr<core::RHICommandList>& commandList)
{
	/*-------------------------------------------------------------------
	-         Check Has Built 
	---------------------------------------------------------------------*/
	if (_hasBuilt) { return; }

	/*-------------------------------------------------------------------
	-         Prepare dxResource
	---------------------------------------------------------------------*/
	const auto dxDevice      = std::static_pointer_cast<directX12::RHIDevice>(_device);
	const auto dxCommandList = std::static_pointer_cast<directX12::RHICommandList>(commandList)->GetCommandList();
	const auto dxTLASBuffer  = std::static_pointer_cast<directX12::GPUBuffer>(_destination);

	/*-------------------------------------------------------------------
	-         Execute CommandList
	---------------------------------------------------------------------*/
	dxCommandList->BuildRaytracingAccelerationStructure(&_rayTracingASDesc, 0, nullptr);
	/*-------------------------------------------------------------------
	-         Resource barrier
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_BARRIER uavBarrier = {};
	uavBarrier.Type          = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	uavBarrier.UAV.pResource = dxTLASBuffer->GetResourcePtr();
	dxCommandList->ResourceBarrier(1, &uavBarrier);
	
	/*-------------------------------------------------------------------
	-         Prepare Resource View
	---------------------------------------------------------------------*/
	_resourceView = dxDevice->CreateResourceView(core::ResourceViewType::AccelerationStructure, dxTLASBuffer, _customHeap);

	_hasBuilt = true;
}
#pragma endregion Build Function