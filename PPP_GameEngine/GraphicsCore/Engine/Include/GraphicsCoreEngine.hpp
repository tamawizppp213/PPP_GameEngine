//////////////////////////////////////////////////////////////////////////////////
///             @file   TemplateText.hpp
///             @brief  TemplateText
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GRAPHICS_CORE_ENGINE_HPP
#define GRAPHICS_CORE_ENGINE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/DirectX12/Core/Include/DirectX12GraphicsDevice.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12CommandContext.hpp"
#include "GraphicsCore/DirectX12/Core/Include/DirectX12GPUResource.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
struct HWND__;
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
/****************************************************************************
*				  			GraphicsCoreEngine
*************************************************************************//**
*  @class     GraphicsCoreEngine
*  @brief     Low level Graphics interface
*****************************************************************************/
class GraphicsCoreEngine
{

public:
	using HWND = HWND__*;
	using GraphicsDevicePtr = std::unique_ptr<GraphicsDeviceDirectX12>;
	using CommandContextPtr = std::unique_ptr<CommandContext>;

	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	static GraphicsCoreEngine& Instance() { static GraphicsCoreEngine engine; return engine; }
	/*-------------------------------------------------------------------
	-          Start up application and ShutDown application
	---------------------------------------------------------------------*/
	void StartUp(HWND hwnd);
	void OnResize();
	void ShutDown();
	/*-------------------------------------------------------------------
	-           Each render scene command interface
	---------------------------------------------------------------------*/
	void OnInitializeRenderScene();
	void BeginDrawFrame();
	void EndDrawFrame();
	void OnTerminateRenderScene();
	/*--------0-----------------------------------------------------------
	-                Command Queue
	---------------------------------------------------------------------*/
	void ExecuteCommandContext();
	void WaitNextFrame();
	void ResetCommandList();
	/*-------------------------------------------------------------------
	-                Create Resource
	---------------------------------------------------------------------*/
	UINT CreateShaderResourceView(GPUResource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC* desc);
	void CreateShaderResourceView(GPUResource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC* desc, D3D12_CPU_DESCRIPTOR_HANDLE destDescriptor);
	UINT CreateUnorderedAccessView(GPUResource* resource, const D3D12_UNORDERED_ACCESS_VIEW_DESC* desc);

	void CreateCommittedResource(const D3D12_HEAP_PROPERTIES* heapProperties, D3D12_HEAP_FLAGS HeapFlags, const D3D12_RESOURCE_DESC* descriptor, D3D12_RESOURCE_STATES initialResourceState, const D3D12_CLEAR_VALUE* optimizedClearValue, const IID& riidResource, void** resource);
    
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	      GraphicsDeviceDirectX12* GetGraphicsDevice()       { return _device.get(); }
	const GraphicsDeviceDirectX12* GetGraphicsDevice() const { return _device.get(); }
	      CommandContext* GetCommandContext()       { return _commandContext.get(); }
	const CommandContext* GetCommandContext() const { return _commandContext.get(); }
	
	int GetCurrentFrameIndex();
	int GetNextFrameBufferIndex();

	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
	
	GraphicsCoreEngine(const GraphicsCoreEngine&)            = delete;
	GraphicsCoreEngine& operator=(const GraphicsCoreEngine&) = delete;
	GraphicsCoreEngine(GraphicsCoreEngine&&)                 = delete;
	GraphicsCoreEngine& operator=(GraphicsCoreEngine&&)      = delete;
private:
	/****************************************************************************
	**                Private Function
	*****************************************************************************/
	GraphicsCoreEngine() = default;
	~GraphicsCoreEngine() = default;
	/****************************************************************************
	**                Private Member Variables
	*****************************************************************************/
	GraphicsDevicePtr _device         = nullptr;
	CommandContextPtr _commandContext = nullptr;

	bool _hasInitialized = false;
};



#endif