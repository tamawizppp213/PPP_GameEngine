//////////////////////////////////////////////////////////////////////////////////
///             @file   SampleRectangle.hpp
///             @brief  Rectangle mesh sample
///             @author Toide Yutaro
///             @date   2022_10_30
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAMPLE_RECTANGLE_HPP
#define SAMPLE_RECTANGLE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/Scene.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class GPUBuffer;
	class GPUGraphicsPipelineState;
	class RHIResourceLayout;
}
namespace sample
{
	/****************************************************************************
	*				  			SampleRectangle
	*************************************************************************//**
	*  @class     SampleRectangle
	*  @brief     Rectangle color mesh sample 
	*****************************************************************************/
	class SampleRectangle : public Scene
	{
		using VertexBufferPtr   = std::shared_ptr<rhi::core::GPUBuffer>;
		using IndexBufferPtr    = std::shared_ptr<rhi::core::GPUBuffer>;
		using ResourceLayoutPtr = std::shared_ptr<rhi::core::RHIResourceLayout>;
		using PipelineStatePtr  = std::shared_ptr<rhi::core::GPUGraphicsPipelineState>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Initialize(const PPPEnginePtr& engine, const GameTimerPtr& gameTimer) override;
		void Update() override;
		void Draw() override;
		void Terminate() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SampleRectangle();
		~SampleRectangle();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void LoadMaterials() override;
		void OnKeyboardInput() override;
		void OnMouseInput() override;
		void OnGamePadInput() override;
		void ExecuteSceneTransition() override;
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		VertexBufferPtr   _vertexBuffer   = nullptr;
		IndexBufferPtr    _indexBuffer    = nullptr;
		PipelineStatePtr  _pipelineState  = nullptr;
		ResourceLayoutPtr _resourceLayout = nullptr;
	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void BuildBuffer();
		void BuildPipelineState();
	};
}
#endif