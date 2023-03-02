//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12GPUFactory.cpp
///             @brief  Pipeline each stage creator
///             @author Toide Yutaro
///             @date   2022_06_30
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/DirectX12GPUPipelineFactory.hpp"
#include "../Include/DirectX12GPUBlendState.hpp"
#include "../Include/DirectX12GPUDepthStencilState.hpp"
#include "../Include/DirectX12GPUInputAssemblyState.hpp"
#include "../Include/DirectX12GPURasterizerState.hpp"
#include "../Include/DirectX12GPUShaderState.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUPipelineFactory::GPUPipelineFactory(const std::shared_ptr<core::RHIDevice>& device)
	:core::GPUPipelineFactory(device)
{

}

std::shared_ptr<core::GPUInputAssemblyState> GPUPipelineFactory::CreateInputAssemblyState(
	const std::vector<core::InputLayoutElement>& elements,
	const core::PrimitiveTopology primitiveTopology)
{
	return std::static_pointer_cast<core::GPUInputAssemblyState>(
		std::make_shared<directX12::GPUInputAssemblyState>(_device, elements, primitiveTopology));
}

std::shared_ptr<core::GPURasterizerState> GPUPipelineFactory::CreateRasterizerState(
	const core::RasterizerProperty& rasterizerProperty)
{
	return std::static_pointer_cast<core::GPURasterizerState>(
		std::make_shared<GPURasterizerState>(_device, rasterizerProperty));
}

std::shared_ptr<core::GPUDepthStencilState> GPUPipelineFactory::CreateDepthStencilState(
	const core::DepthStencilProperty& depthStencilProperty)
{
	return std::static_pointer_cast<core::GPUDepthStencilState>(
		std::make_shared<GPUDepthStencilState>(_device,depthStencilProperty));
}

std::shared_ptr<core::GPUShaderState> GPUPipelineFactory::CreateShaderState()
{
	return std::static_pointer_cast<core::GPUShaderState>(std::make_shared<GPUShaderState>(_device));
}

std::shared_ptr<core::GPUBlendState> GPUPipelineFactory::CreateBlendState(
	const std::vector<core::BlendProperty>& properties)
{
	return std::static_pointer_cast<core::GPUBlendState>(std::make_shared<GPUBlendState>(_device, properties));
}

std::shared_ptr<core::GPUBlendState> GPUPipelineFactory::CreateSingleBlendState(
	const core::BlendProperty& blendProperty)
{
	return std::static_pointer_cast<core::GPUBlendState>(std::make_shared<GPUBlendState>(_device, blendProperty));
}

std::shared_ptr<core::GPUBlendState> GPUPipelineFactory::CreateBlendState(const size_t numRenderTargets)
{
	return std::static_pointer_cast<core::GPUBlendState>(std::make_shared<GPUBlendState>(_device, std::vector<rhi::core::BlendProperty>(numRenderTargets)));
}