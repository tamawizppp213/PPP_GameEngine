//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12CommandList.cpp
///             @brief  CommandList 
///             @author Toide Yutaro
///             @date   2022_06_24
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandList.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12CommandAllocator.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12FrameBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12RenderPass.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12DescriptorHeap.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12ResourceLayout.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Query.hpp"
#include "GraphicsCore/RHI/DirectX12/PipelineState/Include/DirectX12GPUPipelineState.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUBuffer.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUTexture.hpp"
#include "GraphicsCore/RHI/DirectX12/Resource/Include/DirectX12GPUResourceView.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12BaseStruct.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include <stdexcept>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "GameUtility/Container/Include/GUDynamicArray.hpp"
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
RHICommandList::~RHICommandList()
{
	if (_commandList) { _commandList.Reset(); _commandList = nullptr; }
}

RHICommandList::RHICommandList(const gu::SharedPointer<rhi::core::RHIDevice>& device, const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator, const gu::tstring& name) : 
	rhi::core::RHICommandList(device, commandAllocator)
{
	/*-------------------------------------------------------------------
	-               Get device and command allocator
	---------------------------------------------------------------------*/
	const auto dxDevice     = static_cast<RHIDevice*>(_device.Get())->GetDevice();
	const auto rhiAllocator = gu::StaticPointerCast<directX12::RHICommandAllocator>(_commandAllocator);
	const auto dxAllocator  = rhiAllocator->GetAllocator();

	/*-------------------------------------------------------------------
	-               Get command list type
	---------------------------------------------------------------------*/
	_commandListType = rhiAllocator->GetCommandListType();

	/*-------------------------------------------------------------------
	-               Prepare Closed Command List
	---------------------------------------------------------------------*/
	ThrowIfFailed(dxDevice->CreateCommandList(
		_device->GetGPUMask().Value(),            // GPU index
		EnumConverter::Convert(_commandListType),
		dxAllocator.Get(),                        // Associated command allocator
		nullptr,                                  // Initial PipeLine State Object
		IID_PPV_ARGS(_commandList.GetAddressOf())));

	ThrowIfFailed(_commandList->SetName(name.CString()));
	ThrowIfFailed(_commandList->Close());
	_isOpen = false;
}

#pragma endregion Constructor and Destructor

#pragma region Call Draw Frame
/****************************************************************************
*                     BeginRecording
*************************************************************************//**
*  @fn        void RHICommandList::BeginRecording()
*
*  @brief     This function must be called at draw function initially (stillMidFrame = false
*             If still mid frame is set false, this function clears the command allocator
*
*  @param[in] const bool stillMidFrame (default: false)
*
*  @return    void
*****************************************************************************/
void RHICommandList::BeginRecording(const bool stillMidFrame)
{
	/*-------------------------------------------------------------------
	-        コマンドリストが記録可能状態だった場合, Resetすることが出来ない.
	---------------------------------------------------------------------*/
	if (IsOpen()) { return; }

	/*-------------------------------------------------------------------
	-        描画フレームの途中でなければCommandAllocatorのバッファを先頭に戻す.
	---------------------------------------------------------------------*/
	if (_commandAllocator)
	{
		if (!stillMidFrame)
		{
			_commandAllocator->CleanUp();
		} // command buffer clear
		ThrowIfFailed(_commandList->Reset(static_cast<RHICommandAllocator*>(_commandAllocator.Get())->GetAllocator().Get(), nullptr));
	}
	
	/*-------------------------------------------------------------------
	-        コマンドリストを記録可能状態に変更します
	---------------------------------------------------------------------*/
	_isOpen          = true;
	_beginRenderPass = false;
}

/****************************************************************************
*                     EndRecording
*************************************************************************//**
*  @fn        void RHICommandList::EndRecording()
*
*  @brief     コマンドリストを記録状態から実行可能状態に変更します. これはDraw関数の最後に使用します
* 
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHICommandList::EndRecording()
{
	/*-------------------------------------------------------------------
	-      　既に閉じられているならこの後の処理は行わない
	---------------------------------------------------------------------*/
	if (IsClosed()) { OutputDebugStringA("Already closed. \n");  return; }
	
	/*-------------------------------------------------------------------
	-      　記録状態からクローズ状態に以降する
	---------------------------------------------------------------------*/
	ThrowIfFailed(_commandList->Close());

	/*-------------------------------------------------------------------
	-      　終了処理
	---------------------------------------------------------------------*/
	_isOpen = false;
	_beginRenderPass = false;
}

/****************************************************************************
*                     Reset
*************************************************************************//**
*  @fn        void RHICommandList::Reset(const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator)
*
*  @brief     Proceed to the record state. コマンドリストを記録状態に変更します.
		      基本的には, ResetではなくBeginRecordingを使用してください.
*
*  @param[in] const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator
*             コマンドをためておくバッファです
*
*  @return    void
*****************************************************************************/
void RHICommandList::Reset(const gu::SharedPointer<rhi::core::RHICommandAllocator>& commandAllocator)
{
	/*-------------------------------------------------------------------
	-        コマンドリストが記録可能状態だった場合, Resetすることが出来ない.
	---------------------------------------------------------------------*/
	if (IsOpen()) { return; }

	/*-------------------------------------------------------------------
	-         新しいコマンドバッファが同じキューの種類であるかを確認する
	---------------------------------------------------------------------*/
	if (commandAllocator) 
	{
		// コマンドリストの種類チェック
		if (_commandAllocator->GetCommandListType() != GetType()) 
		{
			_RPTW0(_CRT_WARN, L"Different commandList type");
			return;
		}

		// コマンドリストを登録
		_commandAllocator = commandAllocator; 

		// Reset
		ThrowIfFailed(_commandList->Reset(static_cast<RHICommandAllocator*>(_commandAllocator.Get())->GetAllocator().Get(), nullptr));
	}
	else
	{
		return;
	}

	/*-------------------------------------------------------------------
	-        コマンドリストを開いている状態にする
	---------------------------------------------------------------------*/
	_isOpen = true;
	_beginRenderPass = false;
}

/****************************************************************************
*                     BeginRenderPass
*************************************************************************//**
*  @fn        void RHICommandList::BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer)
*
*  @brief     Begin render pass and frame buffer.
*
*  @param[in] const gu::SharedPointer<core::RHIRenderPass>& renderPass
*  @param[in] const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer
*
*  @return    void
*****************************************************************************/
void RHICommandList::BeginRenderPass(const gu::SharedPointer<core::RHIRenderPass>& renderPass, const gu::SharedPointer<core::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Layout Transition (Present -> RenderTarget)
	---------------------------------------------------------------------*/
	gu::DynamicArray<core::ResourceState> states(frameBuffer->GetRenderTargetSize(), core::ResourceState::RenderTarget);
	TransitionResourceStates(static_cast<std::uint32_t>(frameBuffer->GetRenderTargetSize()), frameBuffer->GetRenderTargets().Data(), states.Data());

	/*-------------------------------------------------------------------
	-          Select renderpass and frame buffer action
	---------------------------------------------------------------------*/
	if (_device->IsSupportedRenderPass()) { BeginRenderPassImpl(gu::StaticPointerCast<directX12::RHIRenderPass>(renderPass), gu::StaticPointerCast<directX12::RHIFrameBuffer>(frameBuffer)); }
	else                                  { OMSetFrameBuffer   (gu::StaticPointerCast<directX12::RHIRenderPass>(renderPass), gu::StaticPointerCast<directX12::RHIFrameBuffer>(frameBuffer)); }

	// 参照カウントが無限に増えつつけるバグ対応に使用しました.
	if (_renderPass ) { _renderPass.Reset(); }
	if (_frameBuffer) { _frameBuffer.Reset(); }
	_renderPass  = renderPass;
	_frameBuffer = frameBuffer;
	_beginRenderPass = true;
}

/****************************************************************************
*                     EndRenderPass
*************************************************************************//**
*  @fn        void RHICommandList::EndRenderPass()
*
*  @brief     End render pass
*
*  @param[in] void
*
*  @return    void
*****************************************************************************/
void RHICommandList::EndRenderPass()
{
	if (!_beginRenderPass) { return; }
	if (_device->IsSupportedRenderPass()) { _commandList->EndRenderPass(); }

	/*-------------------------------------------------------------------
	-          Layout Transition (RenderTarget -> Present)
	---------------------------------------------------------------------*/
	gu::DynamicArray<core::ResourceState> states(_frameBuffer->GetRenderTargetSize(), core::ResourceState::Present);
	TransitionResourceStates(static_cast<std::uint32_t>(_frameBuffer->GetRenderTargetSize()), _frameBuffer->GetRenderTargets().Data(), states.Data());
	_beginRenderPass = false;
}

#pragma endregion Call Draw Frame

#pragma region GPU Command
/****************************************************************************
*                       SetDepthBounds
*************************************************************************//**
*  @fn        void RHICommandList::SetDepthBounds(const float minDepth, const float maxDepth)
*
*  @brief     深度が指定の範囲に入っているかをテストし, 範囲内ならばピクセルシェーダーを動作させます.

*  @return 　　void
*****************************************************************************/
void RHICommandList::SetDepthBounds(const float minDepth, const float maxDepth)
{
#if PLATFORM_OS_WINDOWS
	if (_device->IsSupportedDepthBoundsTest())
	{
		_commandList->OMSetDepthBounds(minDepth, maxDepth);
	}
#endif
}

void RHICommandList::SetDescriptorHeap(const gu::SharedPointer<core::RHIDescriptorHeap>& heap)
{
	const auto dxHeap = gu::StaticPointerCast<directX12::RHIDescriptorHeap>(heap);
	_commandList->SetDescriptorHeaps(1, dxHeap->GetHeap().GetAddressOf());
}

#pragma region Query
/****************************************************************************
*                       BeginQuery
*************************************************************************//**
*  @fn        void RHICommandList::BeginQuery(const core::QueryResultLocation& location)
*
*  @brief     GPU情報を取得するためのクエリを開始します
*
*  @param[in] const core::QueryResultLocation& location
*
*  @return 　　void
*****************************************************************************/
void RHICommandList::BeginQuery(const core::QueryResultLocation& location)
{
	/*-------------------------------------------------------------------
	-              Queryの取得
	---------------------------------------------------------------------*/
	const auto query = static_cast<directX12::RHIQuery*>(location.Heap.Get());
	
	Checkf(query, "query is nullptr");
	Check(query->GetDxQueryType() == D3D12_QUERY_TYPE_OCCLUSION ||
		  query->GetDxQueryType() == D3D12_QUERY_TYPE_PIPELINE_STATISTICS);
	
	/*-------------------------------------------------------------------
	-              QueryHeapの取得
	---------------------------------------------------------------------*/
	const auto heap = query->GetHeap();

	Checkf(heap, "heap is nullptr");

	/*-------------------------------------------------------------------
	-               Common command
	---------------------------------------------------------------------*/
	_commandList->BeginQuery(heap.Get(), query->GetDxQueryType(), location.QueryID);
}

/****************************************************************************
*                       EndQuery
*************************************************************************//**
*  @fn        void RHICommandList::EndQuery(const core::QueryResultLocation& location)
*
*  @brief     GPU情報を取得するためのクエリを終了します
*
*  @param[in] const core::QueryResultLocation& location
*
*  @return 　　void
*****************************************************************************/
void RHICommandList::EndQuery(const core::QueryResultLocation& location)
{
	const auto query = static_cast<directX12::RHIQuery*>(location.Heap.Get());
	Checkf(query, "query is nullptr");
	
	// クエリの終了
	_commandList->EndQuery(query->GetHeap().Get(), query->GetDxQueryType(), location.QueryID);
}
#pragma endregion Query
/****************************************************************************
*                       SetPrimitiveTopology
*************************************************************************//**
*  @fn        void RHICommandList::SetPrimitiveTopology(core::PrimitiveTopology topology)
* 
*  @brief     Regist Primitive topology to command list
* 
*  @param[in] const core::PrimitiveTopology
* 
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetPrimitiveTopology(const core::PrimitiveTopology topology)
{
	_commandList->IASetPrimitiveTopology(EnumConverter::Convert(topology));
}

/****************************************************************************
*                       SetViewport
*************************************************************************//**
*  @fn        void rhi::directX12::RHICommandList::SetViewport(const core::Viewport* viewport, std::uint32_t numViewport)
*  @brief     Regist multi viewport to command list
*  @param[in] const core::Viewport view port
*  @param[in] std::uint32_t viewport count
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetViewport(const core::Viewport* viewport, const std::uint32_t numViewport)
{
	gu::DynamicArray<D3D12_VIEWPORT> v(numViewport);
	for (UINT i = 0; i < numViewport; ++i)
	{
		v[i].TopLeftX = viewport->TopLeftX;
		v[i].TopLeftY = viewport->TopLeftY;
		v[i].Width    = viewport->Width;
		v[i].Height   = viewport->Height;
		v[i].MaxDepth = viewport->MaxDepth;
		v[i].MinDepth = viewport->MinDepth;
	}
	_commandList->RSSetViewports(numViewport, v.Data());
}
/****************************************************************************
*                       SetScissorRect
*************************************************************************//**
*  @fn        void rhi::directX12::RHICommandList::SetScissor(const core::ScissorRect* rect, std::uint32_t numRect)
*  @brief     Regist multi scissorRect to command list
*  @param[in] const core::ScissorRect*
*  @param[in] std::uint32_t numRect
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetScissor(const core::ScissorRect* rect, const std::uint32_t numRect)
{
	gu::DynamicArray<D3D12_RECT> r(numRect);
	for (UINT i = 0; i < numRect; ++i)
	{
		r[i].left = rect->Left;
		r[i].right = rect->Right;
		r[i].bottom = rect->Bottom;
		r[i].top = rect->Top;
	}
	_commandList->RSSetScissorRects(numRect, r.Data());
}
/****************************************************************************
*                       SetViewportAndScissor
*************************************************************************//**
*  @fn        void RHICommandList::SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect)
*  @brief     Regist viewport and scissorRect to command list
*  @param[in] const core::Viewport& viewport
*  @param[in] const core::ScissorRect& rect
*  @return 　　void
*****************************************************************************/
void RHICommandList::SetViewportAndScissor(const core::Viewport& viewport, const core::ScissorRect& rect)
{
	D3D12_VIEWPORT v = {};
	v.TopLeftX = viewport.TopLeftX;
	v.TopLeftY = viewport.TopLeftY;
	v.Width    = viewport.Width;
	v.Height   = viewport.Height;
	v.MaxDepth = viewport.MaxDepth;
	v.MinDepth = viewport.MinDepth;

	D3D12_RECT r = {};
	r.left   = rect.Left;
	r.right  = rect.Right;
	r.bottom = rect.Bottom;
	r.top    = rect.Top;

	_commandList->RSSetViewports(1, &v);
	_commandList->RSSetScissorRects(1, &r);
}

void RHICommandList::SetVertexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer)
{
#if __DEBUG
	// Is vertex buffer
	assert(buffer->GetUsage() == core::ResourceUsage::VertexBuffer);
#endif

	// Set up vertex buffer view
	D3D12_VERTEX_BUFFER_VIEW view = {
		gu::StaticPointerCast<directX12::GPUBuffer>(buffer)->GetResourcePtr()->GetGPUVirtualAddress(),
		static_cast<UINT>(buffer->GetTotalByteSize()),
		static_cast<UINT>(buffer->GetElementByteSize())
	};
	// Regist command list
	_commandList->IASetVertexBuffers(0, 1, &view);
}

void RHICommandList::SetResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	_commandList->SetGraphicsRootSignature(gu::StaticPointerCast<directX12::RHIResourceLayout>(resourceLayout)->GetRootSignature().Get());
}

void RHICommandList::SetComputeResourceLayout(const gu::SharedPointer<core::RHIResourceLayout>& resourceLayout)
{
	_commandList->SetComputeRootSignature(gu::StaticPointerCast<directX12::RHIResourceLayout>(resourceLayout)->GetRootSignature().Get());
}

void RHICommandList::SetGraphicsPipeline(const gu::SharedPointer<core::GPUGraphicsPipelineState>& pipelineState)
{
	SetPrimitiveTopology(pipelineState->GetInputAssemblyState()->GetPrimitiveTopology());
	_commandList->SetPipelineState(gu::StaticPointerCast<directX12::GPUGraphicsPipelineState>(pipelineState)->GetPipeline().Get());
}

void RHICommandList::SetComputePipeline(const gu::SharedPointer<core::GPUComputePipelineState>& pipelineState)
{
	_commandList->SetPipelineState(gu::StaticPointerCast<directX12::GPUComputePipelineState>(pipelineState)->GetPipeline().Get());
}
void RHICommandList::SetVertexBuffers(const gu::DynamicArray<gu::SharedPointer<core::GPUBuffer>>& buffers, const size_t startSlot)
{
	auto views = gu::DynamicArray<D3D12_VERTEX_BUFFER_VIEW>(buffers.Size());
	for (size_t i = 0; i < views.Size(); ++i)
	{
#if __DEBUG
		assert(buffers[i]->GetUsage() == core::ResourceUsage::VertexBuffer);
#endif
		views[i].BufferLocation = gu::StaticPointerCast<directX12::GPUBuffer>(buffers[i])->GetResourcePtr()->GetGPUVirtualAddress();
		views[i].SizeInBytes    = static_cast<UINT>(buffers[i]->GetTotalByteSize());
		views[i].StrideInBytes  = static_cast<UINT>(buffers[i]->GetElementByteSize());
	}

	_commandList->IASetVertexBuffers(static_cast<UINT>(startSlot), static_cast<UINT>(views.Size()), views.Data());
}

void RHICommandList::SetIndexBuffer(const gu::SharedPointer<core::GPUBuffer>& buffer, const core::IndexType indexType)
{
#if __DEBUG
	assert(buffer->GetUsage() == core::ResourceUsage::IndexBuffer);
#endif

	D3D12_INDEX_BUFFER_VIEW view = {};
	view.BufferLocation = gu::StaticPointerCast<directX12::GPUBuffer>(buffer)->GetResourcePtr()->GetGPUVirtualAddress();
	view.SizeInBytes    = static_cast<UINT>(buffer->GetTotalByteSize());
	view.Format         = EnumConverter::Convert(indexType);

	_commandList->IASetIndexBuffer(&view);
}

void RHICommandList::DrawIndexed(std::uint32_t indexCount, std::uint32_t startIndexLocation, std::uint32_t baseVertexLocation)
{
	_commandList->DrawIndexedInstanced(indexCount, 1, startIndexLocation, baseVertexLocation, 0);
}


void RHICommandList::DrawIndexedInstanced(std::uint32_t indexCountPerInstance, std::uint32_t instanceCount, std::uint32_t startIndexLocation, std::uint32_t baseVertexLocation, std::uint32_t startInstanceLocation)
{
	_commandList->DrawIndexedInstanced(indexCountPerInstance, instanceCount, startIndexLocation, baseVertexLocation, startInstanceLocation);
}

void RHICommandList::DrawIndexedIndirect(const gu::SharedPointer<core::GPUBuffer>& argumentBuffer, const std::uint32_t drawCallCount)
{
	const auto dxDevice = gu::StaticPointerCast<directX12::RHIDevice>(_device);
	const auto dxBuffer = gu::StaticPointerCast<directX12::GPUBuffer>(argumentBuffer);
	_commandList->ExecuteIndirect(dxDevice->GetDefaultDrawIndexedIndirectCommandSignature().Get(), drawCallCount, dxBuffer->GetResourcePtr(), 0, nullptr, 0);
}

void RHICommandList::DispatchMesh(const std::uint32_t threadGroupCountX, const std::uint32_t threadGroupCountY, const std::uint32_t threadGroupCountZ)
{
	_commandList->DispatchMesh(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}

void RHICommandList::Dispatch(std::uint32_t threadGroupCountX, std::uint32_t threadGroupCountY, std::uint32_t threadGroupCountZ)
{
	_commandList->Dispatch(threadGroupCountX, threadGroupCountY, threadGroupCountZ);
}

#pragma region Transition Resource State
/****************************************************************************
*                     TransitionResourceState
*************************************************************************//**
*  @fn        void RHICommandList::TransitionResourceState(const gu::SharedPointer<core::GPUTexture>& textures, core::ResourceState afters)
*
*  @brief     Transition a single resource layout using barrier
*
*  @param[in] const gu::SharedPointer<core::GPUTexture>& texture array,
*  @param[in] core::ResourceState state array

*  @return 　　void
*****************************************************************************/
void RHICommandList::TransitionResourceState(const gu::SharedPointer<core::GPUTexture>& texture, core::ResourceState after)
{
	BARRIER barrier = BARRIER::Transition(gu::StaticPointerCast<directX12::GPUTexture>(texture)->GetResource().Get(),
		EnumConverter::Convert(texture->GetResourceState()), EnumConverter::Convert(after));
	_commandList->ResourceBarrier(1, &barrier);
	texture->TransitionResourceState(after);
}

/****************************************************************************
*                     TransitionResourceStates
*************************************************************************//**
*  @fn        void RHICommandList::TransitionResourceStates(const std::uint32_t numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::ResourceState* afters)
*
*  @brief     Transition resource layout using barrier
*
*  @param[in] const std::uint32_t numStates
*  @param[in] const gu::SharedPointer<core::GPUTexture>* texture array,
*  @param[in] core::ResourceState* state array

*  @return 　　void
*****************************************************************************/
void RHICommandList::TransitionResourceStates(const std::uint32_t numStates, const gu::SharedPointer<core::GPUTexture>* textures, core::ResourceState* afters)
{
	gu::DynamicArray<BARRIER> barriers = {};
	for (std::uint32_t i = 0; i < numStates; ++i)
	{
		BARRIER barrier = BARRIER::Transition(gu::StaticPointerCast<directX12::GPUTexture>(textures[i])->GetResource().Get(),
			EnumConverter::Convert(textures[i]->GetResourceState()), EnumConverter::Convert(afters[i]));
		textures[i]->TransitionResourceState(afters[i]);
		barriers.Push(barrier);
	}
	_commandList->ResourceBarrier(numStates, barriers.Data());
}

void RHICommandList::TransitionResourceStates(const gu::DynamicArray<gu::SharedPointer<core::GPUResource>>& resources, core::ResourceState* afters)
{
	gu::DynamicArray<BARRIER> barriers(resources.Size());
	for (uint32 i = 0; i < resources.Size(); ++i)
	{
		if (resources[i]->IsTexture())
		{
			barriers[i] = BARRIER::Transition(gu::StaticPointerCast<directX12::GPUTexture>(resources[i])->GetResource().Get(),
				EnumConverter::Convert(resources[i]->GetResourceState()), EnumConverter::Convert(afters[i]));
			resources[i]->TransitionResourceState(afters[i]);
		}
		else
		{
			barriers[i] = BARRIER::Transition(gu::StaticPointerCast<directX12::GPUBuffer>(resources[i])->GetResource().Get(),
				EnumConverter::Convert(resources[i]->GetResourceState()), EnumConverter::Convert(afters[i]));
			resources[i]->TransitionResourceState(afters[i]);
		}
	}
	_commandList->ResourceBarrier((UINT)barriers.Size(), barriers.Data());
}

#pragma endregion Transition Resource State
#pragma region Copy 
/****************************************************************************
*                     CopyResource
*************************************************************************//**
*  @fn      void RHICommandList::CopyResource(const gu::SharedPointer<core::GPUTexture>& dest, const gu::SharedPointer<core::GPUTexture>& source)

*
*  @brief   あるリソースの領域をまとめて別のリソースにコピーする.
*           組み合わせに応じて自動でバッファかテクスチャかを判定します
*
*  @param[in] const gu::SharedPointer<core::GPUTexture>& コピー先のバッファ
*  @param[in] const gu::SharedPointer<core::GPUTexture>& コピー元のバッファ

*  @return 　　void
*****************************************************************************/
void RHICommandList::CopyResource(const gu::SharedPointer<core::GPUTexture>& dest, const gu::SharedPointer<core::GPUTexture>& source)
{
	CopyResource(gu::StaticPointerCast<core::GPUResource>(dest), gu::StaticPointerCast<core::GPUResource>(source));
}

/****************************************************************************
*                     CopyResource
*************************************************************************//**
*  @fn        void RHICommandList::CopyResource(const gu::SharedPointer<core::GPUResource>& dest, const gu::SharedPointer<core::GPUResource>& source)
*
*  @brief     あるリソースの領域をまとめて別のリソースにコピーする. 
*           組み合わせに応じて自動でバッファかテクスチャかを判定します
*
*  @param[in] const gu::SharedPointer<core::GPUBuffer> コピー先のバッファ
*  @param[in] const gu::SharedPointer<core::GPUBuffer>& コピー元のバッファ

*  @return 　　void
*****************************************************************************/
void RHICommandList::CopyResource(const gu::SharedPointer<core::GPUResource>& dest, const gu::SharedPointer<core::GPUResource>& source)
{
	gu::SharedPointer<core::GPUResource> resources[] = { dest, source };
	rhi::core::ResourceState befores[] = { dest->GetResourceState()            , source->GetResourceState() };
	rhi::core::ResourceState afters[] = { core::ResourceState::CopyDestination, core::ResourceState::CopySource };

	TransitionResourceStates({ dest, source }, afters);

	if (dest->IsTexture() && source->IsTexture())
	{
		_commandList->CopyResource(
			gu::StaticPointerCast<directX12::GPUTexture>(dest)->GetResource().Get(),
			gu::StaticPointerCast<directX12::GPUTexture>(source)->GetResource().Get());
	}
	else if (dest->IsTexture() && !source->IsTexture())
	{
		_commandList->CopyResource(
			gu::StaticPointerCast<directX12::GPUTexture>(dest)->GetResource().Get(),
			gu::StaticPointerCast<directX12::GPUBuffer>(source)->GetResource().Get());
	}
	else
	{
		_commandList->CopyResource(
			gu::StaticPointerCast<directX12::GPUBuffer>(dest)->GetResource().Get(),
			gu::StaticPointerCast<directX12::GPUBuffer>(source)->GetResource().Get());
	}

	TransitionResourceStates({ dest, source }, befores);
}

/****************************************************************************
*                     CopyBufferRegion
*************************************************************************//**
*  @fn        void RHICommandList::CopyBufferRegion(const gu::SharedPointer<core::GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<core::GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize)
*
*  @brief     バッファの領域をあるリソースから別のリソースにコピーする.
*
*  @param[in] const gu::SharedPointer<core::GPUBuffer> コピー先のバッファ
*  @param[in] const gu::uint64 destOffset コピー先の書き出す初期バイト数, (ポインタでないことに注意)
*  @param[in] const gu::SharedPointer<core::GPUBuffer>& コピー元のバッファ, 
*  @param[in] const gu::uint64 sourceOffset コピー元の書き出す初期バイト数 (ポインタでないことに注意), 
*  @param[in] const gu::uint64 copyByteSize コピーしたいバイト数

*  @return 　　void
*****************************************************************************/
void RHICommandList::CopyBufferRegion(const gu::SharedPointer<core::GPUBuffer>& dest, const gu::uint64 destOffset, const gu::SharedPointer<core::GPUBuffer>& source, const gu::uint64 sourceOffset, const gu::uint64 copyByteSize)
{
	using enum core::ResourceState;

	const auto rhiDestBuffer   = static_cast<directX12::GPUBuffer*>(dest.Get());
	const auto rhiSourceBuffer = static_cast<directX12::GPUBuffer*>(source.Get());
	const auto destResource    = rhiDestBuffer->GetResourcePtr();
	const auto sourceResource  = rhiSourceBuffer->GetResourcePtr();

	/*-------------------------------------------------------------------
	-           Copyable resource check
	---------------------------------------------------------------------*/
	Checkf(sourceResource != destResource, "CopyBufferRegion cannot be used on the same resource. This can happen when both the source and the dest are suballocated from the same resource.");
	Check(destOffset   + copyByteSize <= rhiDestBuffer  ->GetTotalByteSize());
	Check(sourceOffset + copyByteSize <= rhiSourceBuffer->GetTotalByteSize());

	/*-------------------------------------------------------------------
	-           Prepare copy barrier resource 
	---------------------------------------------------------------------*/
	core::ResourceState befores[] = { dest->GetResourceState(), source->GetResourceState() };
	core::ResourceState afters[]  = { CopyDestination, CopySource };

	// バリアにより, リソースの読み方を伝える
	TransitionResourceStates({ dest, source }, afters);

	/*-------------------------------------------------------------------
	-           Prepare copy barrier resource
	---------------------------------------------------------------------*/
	_commandList->CopyBufferRegion(destResource, destOffset, sourceResource, sourceOffset, copyByteSize);

	/*-------------------------------------------------------------------
	-           After copy barrier resource
	---------------------------------------------------------------------*/
	TransitionResourceStates({ dest, source }, befores);
}


#pragma endregion Copy
#pragma endregion GPU Command
#pragma region Property
void RHICommandList::SetName(const gu::tstring& name)
{
	_commandList->SetName(name.CString());
}

#pragma endregion Property
#pragma region Private Function
/****************************************************************************
*                     BeginRenderPassImpl
*************************************************************************//**
*  @fn        void RHICommandList::BeginRenderPassImpl(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)<directX12::RHIFrameBuffer>& frameBuffer)
*  @brief     If you used render pass, this function is called. Set frame buffer (render target and depth stencil) to command buffer.
*             When you use core::AttachmentLoad::Clear, color or depthStencil frame buffer is cleared.
*  @param[in] const gu::SharedPointer<directX12::RHIRenderPass>& renderPass
*  @param[in] const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer
*  @return 　　void
*****************************************************************************/
void RHICommandList::BeginRenderPassImpl(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Get frame buffer resources
	---------------------------------------------------------------------*/
	const bool hasRTV  = frameBuffer->GetRenderTargetSize() != 0;
	const bool hasDSV  = frameBuffer->GetDepthStencil();
	/*-------------------------------------------------------------------
	-          Set CPU rtv and dsv descriptor handle
	---------------------------------------------------------------------*/
	// render target 
	gu::DynamicArray<D3D12_RENDER_PASS_RENDER_TARGET_DESC> rtvDescs(frameBuffer->GetRenderTargetSize());
	for (size_t i = 0; i < rtvDescs.Size(); ++i)
	{
		// get rtv handle
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetRenderTargetView(i));
#ifdef _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::RenderTarget);
#endif

		const auto rtvHandle = view->GetCPUHandler();
		if (!rtvHandle.ptr) { continue; }

		// set render pass load op
		D3D12_RENDER_PASS_BEGINNING_ACCESS begin = {};
		begin.Type = EnumConverter::Convert(renderPass->GetColorAttachment(i)->LoadOp);
		begin.Clear.ClearValue.Color[0] = renderPass->GetClearColor()[i].Color[core::ClearValue::Red];
		begin.Clear.ClearValue.Color[1] = renderPass->GetClearColor()[i].Color[core::ClearValue::Green];
		begin.Clear.ClearValue.Color[2] = renderPass->GetClearColor()[i].Color[core::ClearValue::Blue];
		begin.Clear.ClearValue.Color[3] = renderPass->GetClearColor()[i].Color[core::ClearValue::Alpha];
		// set render pass store op
		D3D12_RENDER_PASS_ENDING_ACCESS end = {};
		end.Type = EnumConverter::Convert(renderPass->GetColorAttachment(i)->StoreOp);
		
		// set render pass descriptor
		rtvDescs[i].BeginningAccess = begin;
		rtvDescs[i].cpuDescriptor   = rtvHandle;
		rtvDescs[i].EndingAccess    = end;
	}
	// depth stencil
	D3D12_RENDER_PASS_DEPTH_STENCIL_DESC dsvDesc = {};
	if (hasDSV)
	{
		// get dsv handle
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetDepthStencilView());

#if __DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::DepthStencil);
#endif

		const auto dsvHandle = view->GetCPUHandler();

		// set depth render pass load op
		D3D12_RENDER_PASS_BEGINNING_ACCESS depthBegin   = { EnumConverter::Convert(renderPass->GetDepthAttachment()->LoadOp), {} };
		D3D12_RENDER_PASS_ENDING_ACCESS    depthEnd     = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StoreOp), {} };
		D3D12_RENDER_PASS_BEGINNING_ACCESS stencilBegin = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StencilLoad), {} };
		D3D12_RENDER_PASS_ENDING_ACCESS    stencilEnd   = { EnumConverter::Convert(renderPass->GetDepthAttachment()->StencilStore), {} };
		depthBegin.Clear.ClearValue.DepthStencil.Depth     = renderPass->GetDepthClear()->Depth;
		stencilBegin.Clear.ClearValue.DepthStencil.Stencil = renderPass->GetDepthClear()->Stencil;
		
		// set depth stencil descriptor
		dsvDesc.DepthBeginningAccess   = depthBegin;
		dsvDesc.DepthEndingAccess      = depthEnd;
		dsvDesc.StencilBeginningAccess = stencilBegin;
		dsvDesc.StencilEndingAccess    = stencilEnd;
		dsvDesc.cpuDescriptor          = dsvHandle;
	}

	_commandList->BeginRenderPass(static_cast<std::uint32_t>(rtvDescs.Size()), hasRTV ? rtvDescs.Data() : nullptr, hasDSV ? &dsvDesc : nullptr, D3D12_RENDER_PASS_FLAG_NONE);
}

/****************************************************************************
*                     OMSetFrameBuffer
*************************************************************************//**
*  @fn        void RHICommandList::OMSetFrameBuffer(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)
*  @brief     If not used render pass, this function is called. Set frame buffer (render target and depth stencil) to command buffer.
*             When you use core::AttachmentLoad::Clear, color or depthStencil frame buffer is cleared.
*  @param[in] const gu::SharedPointer<directX12::RHIRenderPass>& renderPass
*  @param[in] const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer
*  @return 　　void
*****************************************************************************/
void RHICommandList::OMSetFrameBuffer(const gu::SharedPointer<directX12::RHIRenderPass>& renderPass, const gu::SharedPointer<directX12::RHIFrameBuffer>& frameBuffer)
{
	/*-------------------------------------------------------------------
	-          Get frame buffer resources
	---------------------------------------------------------------------*/
	auto renderTargets = frameBuffer->GetRenderTargets();
	auto depthStencil  = frameBuffer->GetDepthStencil();
	const bool hasRTV  = frameBuffer->GetRenderTargetSize() != 0;
	const bool hasDSV  = frameBuffer->GetDepthStencil();
	/*-------------------------------------------------------------------
	-          Set CPU rtv and dsv descriptor handle
	---------------------------------------------------------------------*/
	gu::DynamicArray<D3D12_CPU_DESCRIPTOR_HANDLE> rtvHandle(renderTargets.Size());
	for (size_t i = 0; i < rtvHandle.Size(); ++i)
	{
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetRenderTargetView(i));

#if _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::RenderTarget);
#endif 

		rtvHandle[i] = view->GetCPUHandler();
	}
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = {};
	if (hasDSV)
	{
		const auto view = gu::StaticPointerCast<directX12::GPUResourceView>(frameBuffer->GetDepthStencilView());
#if _DEBUG
		assert(view->GetResourceViewType() == core::ResourceViewType::DepthStencil);
#endif
		dsvHandle = view->GetCPUHandler();
	}
	/*-------------------------------------------------------------------
	-          Clear frame buffer resources
	---------------------------------------------------------------------*/
	// render target
	for (int i = 0; i < rtvHandle.Size(); ++i)
	{
		const auto colorAttachment = renderPass->GetColorAttachment(i);
		if (colorAttachment->LoadOp != core::AttachmentLoad::Clear) { continue; }
		_commandList->ClearRenderTargetView(rtvHandle[i], renderPass->GetClearColor()[i].Color, 0, nullptr);
	}
	// depth stencil 
	if (hasDSV)
	{
		const auto depthAttachment = renderPass->GetDepthAttachment();

		// set clear flag 
		D3D12_CLEAR_FLAGS clearFlags = {};
		if (depthAttachment->LoadOp      == core::AttachmentLoad::Clear) { clearFlags |= D3D12_CLEAR_FLAG_DEPTH; }
		if (depthAttachment->StencilLoad == core::AttachmentLoad::Clear) { clearFlags |= D3D12_CLEAR_FLAG_STENCIL; }
		// clear depth stencil
		if (clearFlags)
		{
			_commandList->ClearDepthStencilView(dsvHandle, clearFlags, renderPass->GetDepthClear()->Depth, renderPass->GetDepthClear()->Stencil, 0, nullptr);
		}
	}
	/*-------------------------------------------------------------------
	-          Set render target and depth stencil
	---------------------------------------------------------------------*/
	_commandList->OMSetRenderTargets(static_cast<std::uint32_t>(rtvHandle.Size()), hasRTV ? rtvHandle.Data() : nullptr, FALSE, hasDSV ? &dsvHandle : nullptr);
}

#pragma endregion Private Function