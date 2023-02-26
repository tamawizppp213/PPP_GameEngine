//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GPUTexture.hpp
///             @brief  GPU Texture
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12GPUTexture.hpp"
#include "../Include/DirectX12GPUBuffer.hpp"
#include "../../Core/Include/DirectX12Debug.hpp"
#include "../../Core/Include/DirectX12EnumConverter.hpp"
#include "../../Core/Include/DirectX12Device.hpp"
#include "../../Core/Include/DirectX12CommandList.hpp"
#include "../../Core/Include/DirectX12CommandAllocator.hpp"
#include "../../Core/Include/DirectX12CommandQueue.hpp"
#include "../../Core/Include/DirectX12Fence.hpp"
#include "../../Core/Include/DirectX12BaseStruct.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include <DirectXTex/DirectXTex.h>
#include <wincodec.h>
#include <wincodecsdk.h>
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
using namespace DirectX;
#ifdef _DEBUG
#pragma comment(lib, "Pluguins/DirectXTex.lib")
#else
#pragma comment(lib, "Pluguins/DirectXTexd.lib")
#endif

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
namespace
{
	rhi::core::PixelFormat ConvertDXGIIntoRHICoreFormat(DXGI_FORMAT format)
	{
		switch (format)
		{
			case DXGI_FORMAT_R8G8B8A8_UNORM     : return core::PixelFormat::R8G8B8A8_UNORM;
			case DXGI_FORMAT_B8G8R8A8_UNORM     : return core::PixelFormat::B8G8R8A8_UNORM;
			case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB: return core::PixelFormat::B8G8R8A8_UNORM_SRGB;
			case DXGI_FORMAT_R16G16B16A16_FLOAT : return core::PixelFormat::R16G16B16A16_FLOAT;
			case DXGI_FORMAT_R32G32B32A32_FLOAT : return core::PixelFormat::R32G32B32A32_FLOAT;
			case DXGI_FORMAT_R32G32B32_FLOAT    : return core::PixelFormat::R32G32B32_FLOAT;
			case DXGI_FORMAT_D24_UNORM_S8_UINT  : return core::PixelFormat::D24_UNORM_S8_UINT;
			case DXGI_FORMAT_R10G10B10A2_UNORM  : return core::PixelFormat::R10G10B10A2_UNORM;
			case DXGI_FORMAT_D32_FLOAT          : return core::PixelFormat::D32_FLOAT;
			case DXGI_FORMAT_BC1_UNORM          : return core::PixelFormat::BC1_UNORM;
			default:
				throw std::runtime_error("not supported Format type");
		}
	}
}

GPUTexture::GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const std::wstring& name) : core::GPUTexture(device, name)
{
	
}

GPUTexture::GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const core::GPUTextureMetaData& metaData, const std::wstring& name)
	: core::GPUTexture(device, metaData, name)
{
	
	const auto dxDevice = static_cast<directX12::RHIDevice*>(_device.get())->GetDevice();

	/*-------------------------------------------------------------------
	-             Setting heap property
	---------------------------------------------------------------------*/
	D3D12_RESOURCE_DESC resourceDesc = {};
	ConvertDxMetaData(resourceDesc);
	AllocateGPUTextureBuffer(resourceDesc, _device->IsDiscreteGPU());
	SetName(name);
}

GPUTexture::GPUTexture(const std::shared_ptr<core::RHIDevice>& device, const ResourceComPtr& texture, const core::GPUTextureMetaData& metaData, const std::wstring& name)
	: core::GPUTexture(device, metaData, name), _resource(texture)
{
	const auto dxDevice     = static_cast<directX12::RHIDevice*>(_device.get())->GetDevice();
	const auto dxDesc       = texture->GetDesc();
	const auto allocateInfo = dxDevice->GetResourceAllocationInfo(0, 1, &dxDesc);

	// because the size of texture is not always same in different adapters
	// so we need record the real size and the alignment
	_physicalSize = static_cast<size_t>(allocateInfo.SizeInBytes);
	_alignment    = static_cast<size_t>(allocateInfo.Alignment);
	_hasAllocated = true;
}

#pragma region Public Function
void GPUTexture::SetName(const std::wstring& name)
{
	ThrowIfFailed(_resource->SetName(name.c_str()));
}

/****************************************************************************
*                     Load
*************************************************************************//**
*  @fn        void GPUTexture::Load(const std::wstring& filePath, const std::shared_ptr<core::RHICommandList>& commandList)
*
*  @brief     Load texture 
*
*  @param[in] const std::wstring& filePath
*  @param[in] const std::shared_ptr<core::RHICommandList> graphics type commandList
*
*  @return 　　void
*****************************************************************************/
void GPUTexture::Load(const std::wstring& filePath, const std::shared_ptr<core::RHICommandList>& commandList)
{

#ifdef _DEBUG
	assert(_device);
	assert(commandList);
	assert(commandList->GetCommandAllocator()->GetCommandListType() == core::CommandListType::Graphics);
#endif

	const auto dxDevice      = static_cast<directX12::RHIDevice*>(_device.get())->GetDevice();
	const auto dxCommandList = static_cast<directX12::RHICommandList*>(commandList.get())->GetCommandList();

	/*-------------------------------------------------------------------
	-                Choose Extension and Load Texture Data
	---------------------------------------------------------------------*/
	const std::wstring extension    = file::FileSystem::GetExtension(filePath);
	const std::wstring fileName     = file::FileSystem::GetFileName(filePath, false);
	
	TexMetadata  dxMetaData   = {};
	ScratchImage scratchImage = {};
	bool isDXT                = false; // 現時点では使っておりません.

	/*-------------------------------------------------------------------
	-    Select the appropriate texture loading function for each extension
	     Load texture to the cpu memory
	---------------------------------------------------------------------*/
	if (extension == L"tga")
	{
		ThrowIfFailed(LoadFromTGAFile(filePath.c_str(), TGA_FLAGS_NONE, &dxMetaData, scratchImage));
	}
	else if (extension == L"dds")
	{
		ThrowIfFailed(LoadFromDDSFile(filePath.c_str(),DDS_FLAGS_NONE, &dxMetaData, scratchImage));
		isDXT = true;
	}
	else if (extension == L"hdr")
	{
		ThrowIfFailed(LoadFromHDRFile(filePath.c_str(), &dxMetaData, scratchImage));
	}
	else
	{
		ThrowIfFailed(LoadFromWICFile(filePath.c_str(), WIC_FLAGS_NONE, &dxMetaData, scratchImage));
	}

	auto image = scratchImage.GetImage(0, 0, 0);

	/*-------------------------------------------------------------------
	-                 Create core texture metadata
	---------------------------------------------------------------------*/
	if (dxMetaData.IsCubemap())
	{
		_metaData = core::GPUTextureMetaData::CubeMap(image->width, image->height, ::ConvertDXGIIntoRHICoreFormat(dxMetaData.format), dxMetaData.mipLevels);
	}
	else if (dxMetaData.IsVolumemap())
	{
		_metaData = core::GPUTextureMetaData::Texture3D(image->width, image->height, dxMetaData.depth, ::ConvertDXGIIntoRHICoreFormat(dxMetaData.format), dxMetaData.mipLevels);
	}
	else
	{
		_metaData = core::GPUTextureMetaData::Texture2DArray(image->width, image->height, dxMetaData.arraySize,
			::ConvertDXGIIntoRHICoreFormat(dxMetaData.format), dxMetaData.mipLevels);
	}

	/*-------------------------------------------------------------------
	-                 Create directX12 texture buffer
	---------------------------------------------------------------------*/
	if (!_hasAllocated)
	{
		D3D12_RESOURCE_DESC resourceDesc = {};
		ConvertDxMetaData(resourceDesc);
		AllocateGPUTextureBuffer(resourceDesc, _device->IsDiscreteGPU());
	}

	/*-------------------------------------------------------------------
	-                 pack
	---------------------------------------------------------------------*/
	if (_device->IsDiscreteGPU())
	{
		/*-------------------------------------------------------------------
		-                 Prepare Upload Buffer Setting
		                  All mipmap data
		---------------------------------------------------------------------*/
		std::vector<D3D12_SUBRESOURCE_DATA> subResources;
		ThrowIfFailed(PrepareUpload(dxDevice.Get(), image, scratchImage.GetImageCount(), dxMetaData, subResources));

		/*-------------------------------------------------------------------
		-                 Calculate Upload Buffer Size
		---------------------------------------------------------------------*/
		const UINT64 uploadBufferSize = GetRequiredIntermediateSize(
			_resource.Get(), 0, static_cast<UINT>(subResources.size()));

		/*-------------------------------------------------------------------
		-                 Create Upload Buffer
		---------------------------------------------------------------------*/
		D3D12_HEAP_PROPERTIES heapProperty = HEAP_PROPERTY(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC   uploadDesc = RESOURCE_DESC::Buffer(uploadBufferSize);
		
		dxDevice->CreateCommittedResource(
			&heapProperty,
			D3D12_HEAP_FLAG_NONE,
			&uploadDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(_stagingBuffer.GetAddressOf())
		);

		/*-------------------------------------------------------------------
		-                 Copy Texture Data
		---------------------------------------------------------------------*/
		const auto beforeState = EnumConverter::Convert(_metaData.State);
		const auto before      = BARRIER::Transition(_resource.Get(), beforeState, D3D12_RESOURCE_STATE_COPY_DEST);

		dxCommandList->ResourceBarrier(1, &before);
		// write the staging buffer contents to the resource object.
		UpdateSubresources(dxCommandList.Get(), _resource.Get(), _stagingBuffer.Get(),
			0, 0, 
			static_cast<unsigned int>(subResources.size()), subResources.data());

		auto after = BARRIER::Transition(_resource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, beforeState);
		dxCommandList->ResourceBarrier(1, &after);
	}
	else
	{
		// WriteToSubresourceは, DiscreteGPUの場合速度が低下することが報告されているため, DiscreteGPUは別処理. 
		ThrowIfFailed(_resource->WriteToSubresource(
			0, nullptr, 
			image->pixels, 
			static_cast<UINT>(image->rowPitch), 
			static_cast<UINT>(image->slicePitch)));
	}
	_resource->SetName(fileName.c_str());
}

void GPUTexture::Save(const std::wstring& filePath, const std::shared_ptr<core::RHICommandList>& commandList, const std::shared_ptr<core::RHICommandQueue>& commandQueue)
{
#ifdef _DEBUG
	assert(commandList->GetType() == core::CommandListType::Graphics);
	assert(commandQueue->GetType() == core::CommandListType::Graphics);
#endif
	const auto dxCommandList = std::static_pointer_cast<directX12::RHICommandList>(commandList);
	dxCommandList->EndRenderPass();

	const TexMetadata dxMetaData =
	{
		.width      = _metaData.Width,
		.height     = _metaData.Height,
		.depth      = 1,
		.arraySize  = _metaData.DepthOrArraySize,
		.mipLevels  = _metaData.MipLevels,
		.miscFlags  = 0,
		.miscFlags2 = 0,
		.format     = _resource->GetDesc().Format,
		.dimension  = TEX_DIMENSION_TEXTURE2D
	};

	DirectX::Image image =
	{
		.width      = _metaData.Width,
		.height     = _metaData.Height,
		.format     = _resource->GetDesc().Format
	};

	DirectX::ComputePitch(_resource->GetDesc().Format, _metaData.Width, _metaData.Height, image.rowPitch, image.slicePitch);
	
	/*-------------------------------------------------------------------
	-       Create read back buffer to read gpu memory to the cpu memory
	---------------------------------------------------------------------*/
	auto metaData       = core::GPUBufferMetaData::UploadBuffer(core::PixelFormatSizeOf::Get(_metaData.PixelFormat), _metaData.Width * _metaData.Height, core::MemoryHeap::Readback, nullptr);
	metaData.State      = core::ResourceState::CopyDestination;
	const auto buffer   = _device->CreateBuffer(metaData);
	const auto dxBuffer = std::static_pointer_cast<GPUBuffer>(buffer);

	//Transition the resource if necessary
	const auto state = GetResourceState();
	dxCommandList->TransitionResourceState(shared_from_this(), core::ResourceState::CopySource);

	/*-------------------------------------------------------------------
	-       Get the copy target location
	---------------------------------------------------------------------*/
	D3D12_PLACED_SUBRESOURCE_FOOTPRINT bufferFootprint;
	bufferFootprint.Offset = 0;
	bufferFootprint.Footprint.Width    = static_cast<UINT>(image.width);
	bufferFootprint.Footprint.Height   = static_cast<UINT>(image.height);
	bufferFootprint.Footprint.Depth    = 1;
	bufferFootprint.Footprint.RowPitch = static_cast<UINT>(image.rowPitch);
	bufferFootprint.Footprint.Format   = image.format;

	const auto copyDestLocation = TEXTURE_COPY_LOCATION(dxBuffer->GetResourcePtr(), bufferFootprint);
	const auto copySrcLocation  = TEXTURE_COPY_LOCATION(_resource.Get(), 0);

	dxCommandList->GetCommandList()->CopyTextureRegion(&copyDestLocation, 0, 0, 0, &copySrcLocation, nullptr);
	dxCommandList->TransitionResourceState(shared_from_this(), state);

	/*-------------------------------------------------------------------
	-       Execute and Wait GPU
	---------------------------------------------------------------------*/
	const auto fence = _device->CreateFence(0, L"SaveFence");

	dxCommandList->EndRecording();
	commandQueue->Execute({ commandList });
	commandQueue->Signal(fence, 1);
	fence->Wait(1);

	/*-------------------------------------------------------------------
	-       Buffer copy
	---------------------------------------------------------------------*/
	buffer->CopyStart();
	image.pixels = buffer->GetCPUMemory();
	buffer->CopyEnd();


	std::wstring extension = file::FileSystem::GetExtension(filePath);
	/*-------------------------------------------------------------------
	-    Select the appropriate texture loading function for each extension
	---------------------------------------------------------------------*/
	if (extension == L"tga")
	{
		ThrowIfFailed(DirectX::SaveToTGAFile(image, filePath.c_str()));
	}
	else if (extension == L"dds")
	{
		ThrowIfFailed(DirectX::SaveToDDSFile(image, DDS_FLAGS_NONE, filePath.c_str()));
	}
	else if (extension == L"hdr")
	{
		ThrowIfFailed(DirectX::SaveToHDRFile(image, filePath.c_str()));
	}
	else if (extension == L"png")
	{
		ThrowIfFailed(DirectX::SaveToWICFile(image, WIC_FLAGS_NONE, GUID_ContainerFormatPng, filePath.c_str()));
	}
	else if (extension == L"bmp")
	{
		ThrowIfFailed(DirectX::SaveToWICFile(image, WIC_FLAGS_NONE, GUID_ContainerFormatBmp, filePath.c_str()));
	}
	else if (extension == L"jpeg" || extension == L"jpg")
	{
		ThrowIfFailed(DirectX::SaveToWICFile(image, WIC_FLAGS_NONE, GUID_ContainerFormatJpeg, filePath.c_str()));
	}
	else if (extension == L"ico")
	{
		ThrowIfFailed(DirectX::SaveToWICFile(image, WIC_FLAGS_NONE, GUID_ContainerFormatIco, filePath.c_str()));
	}
	else if (extension == L"tiff")
	{
		ThrowIfFailed(DirectX::SaveToWICFile(image, WIC_FLAGS_NONE, GUID_ContainerFormatTiff, filePath.c_str()));
	}
	else if (extension == L"gif")
	{
		ThrowIfFailed(DirectX::SaveToWICFile(image, WIC_FLAGS_NONE, GUID_ContainerFormatGif, filePath.c_str()));
	}
	else
	{
		throw std::runtime_error("unknown extension");

	}
	
	
}

#pragma endregion Public Function
void GPUTexture::Pack(const std::shared_ptr<core::RHICommandList>& commandList)
{
	
}

/****************************************************************************
*                     AllocateGPUTextureBuffer
*************************************************************************//**
*  @fn        void GPUTexture::AllocateGPUTextureBuffer(const D3D12_RESOURCE_DESC& resourceDesc, bool isDiscreteGPU)
*
*  @brief     Allocate texture buffer in GPU memory
*
*  @param[in] const std::wstring& filePath
*  @param[in] const std::shared_ptr<core::RHICommandList> graphics type commandList
*
*  @return 　　void
*****************************************************************************/
void GPUTexture::AllocateGPUTextureBuffer(const D3D12_RESOURCE_DESC& resourceDesc, bool isDiscreteGPU)
{
	const auto dxDevice = static_cast<directX12::RHIDevice*>(_device.get())->GetDevice();

	/*-------------------------------------------------------------------
	-             Setting heap property
	---------------------------------------------------------------------*/
	D3D12_HEAP_PROPERTIES heapProperty = {};
	if (isDiscreteGPU)
	{
		heapProperty.Type                 = D3D12_HEAP_TYPE_CUSTOM;
		heapProperty.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // write back
		heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;               // system memory (本当にいいのかは疑問)
		heapProperty.CreationNodeMask     = 1;
		heapProperty.VisibleNodeMask      = 1;
	}
	else
	{
		heapProperty.Type                 = EnumConverter::Convert(_metaData.HeapType);
		heapProperty.CPUPageProperty      = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
		heapProperty.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
		heapProperty.CreationNodeMask     = 1;
		heapProperty.VisibleNodeMask      = 1;
	}

	/*-------------------------------------------------------------------
	-             Render target or depth stencil clear value
	---------------------------------------------------------------------*/
	D3D12_CLEAR_VALUE clearValue = {};
	clearValue.Format = resourceDesc.Format;
	if (core::EnumHas(_metaData.ResourceUsage, core::ResourceUsage::RenderTarget))
	{
		clearValue.Color[0] = _metaData.ClearColor.Color[0];
		clearValue.Color[1] = _metaData.ClearColor.Color[1];
		clearValue.Color[2] = _metaData.ClearColor.Color[2];
		clearValue.Color[3] = _metaData.ClearColor.Color[3];
	}
	else if(core::EnumHas(_metaData.ResourceUsage, core::ResourceUsage::DepthStencil))
	{
		clearValue.DepthStencil.Depth   = _metaData.ClearColor.Depth;
		clearValue.DepthStencil.Stencil = _metaData.ClearColor.Stencil;
	}

	ThrowIfFailed(dxDevice->CreateCommittedResource(
		&heapProperty, D3D12_HEAP_FLAG_NONE, &resourceDesc, EnumConverter::Convert(_metaData.State),
		core::EnumHas(_metaData.ResourceUsage, core::ResourceUsage::RenderTarget) || 
		core::EnumHas(_metaData.ResourceUsage, core::ResourceUsage::DepthStencil) ? &clearValue : nullptr,
		IID_PPV_ARGS(_resource.GetAddressOf())));

	/*-------------------------------------------------------------------
	-             Get total byte size and alighment size
	---------------------------------------------------------------------*/
	const auto allocateInfo = dxDevice->GetResourceAllocationInfo(0, 1, &resourceDesc);
	_physicalSize = static_cast<size_t>(allocateInfo.SizeInBytes);
	_alignment    = static_cast<size_t>(allocateInfo.Alignment);
	_hasAllocated = true;
}

void GPUTexture::ConvertDxMetaData(D3D12_RESOURCE_DESC& resourceDesc)
{
	resourceDesc.Dimension          = EnumConverter::Convert(_metaData.Dimension);
	resourceDesc.Alignment          = 0;
	resourceDesc.Width              = _metaData.Width;
	resourceDesc.Height             = static_cast<UINT>(_metaData.Height);
	resourceDesc.DepthOrArraySize   = static_cast<UINT16>(_metaData.DepthOrArraySize);
	resourceDesc.MipLevels          = static_cast<UINT16>(_metaData.MipLevels);
	resourceDesc.Format             = EnumConverter::Convert(_metaData.PixelFormat);
	resourceDesc.SampleDesc.Count   = static_cast<UINT>(_metaData.Sample);
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout             = D3D12_TEXTURE_LAYOUT::D3D12_TEXTURE_LAYOUT_UNKNOWN;
	resourceDesc.Flags              = EnumConverter::Convert(_metaData.ResourceUsage);

}