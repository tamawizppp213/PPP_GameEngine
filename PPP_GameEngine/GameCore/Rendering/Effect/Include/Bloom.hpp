//////////////////////////////////////////////////////////////////////////////////
///             @file   Bloom.hpp
///             @brief  Bloom effect 
///             @author Toide Yutaro
///             @date   2022_11_20
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef BLOOM_HPP
#define BLOOM_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "Blur.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class GPUComputePipelineState;
	class RHIResourceLayout;
	class GPUResourceView;
}
namespace gc
{

	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     Class
	*  @brief     temp
	*****************************************************************************/
	class Bloom : public NonCopyable
	{
		using ResourceLayoutPtr         = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using PipelineStatePtr          = std::shared_ptr<rhi::core::GPUComputePipelineState>;
		using GaussianBlurPtr           = std::shared_ptr<GaussianBlur>;
		using ResourceViewPtr     = std::shared_ptr<rhi::core::GPUResourceView>;
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void OnResize(const std::uint32_t newWidth, const std::uint32_t newHeight);
		void Draw();
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		void UpdateBloomPower(const float power);
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Bloom() = default;
		Bloom(const LowLevelGraphicsEnginePtr& engine, const std::uint32_t width, const std::uint32_t height, const float power = 10.0f, const std::wstring& addName = L"");
		~Bloom();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void PrepareGaussianBlurs(const std::uint32_t width, const std::uint32_t height, const std::wstring& name);
		void PreparePipelineState(const std::wstring& name);
		void PrepareResourceView (const std::wstring& name);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		enum {ViewCount = 4};
		PipelineStatePtr _luminancePipeline  = nullptr;
		PipelineStatePtr _finalBloomPipeline = nullptr;
		GaussianBlurPtr  _gaussianBlur[4] = {};
		ResourceLayoutPtr _resourceLayout = nullptr;
		ResourceViewPtr _shaderResourceViews   [ViewCount] = {};
		ResourceViewPtr _unorderedResourceViews[ViewCount] = {};
		LowLevelGraphicsEnginePtr _engine = nullptr;

		float _explosion = 10.0f;
		static constexpr std::uint32_t THREAD = 16;
	};
}
#endif