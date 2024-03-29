//////////////////////////////////////////////////////////////////////////////////
///             @file   SampleSky.hpp
///             @brief  Skybox sample
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAMPLE_COLOR_CHANGE_HPP
#define SAMPLE_COLOR_CHANGE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/Scene.hpp"
#include <memory>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gc
{
	class SkyDome;
	class Camera;
	class ColorChange;
	class GaussianBlur;
	class Dof;
	class Mosaic;
	class Vignette;
	class WhiteBalance;
}

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace sample
{

	/****************************************************************************
	*				  			SampleSky
	*************************************************************************//**
	*  @class     SampleSky
	*  @brief     Skybox sample
	*****************************************************************************/
	class SampleColorChange : public Scene
	{
		using SkyDomePtr = gu::SharedPointer<gc::SkyDome>;
		using CameraPtr  = gu::SharedPointer<gc::Camera>;
		using ColorChangePtr  = gu::SharedPointer<gc::ColorChange>;
		using GaussianBlurPtr = gu::SharedPointer<gc::GaussianBlur>;
		using MosaicPtr = gu::SharedPointer<gc::Mosaic>;
		using VignettePtr = gu::SharedPointer<gc::Vignette>;
		using WhiteBalancePtr = gu::SharedPointer<gc::WhiteBalance>;
		
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
		SampleColorChange();
		~SampleColorChange();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void LoadMaterials() override;
		void OnKeyboardInput() override;
		void OnMouseInput() override;
		void OnGamePadInput() override;
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		SkyDomePtr _skybox = nullptr;

		CameraPtr _camera = nullptr;

		std::vector<ColorChangePtr> _colorChanges = {};

		GaussianBlurPtr _gaussianBlur = nullptr;

		MosaicPtr _mosaic = nullptr;

		VignettePtr _vignette = nullptr;

		WhiteBalancePtr _whiteBalance = nullptr;

		bool _useBlur = false;
		bool _useMosaic = false;
		bool _useVignette = false;
		bool _useWhiteBalance = false;
		bool _useScreenSpaceReflection = false;

		std::uint32_t _colorIndex = 0;
	};
}
#endif