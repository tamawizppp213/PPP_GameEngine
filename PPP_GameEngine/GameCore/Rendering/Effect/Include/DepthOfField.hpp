//////////////////////////////////////////////////////////////////////////////////
///             @file   DepthOfField.hpp
///             @brief  DepthOfField
///             @author Toide Yutaro
///             @date   2023_02_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DEPTH_OF_FIELD_HPP
#define DEPTH_OF_FIELD_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include <memory>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;

namespace rhi::core
{
	class GPUBuffer;
	class GPUTexture;
	class GPUResourceView;
	class RHIResourceLayout;
	class RHIRenderPass;
	class RHIFrameBuffer;
	class GPUComputePipelineState;
}

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{

	/****************************************************************************
	*				  			Dof
	*************************************************************************//**
	*  @class     Dof
	*  @brief     Depth Of Field (linear depth)
	*  @note      問題点: zPrepassが作られてない.
	*****************************************************************************/
	class Dof : public NonCopyable
	{
	protected:
		struct BlurParameter
		{
			float TextureSize[2];
			float Radius;
			float Padding = 0.0f;
		};

		struct ClipSize
		{
			float Near;
			float Far;
			float Padding[2] = {0.0f, 0.0f};
		};

		using GPUBufferPtr              = std::shared_ptr<rhi::core::GPUBuffer>;
		using GPUTexturePtr             = std::shared_ptr<rhi::core::GPUTexture>;
		using ResourceViewPtr           = std::shared_ptr<rhi::core::GPUResourceView>;
		using FrameBufferPtr            = std::shared_ptr<rhi::core::RHIFrameBuffer>;
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using PipelineStatePtr          = std::shared_ptr<rhi::core::GPUComputePipelineState>;
		using RenderPassPtr             = std::shared_ptr<rhi::core::RHIRenderPass>;
		using ResourceLayoutPtr         = std::shared_ptr<rhi::core::RHIResourceLayout>;

	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void OnResize(const float newWidth, const float newHeight);

		void Draw(const ResourceViewPtr& zPrepass);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		void SetUpBlurParameter(float width, float height, float radius = 20.0f);

		void SetUpClipSize(float nearClip = 0.2f, float farClip = 0.8f);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Dof(const LowLevelGraphicsEnginePtr& engine, 
			const float width, const float height, const float radius = 10.0f, 
			const float nearClip = 0.2f, const float farClip = 0.8f, const std::wstring& addName = L"");

		virtual ~Dof();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareRenderBuffer(const size_t width , const size_t height);

		void PrepareBlurParameterBuffer(const float width, const float height, const float radius, const std::wstring& name);

		void PrepareClipSizeBuffer(const float nearClip, const float farClip, const std::wstring& name);

		void PreparePipelineState(const std::wstring& name);

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		enum {ViewCount = 6};
		
		// @brief Blur setting
		ResourceViewPtr _blurParameterView = nullptr;
		BlurParameter   _blurParameter = {};

		/* @brief : Clip size setting*/
		ResourceViewPtr _clipSizeView = nullptr;
		ClipSize        _clipSize = {};
		
		/*-------------------------------------------------------------------
		-             Render resource
		---------------------------------------------------------------------*/
		PipelineStatePtr _verticalPipeline    = nullptr;
		PipelineStatePtr _rhomboidPipeline    = nullptr;
		PipelineStatePtr _finalRenderPipeline = nullptr;

		ResourceLayoutPtr _resourceLayout = nullptr;


		/* @brief : Texture rendering setting*/
		std::vector<ResourceViewPtr> _shaderResourceViews = {};
		std::vector<ResourceViewPtr> _unorderedAccessViews = {};

		LowLevelGraphicsEnginePtr _engine = nullptr;

		static constexpr std::uint32_t THREAD = 16;
	};
}
#endif