//////////////////////////////////////////////////////////////////////////////////
///             @file   DirectX12GrahicsDevice.hpp
///             @brief  Grahics Device for DirectX12
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_GRAPHICS_DEVICE_HPP
#define DIRECTX12_GRAPHICS_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/Interface/Include/IGraphicsDevice.hpp"
#include "DirectX12Core.hpp"
#include <dxgiformat.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

/****************************************************************************
*				  			TemplateClass
*************************************************************************//**
*  @class     TemplateClass
*  @brief     temp
*****************************************************************************/
class GraphicsDeviceDirectX12 : public IGraphicsDevice
{
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	void Initialize        (HWND hwnd) override;
	void OnResize          () override;
	void Finalize          () override;
	void ClearScreen       () override;
	void CompleteInitialize() override;
	void CompleteRendering () override;
	void FlushCommandQueue () override;

	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	void LoadPipeline();
	void LoadAssets();
	/*-------------------------------------------------------------------
	-                        Initialize
	---------------------------------------------------------------------*/
	void CreateDevice();
	void CreateCommandObject();
	void CreateSwapChain();
	void CreateDescriptorHeap();
	void BuildResourceAllocator();
	void CreatePSOs();

	/*-------------------------------------------------------------------
	-                        Debug
	---------------------------------------------------------------------*/
	void EnabledDebugLayer();
	void EnabledGPUBasedValidation();
	void LogAdapters     ();
	void LogAdapterOutputs(Adapter* adapter);
	void LogOutputDisplayModes(Output* output, DXGI_FORMAT format);
	void ReportLiveObjects();
	/*-------------------------------------------------------------------
	-                        HDR
	---------------------------------------------------------------------*/
	void EnsureSwapChainColorSpace();
	bool CheckHDRDisplaySupport();
	void SetHDRMetaData();
	/*-------------------------------------------------------------------
	-                        DXR
	---------------------------------------------------------------------*/
	void CheckDXRSupport();
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	/*-------------------------------------------------------------------
	-                        Debug
	---------------------------------------------------------------------*/
	DeviceComPtr                 _device;             /// Device
	FactoryComPtr                _dxgiFactory;        /// DXGI
	AdapterComPtr                _useAdapter;
	SwapchainComPtr              _swapchain;          /// SwapChain
	CommandQueueComPtr           _commandQueue;       /// Command Queue (Command Execution Unit)
	CommandListComPtr            _commandList;        /// Graphics Command List
	CommandAllocatorComPtr       _commandAllocator[FRAME_BUFFER_COUNT];
	DescriptorHeapComPtr         _rtvHeap;            /// Heap For Render Target View 
	DescriptorHeapComPtr         _dsvHeap;            /// Heap For Depth Stencil View
	DescriptorHeapComPtr         _cbvSrvUavHeap;      /// Heap For Constant Buffer View
	UINT _rtvDescriptorSize       = 0;
	UINT _dsvDescriptorSize       = 0;
	UINT _cbvSrvUavDescriptorSize = 0;
	INT  _currentFrameIndex       = 0;
	
	DXGI_FORMAT _backBufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;

	bool _isWarpAdapter    = false;
	bool _isHDRSupport     = true;
	bool _enableRayTracing = true;

	static constexpr int RTV_DESC_COUNT    = 1000;
	static constexpr int DSV_DESC_COUNT    = 100;
	static constexpr int CBV_DESC_COUNT    = 1024 * 10;
	static constexpr int UAV_DESC_COUNT    = 1024 * 10;
	static constexpr int SRV_DESC_COUNT    = 1024 * 10;
	static constexpr int MAX_SAMPLER_STATE = 16;
};

#endif