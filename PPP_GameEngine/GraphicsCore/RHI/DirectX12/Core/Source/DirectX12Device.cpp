//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12Device.cpp
///             @brief  Logical Device : to use for create GPU resources 
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12Device.hpp"
#include "../Include/DirectX12Adapter.hpp"
#include "../Include/DirectX12CommandQueue.hpp"
#include "../Include/DirectX12CommandAllocator.hpp"
#include "../Include/DirectX12CommandList.hpp"
#include "../Include/DirectX12Swapchain.hpp"
#include "../Include/DirectX12Fence.hpp"
#include "../Include/DirectX12DescriptorHeap.hpp"
#include "../Include/DirectX12RenderPass.hpp"
#include "../Include/DirectX12ResourceLayout.hpp"
#include "../Include/DirectX12FrameBuffer.hpp"
#include "../Include/DirectX12Instance.hpp"
#include "../Include/DirectX12Query.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUPipelineState.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUSampler.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUResourceView.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUPipelineFactory.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingASInstance.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingBLASBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingTLASBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/RayTracing/Include/DirectX12RayTracingGeometry.hpp"
#include "GameUtility/Math/Include/GMMatrix.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>

#if PLATFORM_OS_WINDOWS
#include <Windows.h>
#endif

#if USE_INTEL_EXTENSION
#include <INTC/igdext.h>
#pragma comment(lib, "Plugins/INTC/igdext64.lib")
#endif

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace Microsoft::WRL;
using namespace gu;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////

#pragma region Constructor and Destructor
RHIDevice::RHIDevice()
{
	
}

RHIDevice::~RHIDevice()
{
	if (_device) { Destroy(); }
}

RHIDevice::RHIDevice(const gu::SharedPointer<core::RHIDisplayAdapter>& adapter, const core::RHIMultiGPUMask& mask) :
	core::RHIDevice(adapter, mask)
{

	/*-------------------------------------------------------------------
	-                   Create Logical Device
	---------------------------------------------------------------------*/
	ThrowIfFailed(D3D12CreateDevice(
		gu::StaticPointerCast<RHIDisplayAdapter>(_adapter)->GetAdapter().Get(),      // default adapter
		D3D_FEATURE_LEVEL_12_0, // minimum feature level
		IID_PPV_ARGS(&_device)));

	const auto& gpuName    = adapter->GetName();
	const auto  deviceName = L"Device::" + unicode::ToWString(gpuName);
	_device->SetName(deviceName.c_str());

	/*-------------------------------------------------------------------
	-                 Set gpu debug break
	---------------------------------------------------------------------*/
	SetGPUDebugBreak();

	/*-------------------------------------------------------------------
	-                   Find Highest support model
	---------------------------------------------------------------------*/
	FindHighestFeatureLevel();
	FindHighestShaderModel();

	/*-------------------------------------------------------------------
	-                   Device node count
	---------------------------------------------------------------------*/
	_deviceNodeCount = _device->GetNodeCount();

	/*-------------------------------------------------------------------
	-                   Device Support Check
	---------------------------------------------------------------------*/
	CheckDXRSupport();
	CheckVRSSupport();
	CheckRenderPassSupport();
	CheckMeshShadingSupport();
	CheckMultiSampleQualityLevels();
	CheckDepthBoundsTestSupport();
	CheckResourceTiers();
	CheckMaxHeapSize();
	CheckBindlessSupport();
	CheckStencilReferenceFromPixelShaderSupport();
	CheckSamplerFeedbackSupport();
	CheckAllowTearingSupport();
	CheckMaxRootSignatureVersion();
	CheckWaveLaneSupport();
	SetupDisplayHDRMetaData();
	SetupDefaultCommandSignatures();

#if USE_INTEL_EXTENSION
	CreateIntelExtensionContext();
#endif
	CheckAtomicOperation();
}

#pragma endregion Constructor and Destructor

#pragma region Set up and Destroy

/****************************************************************************
*                     SetUpDefaultHeap
*************************************************************************//**
*  @fn        void RHIDevice::SetUpDefaultHeap(const core::DefaultHeapCount& heapCount)
*
*  @brief     Set up default descriptor heap
*
*  @param[in] const core::DefaultHeapCount& heapCount
*
*  @return    void
*****************************************************************************/
void RHIDevice::SetUpDefaultHeap(const core::DefaultHeapCount& heapCount)
{
	/*-------------------------------------------------------------------
	-                   Create descriptor heap
	---------------------------------------------------------------------*/
	_defaultHeap[DefaultHeapType::CBV_SRV_UAV] = gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis());
	_defaultHeap[DefaultHeapType::RTV]         = gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis());
	_defaultHeap[DefaultHeapType::DSV]         = gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis());
	_defaultHeap[DefaultHeapType::Sampler]     = gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis());

	/*-------------------------------------------------------------------
	-                   Set up descriptor count
	---------------------------------------------------------------------*/
	std::map<core::DescriptorHeapType, size_t> heapInfoList;
	heapInfoList[core::DescriptorHeapType::CBV]     = heapCount.CBVDescCount;
	heapInfoList[core::DescriptorHeapType::SRV]     = heapCount.SRVDescCount;
	heapInfoList[core::DescriptorHeapType::UAV]     = heapCount.UAVDescCount;

	/*-------------------------------------------------------------------
	-                   Allocate decsriptor heap
	---------------------------------------------------------------------*/
	_defaultHeap[DefaultHeapType::CBV_SRV_UAV]->Resize(heapInfoList);
	_defaultHeap[DefaultHeapType::RTV]->Resize(core::DescriptorHeapType::RTV, heapCount.RTVDescCount);
	_defaultHeap[DefaultHeapType::DSV]->Resize(core::DescriptorHeapType::DSV, heapCount.DSVDescCount);
	_defaultHeap[DefaultHeapType::Sampler]->Resize(core::DescriptorHeapType::SAMPLER, heapCount.SamplerDescCount);

}

/****************************************************************************
*                     Destoy
*************************************************************************//**
*  @fn        void RHIDevice::Destroy()
*
*  @brief     Release command resource and device
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHIDevice::Destroy()
{
#if USE_INTEL_EXTENSION
	DestroyIntelExtensionContext();
#endif

	/*-------------------------------------------------------------------
	-              Clear default descriptor heap
	---------------------------------------------------------------------*/
	for (auto& heap : _defaultHeap)
	{
		if (heap.second) 
		{ 
			heap.second.Reset(); 
		}
	}
	_defaultHeap.clear();

	if (_drawIndexedIndirectCommandSignature) { _drawIndexedIndirectCommandSignature.Reset(); }


	/*-------------------------------------------------------------------
	-              Clear device
	---------------------------------------------------------------------*/
	if (_device) 
	{ 
#ifdef _DEBUG
		ReportLiveObjects();
#endif
		_device.Reset(); 
	}
}

#pragma endregion       Set up and Destroy

#pragma region CreateResource

gu::SharedPointer<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const std::vector<gu::SharedPointer<core::GPUTexture>>& renderTargets, const gu::SharedPointer<core::GPUTexture>& depthStencil)
{
	return gu::StaticPointerCast<core::RHIFrameBuffer>(gu::MakeShared<directX12::RHIFrameBuffer>(SharedFromThis(), renderPass, renderTargets, depthStencil));
}

gu::SharedPointer<core::RHIFrameBuffer> RHIDevice::CreateFrameBuffer(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::GPUTexture>& renderTarget, const gu::SharedPointer<core::GPUTexture>& depthStencil)
{
	return gu::StaticPointerCast<core::RHIFrameBuffer>(gu::MakeShared<directX12::RHIFrameBuffer>(SharedFromThis(), renderPass, renderTarget, depthStencil));
}

gu::SharedPointer<core::RHIFence> RHIDevice::CreateFence(const gu::uint64 fenceValue, const std::wstring& name)
{
	// https://suzulang.com/stdshared_ptr%E3%81%A7this%E3%82%92%E4%BD%BF%E3%81%84%E3%81%9F%E3%81%84%E6%99%82%E3%81%AB%E6%B3%A8%E6%84%8F%E3%81%99%E3%82%8B%E3%81%93%E3%81%A8/
	return gu::StaticPointerCast<core::RHIFence>(gu::MakeShared<directX12::RHIFence>(SharedFromThis(), fenceValue, name));
}

gu::SharedPointer<core::RHICommandList> RHIDevice::CreateCommandList(const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator, const std::wstring& name)
{
	return gu::StaticPointerCast<core::RHICommandList>(gu::MakeShared<directX12::RHICommandList>(SharedFromThis(),commandAllocator, name));
}

gu::SharedPointer<core::RHICommandQueue> RHIDevice::CreateCommandQueue(const core::CommandListType type, const std::wstring& name)
{
	return gu::StaticPointerCast<core::RHICommandQueue>(gu::MakeShared<directX12::RHICommandQueue>(SharedFromThis(), type, name));
}

gu::SharedPointer<core::RHICommandAllocator> RHIDevice::CreateCommandAllocator(const core::CommandListType type, const std::wstring& name)
{
	return gu::StaticPointerCast<core::RHICommandAllocator>(gu::MakeShared<directX12::RHICommandAllocator>(SharedFromThis(), type, name));
}

gu::SharedPointer<core::RHISwapchain> RHIDevice::CreateSwapchain(const gu::SharedPointer<core::RHICommandQueue>& commandQueue, const core::WindowInfo& windowInfo, const core::PixelFormat& pixelFormat, const size_t frameBufferCount, const gu::uint32 vsync, const bool isValidHDR )
{
	return gu::StaticPointerCast<core::RHISwapchain>(gu::MakeShared<directX12::RHISwapchain>(SharedFromThis(), commandQueue, windowInfo, pixelFormat, frameBufferCount, vsync, isValidHDR));
}

gu::SharedPointer<core::RHISwapchain>  RHIDevice::CreateSwapchain(const core::SwapchainDesc& desc)
{
	return gu::StaticPointerCast<core::RHISwapchain>(gu::MakeShared<directX12::RHISwapchain>(SharedFromThis(), desc));
}

gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const core::DescriptorHeapType heapType, const size_t maxDescriptorCount)
{
	auto heapPtr = gu::StaticPointerCast<core::RHIDescriptorHeap>(gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis()));
	heapPtr->Resize(heapType, maxDescriptorCount);
	return heapPtr;
}

gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::CreateDescriptorHeap(const std::map<core::DescriptorHeapType, size_t>& heapInfo)
{
	auto heapPtr = gu::StaticPointerCast<core::RHIDescriptorHeap>(gu::MakeShared<directX12::RHIDescriptorHeap>(SharedFromThis()));
	heapPtr->Resize(heapInfo);
	return heapPtr;
}

gu::SharedPointer<core::RHIRenderPass>  RHIDevice::CreateRenderPass(const std::vector<core::Attachment>& colors, const std::optional<core::Attachment>& depth)
{
	return gu::StaticPointerCast<core::RHIRenderPass>(gu::MakeShared<directX12::RHIRenderPass>(SharedFromThis(), colors, depth));
}

gu::SharedPointer<core::RHIRenderPass>  RHIDevice::CreateRenderPass(const core::Attachment& color, const std::optional<core::Attachment>& depth)
{
	return gu::StaticPointerCast<core::RHIRenderPass>(gu::MakeShared<directX12::RHIRenderPass>(SharedFromThis(), color, depth));
}

gu::SharedPointer<core::GPUGraphicsPipelineState> RHIDevice::CreateGraphicPipelineState(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	return gu::StaticPointerCast<core::GPUGraphicsPipelineState>(gu::MakeShared<directX12::GPUGraphicsPipelineState>(SharedFromThis(), renderPass, resourceLayout));
}

gu::SharedPointer<core::GPUComputePipelineState> RHIDevice::CreateComputePipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	return gu::StaticPointerCast<core::GPUComputePipelineState>(gu::MakeShared<directX12::GPUComputePipelineState>(SharedFromThis(), resourceLayout));
}

gu::SharedPointer<core::RHIResourceLayout> RHIDevice::CreateResourceLayout(const std::vector<core::ResourceLayoutElement>& elements, const std::vector<core::SamplerLayoutElement>& samplers, const std::optional<core::Constant32Bits>& constant32Bits, const std::wstring& name)
{
	return gu::StaticPointerCast<core::RHIResourceLayout>(gu::MakeShared<directX12::RHIResourceLayout>(SharedFromThis(), elements, samplers, constant32Bits, name));
}

gu::SharedPointer<core::GPUPipelineFactory> RHIDevice::CreatePipelineFactory()
{
	return gu::StaticPointerCast<core::GPUPipelineFactory>(gu::MakeShared<directX12::GPUPipelineFactory>(SharedFromThis()));
}

gu::SharedPointer<core::GPUResourceView> RHIDevice::CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUTexture>& texture, const gu::uint32 mipSlice, const gu::uint32 planeSlice, const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap)
{
	return gu::StaticPointerCast<core::GPUResourceView>(gu::MakeShared<directX12::GPUResourceView>(SharedFromThis(), viewType, texture, mipSlice, planeSlice, customHeap));
}

gu::SharedPointer<core::GPUResourceView> RHIDevice::CreateResourceView(const core::ResourceViewType viewType, const gu::SharedPointer<core::GPUBuffer>& buffer, const gu::uint32 mipSlice, const gu::uint32 planeSlice , const gu::SharedPointer<core::RHIDescriptorHeap>& customHeap)
{
	return gu::StaticPointerCast<core::GPUResourceView>(gu::MakeShared<directX12::GPUResourceView>(SharedFromThis(), viewType, buffer, mipSlice, planeSlice, customHeap));
}

gu::SharedPointer<core::GPUSampler> RHIDevice::CreateSampler(const core::SamplerInfo& samplerInfo)
{
	return gu::StaticPointerCast<core::GPUSampler>(gu::MakeShared<directX12::GPUSampler>(SharedFromThis(), samplerInfo));
}

gu::SharedPointer<core::GPUBuffer>  RHIDevice::CreateBuffer(const core::GPUBufferMetaData& metaData, const std::wstring& name)
{
	return gu::StaticPointerCast<core::GPUBuffer>(gu::MakeShared<directX12::GPUBuffer>(SharedFromThis(), metaData, name));
}

gu::SharedPointer<core::GPUTexture> RHIDevice::CreateTexture(const core::GPUTextureMetaData& metaData, const std::wstring& name)
{
	return gu::StaticPointerCast<core::GPUTexture>(gu::MakeShared<directX12::GPUTexture>(SharedFromThis(), metaData, name));
}

gu::SharedPointer<core::GPUTexture> RHIDevice::CreateTextureEmpty()
{
	return gu::StaticPointerCast<core::GPUTexture>(gu::MakeShared<directX12::GPUTexture>(SharedFromThis()));
}

//gu::SharedPointer<core::GPURayTracingPipelineState> RHIDevice::CreateRayTracingPipelineState(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
//{
//	return nullptr;
//}
gu::SharedPointer<core::RayTracingGeometry> RHIDevice::CreateRayTracingGeometry(const core::RayTracingGeometryFlags flags, const gu::SharedPointer<core::GPUBuffer>& vertexBuffer, const gu::SharedPointer<core::GPUBuffer>& indexBuffer)
{
	return gu::StaticPointerCast<core::RayTracingGeometry>(gu::MakeShared<directX12::RayTracingGeometry>(SharedFromThis(), flags, vertexBuffer, indexBuffer));
}

gu::SharedPointer<core::ASInstance> RHIDevice::CreateASInstance(
	const gu::SharedPointer<core::BLASBuffer>& blasBuffer, const gm::Float3x4& blasTransform,
	const gu::uint32 instanceID, const gu::uint32 instanceContributionToHitGroupIndex,
	const gu::uint32 instanceMask, const core::RayTracingInstanceFlags flags)
{
	return gu::StaticPointerCast<core::ASInstance>(gu::MakeShared<directX12::ASInstance>(SharedFromThis(), blasBuffer, blasTransform, instanceID, instanceContributionToHitGroupIndex, instanceMask, flags));
}

gu::SharedPointer<core::BLASBuffer>  RHIDevice::CreateRayTracingBLASBuffer(const std::vector<gu::SharedPointer<core::RayTracingGeometry>>& geometryDesc, const core::BuildAccelerationStructureFlags flags)
{
	return gu::StaticPointerCast<core::BLASBuffer>(gu::MakeShared<directX12::BLASBuffer>(SharedFromThis(), geometryDesc, flags));
}

gu::SharedPointer<core::TLASBuffer>  RHIDevice::CreateRayTracingTLASBuffer(const std::vector<gu::SharedPointer<core::ASInstance>>& asInstances, const core::BuildAccelerationStructureFlags flags)
{
	return gu::StaticPointerCast<core::TLASBuffer>(gu::MakeShared<directX12::TLASBuffer>(SharedFromThis(), asInstances, flags));
}

gu::SharedPointer<core::RHIQuery> RHIDevice::CreateQuery(const core::QueryHeapType heapType)
{
	return gu::StaticPointerCast<core::RHIQuery>(gu::MakeShared<directX12::RHIQuery>(SharedFromThis(), heapType));
}

/****************************************************************************
*                     CreateCommittedResource
*************************************************************************//**
*  @fn        HRESULT RHIDevice::CreateCommittedResource(ResourceComPtr& resource,const D3D12_RESOURCE_DESC& resourceDesc,const D3D12_HEAP_PROPERTIES& heapProps,const D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* clearValue)
*
*  @brief     Heap領域の確保と実際にデータをメモリに確保するのを両方行う関数
*             参考はD3D12Resources.cpp(UE5)
*
*  @param[in] const ResourceComPtr&  resource, 
*  @param[in] const D3D12_RESOURCE_DESC&  resourceの設定 : ほとんどの場合はconstですが, 設定ミスがある場合には変更が入ります
*  @param[in] D3D12_HEAP_PROPERTIES& ヒープの設定
*  @param[in] const D3D12_RESOURCE_STATES 最初に設定するresource state
*  @param[in] const D3D12_CLEAR_VALUE&    クリアカラー
* 
*  @return 　　HRESULT
*****************************************************************************/
HRESULT RHIDevice::CreateCommittedResource(ResourceComPtr& resource,
	const D3D12_RESOURCE_DESC& resourceDesc,
	const D3D12_HEAP_PROPERTIES& heapProps,
	const D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* clearValue)
{
	// Stateの初期化が必要かどうか
	const bool requireInitialize = (resourceDesc.Flags & (D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET | D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL)) != 0;
	
	/*-------------------------------------------------------------------
	-            Heap flagの設定
	---------------------------------------------------------------------*/
	D3D12_HEAP_FLAGS heapFlags = (_isSupportedHeapNotZero && !requireInitialize)
		? D3D12_HEAP_FLAG_CREATE_NOT_ZEROED : D3D12_HEAP_FLAG_NONE;

	auto localDesc = resourceDesc;

	// 共有リソース
	if (resourceDesc.Flags & D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS)
	{
		heapFlags |= D3D12_HEAP_FLAG_SHARED;

		// バッファ作成時には同時アクセスフラグは使用不可
		if (resourceDesc.Dimension == D3D12_RESOURCE_DIMENSION_BUFFER)
		{
			localDesc.Flags &= ~D3D12_RESOURCE_FLAG_ALLOW_SIMULTANEOUS_ACCESS;
		}
	}

	// RayTracingのAcceleration Structureを使用する場合
	if (initialState == D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE)
	{
		localDesc.Flags |= D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	}

	/*-------------------------------------------------------------------
	-            ヒープ領域の確保とGPUにメモリを確保する
	---------------------------------------------------------------------*/
	// 今後の方針としては, uncompressed uavを使用するときは処理を分岐する
	return _device->CreateCommittedResource(&heapProps, heapFlags, &localDesc, initialState, clearValue, IID_PPV_ARGS(resource.GetAddressOf()));;
}

/****************************************************************************
*                     CreateReservedResource
*************************************************************************//**
*  @fn        HRESULT RHIDevice::CreateReservedResource( ResourceComPtr& resource, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_HEAP_PROPERTIES& heapProp, const D3D12_RESOURCE_STATES initialState,const D3D12_CLEAR_VALUE* clearValue)
*
*  @brief     Heap内にまだマップまでは行わない予約済みのリソースを作成
*
*  @param[in] const ResourceComPtr&  resource,
*  @param[in] const D3D12_RESOURCE_DESC&  resourceの設定 : ほとんどの場合はconstですが, 設定ミスがある場合には変更が入ります
*  @param[in] D3D12_HEAP_PROPERTIES& ヒープの設定
*  @param[in] const D3D12_RESOURCE_STATES 最初に設定するresource state
*  @param[in] const D3D12_CLEAR_VALUE&    クリアカラー
*
*  @return 　　HRESULT
*****************************************************************************/
HRESULT RHIDevice::CreateReservedResource( ResourceComPtr& resource, const D3D12_RESOURCE_DESC& resourceDesc, const D3D12_HEAP_PROPERTIES& heapProp, const D3D12_RESOURCE_STATES initialState,const D3D12_CLEAR_VALUE* clearValue)
{
	/*-------------------------------------------------------------------
	-            Flagの設定
	---------------------------------------------------------------------*/
	auto localDesc   = resourceDesc;
	localDesc.Layout = D3D12_TEXTURE_LAYOUT_64KB_UNDEFINED_SWIZZLE;

	/*-------------------------------------------------------------------
	-            Heap内にまだマップまでは行わない予約済みのリソースを作成
	---------------------------------------------------------------------*/
	return _device->CreateReservedResource(&localDesc, initialState, clearValue, IID_PPV_ARGS(resource.GetAddressOf()));
}

/****************************************************************************
*                     CreatePlacedResource
*************************************************************************//**
*  @fn        HRESULT RHIDevice::CreatePlacedResource( ResourceComPtr& resource, const D3D12_RESOURCE_DESC& resourceDesc, const HeapComPtr& heap,const gu::uint64 heapOffset,const D3D12_RESOURCE_STATES initialState,const D3D12_CLEAR_VALUE* clearValue = nullptr)
*
*  @brief     既に作成済みのヒープに配置されるリソースを作成する.
*             Committed, Reserved, Placedの中では最も高速に動作する
*
*  @param[in] const ResourceComPtr&  resource,
*  @param[in] const D3D12_RESOURCE_DESC&  resourceの設定 : ほとんどの場合はconstですが, 設定ミスがある場合には変更が入ります
*  @param[in] const HeapComPtr& ヒープ
* 　@param[in] const gu::uint64 heapのoffsetバイト数
*  @param[in] const D3D12_RESOURCE_STATES 最初に設定するresource state
*  @param[in] const D3D12_CLEAR_VALUE&    クリアカラー
*
*  @return 　　HRESULT
*****************************************************************************/
HRESULT RHIDevice::CreatePlacedResource( ResourceComPtr& resource, const D3D12_RESOURCE_DESC& resourceDesc, const HeapComPtr& heap, const gu::uint64 heapOffset, const D3D12_RESOURCE_STATES initialState, const D3D12_CLEAR_VALUE* clearValue )
{
	return _device->CreatePlacedResource(heap.Get(), heapOffset, &resourceDesc, initialState, clearValue, IID_PPV_ARGS(resource.GetAddressOf()));
}

#pragma endregion           Create Resource Function

#pragma region Debug Function

/****************************************************************************
*                     ReportLiveObjects
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::ReportLiveObjects()
* 
*  @brief     ReportLiveObjects
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHIDevice::ReportLiveObjects()
{
#ifdef _DEBUG
	ID3D12DebugDevice2* debugInterface = nullptr;
	if (SUCCEEDED(_device.Get()->QueryInterface(&debugInterface)))
	{
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL); // Only Objects with external reference counts
		debugInterface->Release();
	}
#endif
}

#pragma endregion           Debug Function

#pragma region Device Support Function
/****************************************************************************
*						CheckDXRSupport
*************************************************************************//**
*  @fn        void DirectX12::CheckDXRSupport()
* 
*  @brief     Check DXRSupport
* 
*  @param[in] void
* 
*  @return 　　void
* 
*  @details   Tier1_1の場合,　以下の内容が可能となります 
　　　　　　　　　・ExecuteIndirectを介したDispatchRays呼び出しのサポート, RayQueryの使用
* 　　　　　　　　・AddToStateObjectを介した既存の状態オブジェクトへの増分追加
　　　　　　　　　・SkipTriangles, skip procedual primitivesのフラグの使用
		 　　　　https://github.com/microsoft/DirectX-Specs/blob/master/d3d/Raytracing.md#d3d12_raytracing_tier　
			   https://devblogs.microsoft.com/directx/dxr-1-1/　
*****************************************************************************/
void RHIDevice::CheckDXRSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options{};
		
	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options, UINT(sizeof(options))))) 
	{
		_isSupportedRayTracing = false;
		return; 
	}

	_rayTracingTier        = options.RaytracingTier;
	_isSupportedRayTracing = options.RaytracingTier   >= D3D12_RAYTRACING_TIER_1_0;
	_isSupportedRayQuery   = options.RaytracingTier   >= D3D12_RAYTRACING_TIER_1_1;
}

/****************************************************************************
*						CheckRenderPassSupport
*************************************************************************//**
*  @fn        void RHIDevice::CheckRenderPassSupport()
*
*  @brief     Check render pass support
*
*  @param[in] void
*
*  @return 　　void
*
*  @details   Tier0 : レンダーパスは未実装, 一応ソフトウェアエミュレーションを介してのみ提供とは言っているので, サポート対象とはいたします. 
*             Tier1 : Render Target, Depth Bufferの書き込みの高速化, しかし, UAVの書き込みはRenderPass内では効率的にはならないとのこと
*             Tier2 : Render Target, Depth Bufferの書き込みの高速化, UAVの書き込みもOK
*****************************************************************************/
void RHIDevice::CheckRenderPassSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options{};

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options, UINT(sizeof(options)))))
	{
		_isSupportedRenderPass = false;
		_renderPassTier        = D3D12_RENDER_PASS_TIER_0; 
		return;
	}

	_isSupportedRenderPass = options.RenderPassesTier >= D3D12_RENDER_PASS_TIER_0;
	_renderPassTier        = options.RenderPassesTier;
}

/****************************************************************************
*						CheckSamplerFeedbackSupport
*************************************************************************//**
*  @fn        void RHIDevice::CheckSamplerFeedbackSupport()
*
*  @brief     Check sampelr feedback support
*
*  @param[in] void
*
*  @return 　　void
*
*  @details   Sampler feedbackは, テクスチャサンプリング情報と位置をキャプチャ, 記録するための機能. 
*             https://microsoft.github.io/DirectX-Specs/d3d/SamplerFeedback.html
*             Tier 0.9 : D3D12_TEXTURE_ADDRESS_MODE_WRAP, D3D12_TEXTURE_ADDRESS_MODE_CLAMPのみに対応する, mipmapも0のみ対応
* 　　　　　　　 Tier 1.0 : 全てのTexture addressing modeで使用可能
*****************************************************************************/
void RHIDevice::CheckSamplerFeedbackSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS7 options{};

	if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &options, UINT(sizeof(options)))))
	{
		_isSupportedSamplerFeedback = options.SamplerFeedbackTier >= D3D12_SAMPLER_FEEDBACK_TIER_0_9;
		_samplerFeedbackTier        = options.SamplerFeedbackTier;
	}
	else
	{
		_isSupportedSamplerFeedback = false;
		_samplerFeedbackTier        = D3D12_SAMPLER_FEEDBACK_TIER_NOT_SUPPORTED;
	}

}

/****************************************************************************
*						CheckBindlessSupport
*************************************************************************//**
*  @fn        void RHIDevice::CheckBindlessSupport()
*
*  @brief     Check bindless support
*
*  @param[in] void
*
*  @return 　　void
*
*  @details   通常はRootParameterを使って各リソースビューとグラフィックパイプラインの紐づけを行うが, 
*             DescriptorHeapのインデックスだけを使ってバインドし, これらのインデクスを全てRootConstantに配置可能.
*             https://rtarun9.github.io/blogs/bindless_rendering/
*****************************************************************************/
void RHIDevice::CheckBindlessSupport()
{
	if (GetMaxSupportedShaderModel() >= D3D_SHADER_MODEL_6_6 && _resourceBindingTier == D3D12_RESOURCE_BINDING_TIER_3)
	{
		_isSupportedBindless = true;
		_bindlessResourceType = GetMaxSupportedFeatureLevel() <= D3D_FEATURE_LEVEL_11_1 ? core::BindlessResourceType::OnlyRayTracing : core::BindlessResourceType::AllShaderTypes;
	}
}
/****************************************************************************
*                     CheckVRSSupport
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CheckVRSSupport()
* 
*  @brief     Variable Rate Shading support
* 
*  @param[in] void
* 
*  @return 　　void
* 
*  @details   可変レートシェーディングは, 複数ピクセルの塊を1ピクセルとして計算する手法です. 
*             Tier1はDrawCallごとに指定でき, 描画される対象に一律に適用します.
*             Tier2はDrawCallだけでなく, プリミティブごと, タイルごとにも設定できるようになります. 
*             共通して, 1x1, 1x2, 2x1, 2x2は全てのレベルでサポートされているものの, 2x4, 4x2, 4x4はAdditionalShadingRatesSupportedを確認する必要があります.
*             https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/vrs
*             https://sites.google.com/site/monshonosuana/directx%E3%81%AE%E8%A9%B1/directx%E3%81%AE%E8%A9%B1-%E7%AC%AC168%E5%9B%9E
*****************************************************************************/
void RHIDevice::CheckVRSSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS6 options = {};
	if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS6, &options, sizeof(options))))
	{
		_variableRateShadingTier = options.VariableShadingRateTier;

		if (_variableRateShadingTier != D3D12_VARIABLE_SHADING_RATE_TIER_NOT_SUPPORTED)
		{
			_isSupportedLargerVariableRateShadingSize = options.AdditionalShadingRatesSupported;
		}

		if (options.VariableShadingRateTier == D3D12_VARIABLE_SHADING_RATE_TIER_1)
		{
			OutputDebugStringA("Gpu api: Variable Rate Shading Tier1 supported\n");
			_isSupportedVariableRateShadingTier1 = true;
			_isSupportedVariableRateShadingTier2 = false;
			_variableRateShadingImageTileSize    = 1;    // Tier2のみの機能なので必要なし
		}
		else if (options.VariableShadingRateTier == D3D12_VARIABLE_SHADING_RATE_TIER_2)
		{
			OutputDebugStringA("Gpu api: Valiable Rate Shading Tier2 supported\n");
			_isSupportedVariableRateShadingTier1 = true;
			_isSupportedVariableRateShadingTier2 = true;
			_variableRateShadingImageTileSize = options.ShadingRateImageTileSize; // 8, 16, 32が返される
		}
		else
		{
			OutputDebugStringA("Gpu api: Variable Rate Shading not supported");
			_isSupportedVariableRateShadingTier1 = false;
			_isSupportedVariableRateShadingTier2 = false;
			_variableRateShadingImageTileSize = 1;
		}
	}
	else
	{
		OutputDebugStringA("GpuApi : Variable Rate Shading note supported on current gpu hardware.\n");
		_isSupportedVariableRateShadingTier1 = false;
		_isSupportedVariableRateShadingTier2 = false;
		_variableRateShadingImageTileSize = 1;
	}
}

/****************************************************************************
*                     MultiSampleQualityLevels
*************************************************************************//**
*  @fn        void DirectX12::CheckMultiSampleQualityLevels(void)
* 
*  @brief     Multi Sample Quality Levels for Msaa (Anti-Alias)
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckMultiSampleQualityLevels()
{
	for (gu::uint32 sampleCount = DESIRED_MAX_MSAA_SAMPLE_COUNT; sampleCount > 0; sampleCount--)
	{
		D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels = {};
		msQualityLevels.SampleCount = sampleCount;

		if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels))))
		{
			_msaaQuality     = msQualityLevels.NumQualityLevels;
			_maxMSAASampleCount = sampleCount;
			break;
		}
	}
}

/****************************************************************************
*                     CheckHDRDisplaySupport
*************************************************************************//**
*  @fn        void DirectX12::CheckHDRDisplaySupport()
* 
*  @brief     CheckHDRDisplaySupport()　https://qiita.com/dgtanaka/items/672d2e7b3152f4e5ed49
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHIDevice::SetupDisplayHDRMetaData()
{
	/*-------------------------------------------------------------------
	-               Get display adapter
	---------------------------------------------------------------------*/
	const auto adapter = gu::StaticPointerCast<directX12::RHIDisplayAdapter>(_adapter);
	auto dxAdapter     = adapter->GetAdapter();

	ComPtr<IDXGIOutput> output = nullptr;

	/*-------------------------------------------------------------------
	-               AdapterはAMD or Nvidiaであれば処理を実行する
	---------------------------------------------------------------------*/
	if (!(adapter->IsAdapterAMD() || adapter->IsAdapterNVIDIA()))
	{
		_isSupportedHDR = false; 
		return;
	}

	/*-------------------------------------------------------------------
	-              Search EnableHDR output
	---------------------------------------------------------------------*/
	for(uint32 i = 0; dxAdapter->EnumOutputs(i, output.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
#if DXGI_MAX_OUTPUT_INTERFACE >= 6
		
		OutputComPtr output6 = nullptr;

		if (SUCCEEDED(output->QueryInterface(IID_PPV_ARGS(output6.GetAddressOf()))))
		{
			DXGI_OUTPUT_DESC1 desc = {};
			output6->GetDesc1(&desc);

			/*-------------------------------------------------------------------
			-              IS HDR Enable Color Space
			---------------------------------------------------------------------*/
			if (desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020)
			{
				_isSupportedHDR = true;

				/*-------------------------------------------------------------------
				-              Set up hdr display information
				---------------------------------------------------------------------*/
				_displayInfo = 
				{
					core::DisplayColorGamut::Rec2020_D65,
					desc.MaxLuminance <= 1000.0f ? core::DisplayOutputFormat::HDR_ACES_1000nit_ST2084 : core::DisplayOutputFormat::HDR_ACES_2000nit_ST2084,
					{desc.RedPrimary  [0], desc.RedPrimary  [1]},
					{desc.GreenPrimary[0], desc.GreenPrimary[1]},
					{desc.BluePrimary [0], desc.BluePrimary [1]},
					{desc.WhitePoint  [0], desc.WhitePoint  [1]},
					{desc.MinLuminance},
					{desc.MaxLuminance},
					{desc.MaxFullFrameLuminance},
					{desc.DesktopCoordinates.left, desc.DesktopCoordinates.top, desc.DesktopCoordinates.right, desc.DesktopCoordinates.bottom},
				};


				output.Reset();
				break;
			}
		}
		
#endif
		output.Reset();
	}

	_isSupportedHDR = false;
}

/****************************************************************************
*                     CheckMeshShadingSupport
*************************************************************************//**
*  @fn        void DirectX12::CheckMeshShadingSupport()
* 
*  @brief     Mesh Shading support check
* 
*  @param[in] void
* 
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckMeshShadingSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS7 options{};

	if (FAILED(_device->CheckFeatureSupport(
		D3D12_FEATURE_D3D12_OPTIONS7, &options, UINT(sizeof(options)))))
	{
		_isSupportedMeshShading = false; return;
	}

	_isSupportedMeshShading = options.MeshShaderTier >= D3D12_MESH_SHADER_TIER_1;

	gu::details::StringBase<wchar, 2> test;
	test.Assign(L"abcdfgagahahagagagaga");
	const auto a  =test.FindLastIndexOf(L"aga");

	
	printf("%llu", test.Size());

}

/****************************************************************************
*                     CheckAllowTearingSupport
*************************************************************************//**
*  @fn        void RHIDevice::CheckAllowTearingSupport()
*
*  @brief     Allow tearing support
*            
*  @param[in] void
*
*  @return 　　void
* 
*  @details   可変レートのリフレッシュレートを使用するときに使用します. 
*             画面のティアリング (準備できてないときにフリップ処理が入ってしまう画面のちらつき)が生じないように, 次のフレームの準備がおきるまで遅延をしてくれる機能です.
*             ただし, VSyncは0にしてください. 
*****************************************************************************/
void RHIDevice::CheckAllowTearingSupport()
{
#if DXGI_MAX_FACTORY_INTERFACE >= 5
	const auto dxInstance = static_cast<directX12::RHIInstance*>(_adapter->GetInstance());
	const auto factory    = dxInstance->GetFactory();
	
	BOOL allowTearing = FALSE; // Boolはint型なので, boolだと×
	if (SUCCEEDED(factory->CheckFeatureSupport(DXGI_FEATURE_PRESENT_ALLOW_TEARING, &allowTearing, sizeof(allowTearing))))
	{
		_isSupportedAllowTearing = allowTearing;
	}
	else
	{
		_isSupportedAllowTearing = false;
	}
#endif
}

/****************************************************************************
*                     DepthBoundsTestSupport
*************************************************************************//**
*  @fn        void RHIDevice::CheckDepthBoundsTestSupport()
*
*  @brief      深度値が指定の範囲に入っているかをテストし, 範囲内ならばピクセルシェーダーを動作させ, 範囲外ならば該当ピクセルを早期棄却する方法
		　　　　 Deferred Renderingにおけるライトのaccumulation, Deferred RenderingにおけるCascaded Shadow Map, 被写界深度エフェクト, 遠景描画等に使用可能 
*              https://microsoft.github.io/DirectX-Specs/d3d/DepthBoundsTest.html
*              https://shikihuiku.wordpress.com/tag/depthboundstest/
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckDepthBoundsTestSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS2 options{};
	if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS2, &options, sizeof(options))))
	{
		_isSupportedDepthBoundsTest = options.DepthBoundsTestSupported;
	}
	else
	{
		_isSupportedDepthBoundsTest = false;
	}
}

/****************************************************************************
*                     CheckResourceTiers
*************************************************************************//**
*  @fn        void RHIDevice::CheckResourceTiers()
*
*  @brief     パイプラインで使用可能なリソースの上限値を確認するために使用する
* 　　　　　　　　
*  @param[in] void
*
*  @return 　　void
*  
*  @details    大きく異なるのは以下の点です
*             1. CBV : Tier 1, 2は14まで. Tier3 はDescripterHeapの最大数
*             2. SRV : Tier 1は128まで. Tier2, 3はDescripterHeapの最大数
*             3. UAV : Tier1は機能レベル 11.1+以上で64, それ以外で8, Tier2は64, Tier3はDescripterHeapの最大数
*             4. Sampler : Tier1は16, それ以外で2048
*             5. ヒープ内のDescripterの最大数 Tier1, 2は1,000,000、Tier3は無制限
* 　　　　　　　　https://learn.microsoft.com/ja-jp/windows/win32/direct3d12/hardware-support
* 
* 　　　　　　　　HeapTierの方では, バッファー, RenderTargetとDepthStencil, TargetStencil, 深度ステンシルテクスチャのレンダリングを同一ヒープで使用できるかを調べます
* 　　　　　　　　Tier1が排他, Tier2が混在可能です.
* 　　　　　　　　https://learn.microsoft.com/ja-jp/windows/win32/api/d3d12/ne-d3d12-d3d12_resource_heap_tier
*****************************************************************************/
void RHIDevice::CheckResourceTiers()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS options{};
	
	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options)))) { return; }
	_resourceBindingTier = options.ResourceBindingTier;
	_resourceHeapTier    = options.ResourceHeapTier;
}

/****************************************************************************
*                     CheckStencilReferenceFromPixelShaderSupport
*************************************************************************//**
*  @fn        void RHIDevice::CheckStencilReferenceFromPixelShaderSupport()
*
*  @brief     ステンシルバッファの参照値をピクセルシェーダーで出力出来るようにします. 
*             https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/shader-specified-stencil-reference-value
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckStencilReferenceFromPixelShaderSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS options{};

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS, &options, sizeof(options)))) { return; }
	_isSupportedStencilReferenceFromPixelShader = options.PSSpecifiedStencilRefSupported != 0;
}
/****************************************************************************
*                     CheckMaxHeapSize()
*************************************************************************//**
*  @fn        void RHIDevice::CheckMaxHeapSize()
*
*  @brief     最大のHeap Sizeをセットします.
* 
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckMaxHeapSize()
{
	if (_resourceBindingTier == D3D12_RESOURCE_BINDING_TIER_1)
	{
		_maxDescriptorHeapCount = D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_1;
	}
	else if (_resourceBindingTier == D3D12_RESOURCE_BINDING_TIER_2)
	{
		_maxDescriptorHeapCount = D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2;
	}
	else if (_resourceBindingTier == D3D12_RESOURCE_BINDING_TIER_3)
	{
		_maxDescriptorHeapCount = D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2;
	}
	else
	{
		_maxDescriptorHeapCount = D3D12_MAX_SHADER_VISIBLE_DESCRIPTOR_HEAP_SIZE_TIER_2;
	}

	_maxSamplerHeapCount = D3D12_MAX_SHADER_VISIBLE_SAMPLER_HEAP_SIZE;
}
/****************************************************************************
*                     FindHighestFeatureLevel
*************************************************************************//**
*  @fn        void RHIDevice::FindHighestFeatureLevel()
*
*  @brief     Set up max feature level.
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::FindHighestFeatureLevel()
{
	const D3D_FEATURE_LEVEL featureLevels[] =
	{
#if D3D12_CORE_ENABLED
		D3D_FEATURE_LEVEL_12_2,
#endif
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0
	};

	D3D12_FEATURE_DATA_FEATURE_LEVELS featureLevelCaps = {};
	featureLevelCaps.pFeatureLevelsRequested = featureLevels;
	featureLevelCaps.NumFeatureLevels        = _countof(featureLevels);

	/*-------------------------------------------------------------------
	-               Feature Support Check
	---------------------------------------------------------------------*/
	ThrowIfFailed(_device->CheckFeatureSupport(D3D12_FEATURE_FEATURE_LEVELS, &featureLevelCaps, sizeof(featureLevelCaps)));
	_maxSupportedFeatureLevel = featureLevelCaps.MaxSupportedFeatureLevel;
}

/****************************************************************************
*                     FindHighestShaderModel
*************************************************************************//**
*  @fn        void RHIDevice::FindHighestShaderModel()
*
*  @brief     Set up max shader model.
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::FindHighestShaderModel()
{
	const D3D_SHADER_MODEL shaderModels[] =
	{
#if D3D12_CORE_ENABLED
		D3D_HIGHEST_SHADER_MODEL,
		D3D_SHADER_MODEL_6_7,
		D3D_SHADER_MODEL_6_6,
#endif
		D3D_SHADER_MODEL_6_5,
		D3D_SHADER_MODEL_6_4,
		D3D_SHADER_MODEL_6_3,
		D3D_SHADER_MODEL_6_2,
		D3D_SHADER_MODEL_6_1,
		D3D_SHADER_MODEL_6_0,
	};

	D3D12_FEATURE_DATA_SHADER_MODEL featureShaderModel = {};
	for (const auto shaderModel : shaderModels)
	{
		featureShaderModel.HighestShaderModel = shaderModel;

		if (SUCCEEDED(_device->CheckFeatureSupport(D3D12_FEATURE_SHADER_MODEL, &featureShaderModel, sizeof(featureShaderModel))))
		{
			_maxSupportedShaderModel = featureShaderModel.HighestShaderModel;
			return;
		}
	}
}

/****************************************************************************
*                     CheckWaveLaneSupport
*************************************************************************//**
*  @fn        void RHIDevice::CheckWaveLaneSupport()
*
*  @brief      HLSLで明示的にGPU上で複数スレッドの使用が可能となります.
		       Wave : プロセッサ上の同時に実行されるスレッドの集合
			   Lane : 個々のスレッド
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckWaveLaneSupport()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS1 options{};

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS1, &options, sizeof(options)))) { return; }
	_isSupportedWaveLane = !!options.WaveOps;
	_minWaveLaneCount    = options.WaveLaneCountMin;
	_maxWaveLaneCount    = options.WaveLaneCountMax;
}

/****************************************************************************
*                     CheckNative16bitOperation
*************************************************************************//**
*  @fn        void RHIDevice::CheckNative16bitOperation()
*
*  @brief     16 bitのシェーダー操作が可能かどうかを調べます
* 
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckNative16bitOperation()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS4 options{};

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS4, &options, sizeof(options)))) { return; }
	_isSupported16bitOperation = !!options.Native16BitShaderOpsSupported;
}

/****************************************************************************
*                     CheckAtomicOperation
*************************************************************************//**
*  @fn        void RHIDevice::CheckAtomicOperation()
*
*  @brief     Wave用にAtomic操作が可能かどうかを調べます.
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckAtomicOperation()
{
	D3D12_FEATURE_DATA_D3D12_OPTIONS7 options7{};
	D3D12_FEATURE_DATA_D3D12_OPTIONS9 options9{};
	D3D12_FEATURE_DATA_D3D12_OPTIONS11 options11{};

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS7, &options7, sizeof(options7)))) { return; }
	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS9, &options9, sizeof(options9)))) { return; }
	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS11, &options11, sizeof(options11)))) { return; }

	_isSupportedAtomicOperation = !!options9.AtomicInt64OnTypedResourceSupported
#if USE_INTEL_EXTENSION
		|| IsSupportedIntelEmulatedAtomic64();
#else
		;
#endif

	_isSupportedAtomicInt64OnTypedResource             = !!options9.AtomicInt64OnTypedResourceSupported;
	_isSupportedAtomicInt64OnGroupSharedSupported      = !!options9.AtomicInt64OnGroupSharedSupported;
	_isSupportedInt64OnDescriptorHeapResourceSupported = !!options11.AtomicInt64OnDescriptorHeapResourceSupported;
	_isSupportedAtomicUInt64 = _isSupportedAtomicInt64OnTypedResource && _isSupportedInt64OnDescriptorHeapResourceSupported;
}

/****************************************************************************
*                     CheckMaxRootSignatureVersion
*************************************************************************//**
*  @fn        void RHIDevice::CheckMaxRootSignatureVersion()
*
*  @brief     RootSignatureの最新バージョンを調べます
*             1_0 or 1: Default
*             1_1     : Descriptorに対して最適化を行うためのフラグを設置可能
*
*  @param[in] void
*
*  @return 　　void
*****************************************************************************/
void RHIDevice::CheckMaxRootSignatureVersion()
{
	D3D12_FEATURE_DATA_ROOT_SIGNATURE options = {};
	options.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;

	if (FAILED(_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &options, sizeof(options))))
	{
		options.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}

	_maxRootSignatureVersion = options.HighestVersion;
}

#if USE_INTEL_EXTENSION

/****************************************************************************
*                     CreateIntelExtensionContext
*************************************************************************//**
*  @fn        INTCExtensionContext* RHIDevice::CreateIntelExtensionContext(INTCExtensionInfo& intelExtensionInfo)
*
*  @brief     INTCExtensionContextを生成します.
*
*  @param[out]void
*
*  @return    void
*****************************************************************************/
void RHIDevice::CreateIntelExtensionContext()
{
	_isSupportedIntelEmulatedAtomic64 = false;

	/*-------------------------------------------------------------------
	-         ドライバーのインストールディレクトリを検索し, igdext64.dllを見つけます
	---------------------------------------------------------------------*/
	if (FAILED(INTC_LoadExtensionsLibrary(false)))
	{
		_RPT0(_CRT_WARN, "Failed to load Intel Extension Library\n");
	}

	/*-------------------------------------------------------------------
	-         サポートされているバージョン数の取得
	---------------------------------------------------------------------*/
	std::vector<INTCExtensionVersion> supportedExtensionsVersions = {};
	gu::uint32 supportedExtensionVersionCount = 0;

	if (FAILED(INTC_D3D12_GetSupportedVersions(_device.Get(), nullptr, &supportedExtensionVersionCount)))
	{
		return;
	}

	// サポートされているバージョン数だけインスタンス作成
	supportedExtensionsVersions.resize(supportedExtensionVersionCount);

	// 目標のバージョン
	const INTCExtensionVersion atomicsRequiredVersion =
	{
		.HWFeatureLevel = 4,
		.APIVersion     = 8,
		.Revision       = 0
	};

	/*-------------------------------------------------------------------
	-         サポートされているバージョン情報を取得する
	---------------------------------------------------------------------*/
	if (FAILED(INTC_D3D12_GetSupportedVersions(_device.Get(), supportedExtensionsVersions.data(), &supportedExtensionVersionCount)))
	{
		return;
	}

	_RPT0(_CRT_WARN, "/////////////////////////////////////////////////\n");
	_RPT0(_CRT_WARN, " Supported Extension Versions in this driver: \n");
	_RPT0(_CRT_WARN, "/////////////////////////////////////////////////\n");

	INTCExtensionInfo intelExtensionInfo = {};
	for (uint32 i = 0; i < supportedExtensionVersionCount; ++i)
	{
		if((supportedExtensionsVersions[i].HWFeatureLevel >= atomicsRequiredVersion.HWFeatureLevel) && 
		   (supportedExtensionsVersions[i].APIVersion     >= atomicsRequiredVersion.APIVersion    ) &&  
		   (supportedExtensionsVersions[i].Revision       >= atomicsRequiredVersion.Revision     ))
		{
			_RPTN(_CRT_WARN, "Intel Extension loaded requested version: %u.%u.%u\n",
				&supportedExtensionsVersions[i].HWFeatureLevel,
				&supportedExtensionsVersions[i].APIVersion,
				&supportedExtensionsVersions[i].Revision);

			intelExtensionInfo.RequestedExtensionVersion = supportedExtensionsVersions[i];
			break;
		}
	}

	/*-------------------------------------------------------------------
	-         サポートされたバージョンのDeviceを作成する
	---------------------------------------------------------------------*/
	INTCExtensionContext* intelExtensionContext = nullptr;

	INTCExtensionAppInfo applicationInfo = {};
	applicationInfo.pEngineName   = L"PPP_Engine";
	applicationInfo.EngineVersion = 1;

	const auto result = INTC_D3D12_CreateDeviceExtensionContext(_device.Get(), &intelExtensionContext, &intelExtensionInfo, &applicationInfo);
	
	if (SUCCEEDED(result))
	{
		_RPT0(_CRT_WARN, "Intel Extension Framework enabled.\n");
	}
	else
	{
		if (result == E_NOINTERFACE)
		{
			_RPT0(_CRT_WARN, "Intel Extensions Framework not supported by driver. Please check if a driver update is available\n");
		}
		else if (result == E_INVALIDARG)
		{
			_RPT0(_CRT_WARN, "Intel Extensions Framework passed invalid creation arguments\n");
		}
		else
		{
			_RPT0(_CRT_WARN, "Intel Extensions Framework failed to initialize\n");
		}
	}

	/*-------------------------------------------------------------------
	-         終了処理
	---------------------------------------------------------------------*/
	if (!supportedExtensionsVersions.empty())
	{
		supportedExtensionsVersions.clear();
		supportedExtensionsVersions.shrink_to_fit();
	}

	_intelExtensionContext = intelExtensionContext;
	_isSupportedIntelEmulatedAtomic64 = true;
}


/****************************************************************************
*                     DestroyIntelExtensionContext
*************************************************************************//**
*  @fn        void RHIDevice::DestroyIntelExtensionContext()
*
*  @brief     INTCExtensionContextを破棄します. 
*
*  @param[in] INTCExtensionContext* 
*
*  @return    void
*****************************************************************************/
void RHIDevice::DestroyIntelExtensionContext()
{
	if (_intelExtensionContext == nullptr) { return; }

	/*-------------------------------------------------------------------
	-              INTCExtensionContextがあった場合は破棄を行う
	---------------------------------------------------------------------*/
	const auto result = INTC_DestroyDeviceExtensionContext(&_intelExtensionContext);

	/*-------------------------------------------------------------------
	-              表示
	---------------------------------------------------------------------*/
#if PLATFORM_OS_WINDOWS
	if (result == S_OK)
	{
		OutputDebugStringA("Intel Extensions Framework unloaded\n");
	}
	else
	{
		OutputDebugStringA("Intel Extensions Framework error when unloading\n");
	}
#else
	if (result == S_OK)
	{
		_RPT0(_CRT_WARN, "Intel Extensions Framework unloaded\n");
	}
	else
	{
		_RPT0(_CRT_WARN, "Intel Extensions Framework error when unloading\n");
	}
#endif // PLATFORM_OS_WINDOWS
}

/****************************************************************************
*                     IsSupportedIntelEmulatedAtomic64
*************************************************************************//**
*  @fn        bool RHIDevice::IsSupportedIntelEmulatedAtomic64()
*
*  @brief     Atomic 64 bitがサポートされているかを返します.
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool RHIDevice::IsSupportedIntelEmulatedAtomic64()
{
	/*-------------------------------------------------------------------
	-          Use intel adapter
	---------------------------------------------------------------------*/
	const auto rhiAdapter = static_cast<directX12::RHIDisplayAdapter*>(_adapter.Get());

	if (!rhiAdapter->IsAdapterIntel()) { return false; }

	/*-------------------------------------------------------------------
	-          Created intel extension context
	---------------------------------------------------------------------*/
	return _intelExtensionContext && _isSupportedIntelEmulatedAtomic64;
}
#endif // USE_INTEL_EXTENSION


#pragma endregion  Device Support Function

#pragma region Property
/****************************************************************************
*                     SetName
*************************************************************************//**
*  @fn        void RHIDevice::SetName(const std::wstring& name)
*
*  @brief     Set Logical device name
*
*  @param[in] const std::wstring& name
*
*  @return    void
*****************************************************************************/
void RHIDevice::SetName(const std::wstring& name)
{
	_device->SetName(name.c_str());
}

/****************************************************************************
*                     GetDefaultHeap
*************************************************************************//**
*  @fn        gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::GetDefaultHeap(const core::DescriptorHeapType heapType)
*
*  @brief     Return descriptor heap (CBV, SRV, UAV, RTV, DSV)
*
*  @param[in] const core::DefaultHeapType
*
*  @return    gu::SharedPointer<core::RHIDescriptorHeap>
*****************************************************************************/
gu::SharedPointer<core::RHIDescriptorHeap> RHIDevice::GetDefaultHeap(const core::DescriptorHeapType heapType)
{
	switch (heapType)
	{
		case core::DescriptorHeapType::CBV:
		case core::DescriptorHeapType::SRV:
		case core::DescriptorHeapType::UAV: { return _defaultHeap[DefaultHeapType::CBV_SRV_UAV];}
		case core::DescriptorHeapType::RTV: { return _defaultHeap[DefaultHeapType::RTV]; }
		case core::DescriptorHeapType::DSV: { return _defaultHeap[DefaultHeapType::DSV]; }
		default: { return nullptr;}
	}
}

/****************************************************************************
*                     SetGPUDebugBreak
*************************************************************************//**
*  @fn        void RHIDevice::SetGPUDebugBreak()
*
*  @brief     Set gpu debug break
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHIDevice::SetGPUDebugBreak()
{
	/*-------------------------------------------------------------------
	-              GPU debug breakを使用するか
	---------------------------------------------------------------------*/
	const auto dxInstance = static_cast<rhi::directX12::RHIInstance*>(_adapter->GetInstance());
	if (!dxInstance->UseGPUDebugBreak()) { return; }

	/*-------------------------------------------------------------------
	-              Info queueが存在しているか
	---------------------------------------------------------------------*/
	InfoQueuePtr infoQueue = nullptr;
	_device.As(&infoQueue);

	if (!infoQueue) { return; }

	/*-------------------------------------------------------------------
	-              深刻度の設定 (現在はerrorのみ対応いたします.)
	---------------------------------------------------------------------*/
	infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
}

/****************************************************************************
*                     SetupDefaultCommandSignatures
*************************************************************************//**
*  @fn        void RHIDevice::SetupDefaultCommandSignatures()
*
*  @brief     Set up command signatures
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHIDevice::SetupDefaultCommandSignatures()
{
	/*-------------------------------------------------------------------
	-              ExecuteIndirect draw index command signatures
	---------------------------------------------------------------------*/
	{
		D3D12_INDIRECT_ARGUMENT_DESC indirectArgumentDesc;
		indirectArgumentDesc.Type = D3D12_INDIRECT_ARGUMENT_TYPE_DRAW_INDEXED;

		const D3D12_COMMAND_SIGNATURE_DESC commandSignatureDesc =
		{
			.ByteStride = sizeof(D3D12_DRAW_INDEXED_ARGUMENTS),
			.NumArgumentDescs = 1,
			.pArgumentDescs = &indirectArgumentDesc,
			.NodeMask = 0
		};

		ThrowIfFailed(_device->CreateCommandSignature(&commandSignatureDesc, nullptr, IID_PPV_ARGS(_drawIndexedIndirectCommandSignature.GetAddressOf())));
	}
}
#pragma endregion Property
