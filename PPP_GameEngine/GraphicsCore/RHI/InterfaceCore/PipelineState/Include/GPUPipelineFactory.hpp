//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUPipelineFactory.hpp
///             @brief  Blend State
///             @author Toide Yutaro
///             @date   2022_06_28
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GPU_PIPELINE_FACTORY_HPP
#define GPU_PIPELINE_FACTORY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayoutElement.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <memory>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHIDevice;
	class GPUInputAssemblyState;
	class GPURasterizerState;
	class GPUDepthStencilState;
	class GPUShaderState;
	class GPUBlendState;
	/****************************************************************************
	*				  			RHIPipelineState
	*************************************************************************//**
	*  @class     RHIPipelineState
	*  @brief     PipelineState
	*****************************************************************************/
	class GPUPipelineFactory : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual std::shared_ptr<GPUInputAssemblyState> CreateInputAssemblyState(
			const std::vector<InputLayoutElement>& elements,
			const PrimitiveTopology primitiveTopology = PrimitiveTopology::TriangleList) = 0;

		virtual std::shared_ptr<GPURasterizerState> CreateRasterizerState(
			const FrontFace   frontFace   = FrontFace::Clockwise,
			const CullingMode cullingMode = CullingMode::None,
			const FillMode    fillMode    = FillMode::Solid,
			const bool depthClamp = true) = 0;

		virtual std::shared_ptr<GPUDepthStencilState> CreateDepthStencilState(
			const bool                 depthEnable      = true,
			const bool                 depthWriteEnable = true,
			const bool                 stencilEnable    = false,
			const CompareOperator      depthOperator    = CompareOperator::LessEqual,
			const StencilOperatorInfo& front            = StencilOperatorInfo(),
			const StencilOperatorInfo& back             = StencilOperatorInfo()) = 0;

		virtual std::shared_ptr<GPUShaderState> CreateShaderState(
			const ShaderType type,
			const std::string& fileName,
			const std::string& entryPoint    = "main",
			const std::string& shaderVersion = "6.6"
		) = 0;

		virtual std::shared_ptr<GPUBlendState> CreateBlendState(
			const std::vector<BlendProperty>& properties = { BlendProperty() }) = 0;

		virtual std::shared_ptr<GPUBlendState> CreateSingleBlendState(
			const BlendProperty& blendProperty = BlendProperty()
		) = 0;

		virtual std::shared_ptr<GPUBlendState> CreateBlendState(const size_t numRenderTargets) = 0;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		virtual ~GPUPipelineFactory() = default;
		explicit GPUPipelineFactory(const std::shared_ptr<RHIDevice>& device) : _device(device) {};
		GPUPipelineFactory() = default;
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;

	};
}
#endif