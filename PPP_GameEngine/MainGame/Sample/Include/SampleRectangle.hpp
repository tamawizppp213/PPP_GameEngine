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
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace ui
{
	class Image;
	class UIRenderer;
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
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, GameTimer* gameTimer) override;
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
	};
}
#endif