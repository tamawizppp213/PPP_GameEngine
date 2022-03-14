//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GraphicsDevice.cpp
///             @brief  Graphics Device
///             @author Toide Yutaro
///             @date   -
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12GraphicsDevice.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Include.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12Debug.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace Microsoft::WRL;
enum class GPUVender
{
	NVidia,
	AMD,
	Intel,
	CountOfGPUVender
};
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Public Function
/****************************************************************************
*							Initialize
*************************************************************************//**
*  @fn        void DirectX12::Initialize(void)
*  @brief     Initialize Back Screen
*  @param[in] HWND hwnd
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::Initialize(HWND hwnd)
{
	if (_hasInitialized) { return; }
	SetHWND(hwnd);
	LoadPipeline();
	LoadAssets();
	_hasInitialized = true;
}
void GraphicsDeviceDirectX12::OnResize()
{

}
#pragma endregion Public Function
#pragma region Private Function
/****************************************************************************
*                     LoadPipeLine
*************************************************************************//**
*  @fn        void DirectX12::LoadPipeLine(void)
*  @brief     Load DirectX Basic PipeLine (for initialize)
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LoadPipeline()
{
	/*-------------------------------------------------------------------
	-                   Set Debug Layer
	---------------------------------------------------------------------*/
#if _DEBUG
	EnabledDebugLayer();
#endif

}
void GraphicsDeviceDirectX12::LoadAssets()
{
	
}
#pragma region           Initialize Function
/****************************************************************************
*                     CreateDevice
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CreateDevice(void)
*  @brief     Create DirectX12 Device
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CreateDevice()
{
	/*-------------------------------------------------------------------
	-                   Crate DXGIFactory
	---------------------------------------------------------------------*/
#ifdef _DEBUG
	ThrowIfFailed(CreateDXGIFactory2(DXGI_CREATE_FACTORY_DEBUG, IID_PPV_ARGS(&_dxgiFactory)));
#else
	ThrowIfFailed(CreateDXGIFactory1(IID_PPV_ARGS(&_dxgiFactory)));
#endif

	/*-------------------------------------------------------------------
	-                   Search Hardware Adapter
	---------------------------------------------------------------------*/
	AdapterComPtr adapter;
	AdapterComPtr adapterVender[(int)GPUVender::CountOfGPUVender];
	size_t maxVideoMemory[(int)GPUVender::CountOfGPUVender] = { 0 };
	for (int i = 0; _dxgiFactory->EnumAdapters1(i, (IDXGIAdapter1**)adapter.GetAddressOf()) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		DXGI_ADAPTER_DESC1 adapterDesc;
		adapter->GetDesc1(&adapterDesc);
		if (adapterDesc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE) { continue; }
		if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, __uuidof(ID3D12Device), nullptr))) { continue; }
		if (wcsstr(adapterDesc.Description, L"NVIDIA") != nullptr)
		{
			if (adapterDesc.DedicatedVideoMemory > maxVideoMemory[(int)GPUVender::NVidia])
			{
				if (adapterVender[(int)GPUVender::NVidia]) { adapterVender[(int)GPUVender::NVidia]->Release(); }
				adapterVender[(int)GPUVender::NVidia] = adapter;
				adapterVender[(int)GPUVender::NVidia]->AddRef();
			}
		}
		else if (wcsstr(adapterDesc.Description, L"AMD") != nullptr)
		{
			if (adapterDesc.DedicatedVideoMemory > maxVideoMemory[(int)GPUVender::AMD])
			{
				if (adapterVender[(int)GPUVender::AMD]) { adapterVender[(int)GPUVender::AMD]->Release(); }
				adapterVender[(int)GPUVender::AMD] = adapter;
				adapterVender[(int)GPUVender::AMD]->AddRef();
			}
		}
		else if (wcsstr(adapterDesc.Description, L"Intel") != nullptr)
		{
			if (adapterDesc.DedicatedVideoMemory > maxVideoMemory[(int)GPUVender::Intel])
			{
				if (adapterVender[(int)GPUVender::Intel]) { adapterVender[(int)GPUVender::Intel]->Release(); }
				adapterVender[(int)GPUVender::Intel] = adapter;
				adapterVender[(int)GPUVender::Intel]->AddRef();
			}
		}
		//adapter->Release();
	}
	for (int i = 0; i < (int)GPUVender::CountOfGPUVender; ++i)
	{
		if (adapterVender[i] != nullptr)
		{
			_useAdapter = adapterVender[i].Detach();
			break;
		}
	}

	/*-------------------------------------------------------------------
	-                   Create Hardware Device
	---------------------------------------------------------------------*/
	HRESULT hardwareResult = D3D12CreateDevice(
		_useAdapter.Get(),                // default adapter
		D3D_FEATURE_LEVEL_11_0, // minimum feature level
		IID_PPV_ARGS(&_device));


	/*-------------------------------------------------------------------
	-                  Fallback to WARP Device
	-    (WARP: hight speed fully conformant software rasterizer)
	---------------------------------------------------------------------*/
	if (FAILED(hardwareResult))
	{
		AdapterComPtr warpAdapter;
		ThrowIfFailed(_dxgiFactory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter)));

		ThrowIfFailed(D3D12CreateDevice(
			warpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&_device)));
		_isWarpAdapter = true;
	}
	_device->SetName(L"DirectX12::Device");

	for (int i = 0; i < (int)GPUVender::CountOfGPUVender; ++i)
	{
		if (adapterVender[i] != nullptr)
		{
			adapterVender[i]->Release();
		}
	}
}
/****************************************************************************
*							CreateCommandObject
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CreateCommandObject(void)
*  @brief     Create command object (command queue, command allocator, and command list)@n 
*             for initialize.
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CreateCommandObject()
{
	/*-------------------------------------------------------------------
	-                   Create Command Queue
	---------------------------------------------------------------------*/
	D3D12_COMMAND_QUEUE_DESC cmdQDesc = {};
	cmdQDesc.NodeMask = 0;  // No Adapter Mask
	cmdQDesc.Priority = D3D12_COMMAND_QUEUE_PRIORITY_NORMAL; 
	cmdQDesc.Type     = D3D12_COMMAND_LIST_TYPE_DIRECT;// enable to execute all command 
	cmdQDesc.Flags    = D3D12_COMMAND_QUEUE_FLAG_NONE;

	ThrowIfFailed(_device->CreateCommandQueue(&cmdQDesc, IID_PPV_ARGS(&_commandQueue)));
	_commandQueue->SetName(L"DirectX12::CommandQueue");

	/*-------------------------------------------------------------------
	-                   Create Command Allocator
	---------------------------------------------------------------------*/
	for (int i = 0; i < FRAME_BUFFER_COUNT; ++i)
	{
		ThrowIfFailed(_device->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&_commandAllocator[i])));
		_commandAllocator[i]->SetName(L"DirectX12::CommandAllocator");
	}
	

	/*-------------------------------------------------------------------
	-                   Create Command List
	---------------------------------------------------------------------*/

	ThrowIfFailed(_device->CreateCommandList(
		0,
		D3D12_COMMAND_LIST_TYPE_DIRECT,
		_commandAllocator[_currentFrameIndex].Get(), // Associated command allocator
		nullptr,                 // Initial PipeLine State Object
		IID_PPV_ARGS(_commandList.GetAddressOf())));
	_commandList->SetName(L"DirectX12::CommandList");

	// Start off in a closed state.
	// This is because the first time we refer to the command list when it is reset.
	_commandList->Close();
}
/****************************************************************************
*							CreateDescriptorHeap
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::CreateDescriptorHeap(void)
*  @brief     Create RTV and DSV descriptor heap
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CreateDescriptorHeap()
{
	/*-------------------------------------------------------------------
	-                   Get Descriptor Inclement Size
	---------------------------------------------------------------------*/
	_rtvDescriptorSize       = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	_dsvDescriptorSize       = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	_cbvSrvUavDescriptorSize = _device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	/*-------------------------------------------------------------------
	-			     Set RTV Heap
	---------------------------------------------------------------------*/
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = RTV_DESC_COUNT;
	rtvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvHeapDesc.NodeMask       = 0;
	ThrowIfFailed(_device->CreateDescriptorHeap(
		&rtvHeapDesc, IID_PPV_ARGS(_rtvHeap.GetAddressOf())));
	_rtvHeap->SetName(L"DirectX12::RenderTargetHeap");

	/*-------------------------------------------------------------------
	-			     Set DSV Heap 
	---------------------------------------------------------------------*/
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc={};
	dsvHeapDesc.NumDescriptors = DSV_DESC_COUNT;
	dsvHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvHeapDesc.NodeMask       = 0;
	ThrowIfFailed(_device->CreateDescriptorHeap(
		&dsvHeapDesc, IID_PPV_ARGS(_dsvHeap.GetAddressOf())));
	_dsvHeap->SetName(L"DirectX12::DepthStencilViewHeap");

	/*-------------------------------------------------------------------
	-			     Set CBV, SRV, UAV Heap 
	---------------------------------------------------------------------*/
	D3D12_DESCRIPTOR_HEAP_DESC csuHeapDesc={};
	csuHeapDesc.NumDescriptors = CBV_DESC_COUNT + SRV_DESC_COUNT + UAV_DESC_COUNT;
	csuHeapDesc.Type           = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	csuHeapDesc.Flags          = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	csuHeapDesc.NodeMask       = 0;
	ThrowIfFailed(_device->CreateDescriptorHeap(&csuHeapDesc, IID_PPV_ARGS(&_cbvSrvUavHeap)));
	_cbvSrvUavHeap->SetName(L"DirectX12::CBV,SRV,UAVHeap");
}
/****************************************************************************
*							BuildResourceAllocator
*************************************************************************//**
*  @fn        void DirectX12::BuildResourceAllocator()
*  @brief     Build Resource Allocator
*  @param[in] void
*  @return 　　void
*****************************************************************************/

#pragma endregion Initialize Function
#pragma region           Debug      Function
/****************************************************************************
*                     EnabledDebugLayer
*************************************************************************//**
*  @fn        void DirectX12::EnabledDebugLayer(void)
*  @brief     Enabled CPU debug layer
*  @param[in] void
*  @return 　　void @n
*  @details   it must be called before the D3D12 device is created.
*****************************************************************************/
void GraphicsDeviceDirectX12::EnabledDebugLayer()
{
	DebugComPtr debugController;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	debugController->EnableDebugLayer();
	debugController.Reset();
}

/****************************************************************************
*                   EnabledGPUBasedValiation
*************************************************************************//**
*  @fn        void DirectX12::EnabledGPUBasedValidation(void)
*  @brief     Enabled GPU debugger
*  @param[in] void
*  @return 　　void
*  @details   GPU-based valiation helps to identify the following errors.@n
*             1. Use of uninitialized or incompatible descriptors in a shader.@n
*             2. Use of descriptors referencing deleted Resources in a shader.@n
*             3. Validation of promoted resource states and resource state decay.@n
*             4. Indexing beyond the end of the descriptor heap in a shader.@n
*             5. Shader accesses of resources in incompatible state.@n
*             6. Use of uninitialized or incompatible Samplers in a shader.@n
*****************************************************************************/
void GraphicsDeviceDirectX12::EnabledGPUBasedValidation()
{
	DebugComPtr debugController;
	DebugComPtr debug;
	ThrowIfFailed(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)));
	ThrowIfFailed(debugController->QueryInterface(IID_PPV_ARGS(&debug)));
	debug->SetEnableGPUBasedValidation(true);

}

/****************************************************************************
*                     LogAdapters
*************************************************************************//**
*  @fn        void DirectX12::LogAdapters(void)
*  @brief     Show all adapter name (GeForce... )
*  @param[in] Adapter* adapter
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LogAdapters()
{
	UINT i = 0;
	Adapter* adapter = nullptr;
	std::vector<Adapter*> adapterList;

	/*-------------------------------------------------------------------
	-                  Enum Adapter List
	---------------------------------------------------------------------*/
	while (_dxgiFactory->EnumAdapters(i, (IDXGIAdapter**)&adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adapter->GetDesc(&desc);

		std::wstring text = L"***Adapter: ";
		text += desc.Description;
		text += L"\n";

		OutputDebugString(text.c_str());

		adapterList.push_back(adapter);

		++i;
	}

	/*-------------------------------------------------------------------
	-                  Show Adapter List
	---------------------------------------------------------------------*/
	for (size_t i = 0; i < adapterList.size(); ++i)
	{
		LogAdapterOutputs(adapterList[i]);
		SAFE_RELEASE(adapterList[i]);
	}
}

/****************************************************************************
*                     LogAdapterOutputs
*************************************************************************//**
*  @fn        void DirectX12::LogAdapterOutputs(Adapter* adapter)
*  @brief     Show all display output name
*  @param[in] Adapter* adapter
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LogAdapterOutputs(Adapter* adapter)
{
	/*-------------------------------------------------------------------
	-                     Initialize
	---------------------------------------------------------------------*/
	UINT i = 0;
	Output* output = nullptr;

	/*-------------------------------------------------------------------
	-                Show all adapter name
	---------------------------------------------------------------------*/
	while (adapter->EnumOutputs(i, (IDXGIOutput**)&output) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_OUTPUT_DESC desc;
		output->GetDesc(&desc);

		std::wstring text = L"***Output: ";
		text += desc.DeviceName;
		text += L"n";
		OutputDebugString(text.c_str());

		LogOutputDisplayModes(output, _backBufferFormat);

		SAFE_RELEASE(output);

		++i;
	}
}

/****************************************************************************
*                     LogOutputDisplayModes
*************************************************************************//**
*  @fn        void DirectX12::LogOutputDisplayModes(Output* output, DXGI_FORMAT format)
*  @brief     Show display modes (output screen width, height and refresh rates)
*  @param[in] Output* output: for display modelist
*  @param[in] DXGI_FORMAT   : format
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::LogOutputDisplayModes(Output* output, DXGI_FORMAT format)
{
	UINT count = 0;
	UINT flags = 0;

	/*-------------------------------------------------------------------
	-             Call with nullptr to get list count
	---------------------------------------------------------------------*/
	output->GetDisplayModeList(format, flags, &count, nullptr);

	std::vector<DXGI_MODE_DESC> modeList(count);
	output->GetDisplayModeList(format, flags, &count, &modeList[0]);

	/*-------------------------------------------------------------------
	-                 Show display modes
	---------------------------------------------------------------------*/
	for (auto& x : modeList)
	{
		UINT n = x.RefreshRate.Numerator;
		UINT d = x.RefreshRate.Denominator;
		std::wstring text =
			L"Width = " + std::to_wstring(x.Width) + L" " +
			L"Height = " + std::to_wstring(x.Height) + L" " +
			L"Refresh = " + std::to_wstring(n) + L"/" + std::to_wstring(d) +
			L"\n";

		::OutputDebugString(text.c_str());
	}
}
/****************************************************************************
*                     ReportLiveObjects
*************************************************************************//**
*  @fn        void GraphicsDeviceDirectX12::ReportLiveObjects()
*  @brief     ReportLiveObjects
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::ReportLiveObjects()
{
	ID3D12DebugDevice2* debugInterface = nullptr;
	if (SUCCEEDED(_device.Get()->QueryInterface(&debugInterface)))
	{
		debugInterface->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL | D3D12_RLDO_IGNORE_INTERNAL);
		debugInterface->Release();
	}
}
#pragma endregion  Debug Function
#pragma region HDR Function
/****************************************************************************
*                     EnsureSwapChainColorSpace
*************************************************************************//**
*  @fn        void DirectX12::EnsureSwapChainColorSpace()
*  @brief     Check SwapChain Color Space
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::EnsureSwapChainColorSpace()
{

	DXGI_SWAP_CHAIN_DESC1 desc;
	ThrowIfFailed(_swapchain->GetDesc1(&desc));

	DXGI_COLOR_SPACE_TYPE colorSpace;
	switch (desc.Format)
	{
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G10_NONE_P709;
			break;
		case DXGI_FORMAT_R10G10B10A2_UNORM:
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
			break;
		default:
			colorSpace = DXGI_COLOR_SPACE_RGB_FULL_G22_NONE_P709;
			break;
	}

	_swapchain->SetColorSpace1(colorSpace);
}

/****************************************************************************
*                     SetHDRMetaData
*************************************************************************//**
*  @fn        void DirectX12::SetHDRMetaData()
*  @brief     Set HDR Meta Data
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::SetHDRMetaData()
{

	DXGI_SWAP_CHAIN_DESC1 desc;
	ThrowIfFailed(_swapchain->GetDesc1(&desc));

	/*-------------------------------------------------------------------
	-          In case False (isHDRSupport)
	---------------------------------------------------------------------*/
	if (!_isHDRSupport)
	{
		ThrowIfFailed(_swapchain->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_NONE, 0, nullptr));
		return;
	}

	/*-------------------------------------------------------------------
	-          Define Display Chromacity
	---------------------------------------------------------------------*/
	struct DisplayChromacities
	{
		float RedX; float RedY;
		float GreenX; float GreenY;
		float BlueX; float BlueY;
		float WhiteX; float WhiteY;
	};

	static const DisplayChromacities DisplayChromacityList[] =
	{
		 { 0.64000f, 0.33000f, 0.30000f, 0.60000f, 0.15000f, 0.06000f, 0.31270f, 0.32900f }, // Display Gamut Rec709 
		 { 0.70800f, 0.29200f, 0.17000f, 0.79700f, 0.13100f, 0.04600f, 0.31270f, 0.32900f }  // Display Gamut Rec2020
	};

	/*-------------------------------------------------------------------
	-          Select Chroma Format
	---------------------------------------------------------------------*/
	int selectedChroma = 0;
	if (desc.Format == DXGI_FORMAT_R16G16B16A16_FLOAT)
	{
		selectedChroma = 1;
	}

	/*-------------------------------------------------------------------
	-          Set HDR10Meta Data
	---------------------------------------------------------------------*/
	const auto& chroma = DisplayChromacityList[selectedChroma];
	DXGI_HDR_METADATA_HDR10 HDR10MetaData = {};
	HDR10MetaData.RedPrimary[0] = UINT16(chroma.RedX * 50000.0f);
	HDR10MetaData.RedPrimary[1] = UINT16(chroma.RedY * 50000.0f);
	HDR10MetaData.GreenPrimary[0] = UINT16(chroma.GreenX * 50000.0f);
	HDR10MetaData.GreenPrimary[1] = UINT16(chroma.GreenY * 50000.0f);
	HDR10MetaData.BluePrimary[0] = UINT16(chroma.BlueX * 50000.0f);
	HDR10MetaData.BluePrimary[1] = UINT16(chroma.BlueY * 50000.0f);
	HDR10MetaData.WhitePoint[0] = UINT16(chroma.WhiteX * 50000.0f);
	HDR10MetaData.WhitePoint[1] = UINT16(chroma.WhiteY * 50000.0f);
	HDR10MetaData.MaxMasteringLuminance = UINT(1000.0f * 10000.0f);
	HDR10MetaData.MinMasteringLuminance = UINT(0.001f * 10000.0f);
	HDR10MetaData.MaxContentLightLevel = UINT16(2000.0f);
	HDR10MetaData.MaxFrameAverageLightLevel = UINT16(500.0f);

	/*-------------------------------------------------------------------
	-          Set HDRMetaData
	---------------------------------------------------------------------*/
	_swapchain->SetHDRMetaData(DXGI_HDR_METADATA_TYPE_HDR10, sizeof(HDR10MetaData), &HDR10MetaData);
}

/****************************************************************************
*                     CheckHDRDisplaySupport
*************************************************************************//**
*  @fn        bool DirectX12::CheckHDRDisplaySupport()
*  @brief     CheckHDRDisplaySupport()
*  @param[in] void
*  @return 　　void
*****************************************************************************/
bool GraphicsDeviceDirectX12::CheckHDRDisplaySupport()
{
	_backBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
	bool isEnabledHDR =
		_backBufferFormat == DXGI_FORMAT_R16G16B16A16_FLOAT ||
		_backBufferFormat == DXGI_FORMAT_R10G10B10A2_UNORM;

	if (isEnabledHDR)
	{
		bool isDisplayHDR10 = false;
		UINT index = 0;

		ComPtr<IDXGIOutput> currentOutput;
		while (_useAdapter->EnumOutputs(index, &currentOutput) != DXGI_ERROR_NOT_FOUND)
		{
			OutputComPtr output;
			currentOutput.As(&output);

			DXGI_OUTPUT_DESC1 desc;
			output->GetDesc1(&desc);

			isDisplayHDR10 |= desc.ColorSpace == DXGI_COLOR_SPACE_RGB_FULL_G2084_NONE_P2020;
			++index;
		}

		if (!isDisplayHDR10)
		{
			//_backBufferFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			//isEnabledHDR = false;
		}
	}

	return isEnabledHDR;

}
#pragma endregion    HDR Function
#pragma region DXR Function
/****************************************************************************
*						CheckDXRSupport
*************************************************************************//**
*  @fn        void DirectX12::CheckDXRSupport
*  @brief     Check DXRSupport
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void GraphicsDeviceDirectX12::CheckDXRSupport()
{
	bool enableRayTracing = true;
	D3D12_FEATURE_DATA_D3D12_OPTIONS5 options{};
	HRESULT result = _device->CheckFeatureSupport(D3D12_FEATURE_D3D12_OPTIONS5, &options, UINT(sizeof(options)));
	if (FAILED(result) || options.RaytracingTier == D3D12_RAYTRACING_TIER_NOT_SUPPORTED)
	{
		std::wstring errorMessage = L"DirectX Raytracing not supported.";
		OutputDebugString(errorMessage.c_str());
		enableRayTracing = false;
	}

	_enableRayTracing = enableRayTracing;
}
#pragma endregion    DXR Function
#pragma endregion Private Function