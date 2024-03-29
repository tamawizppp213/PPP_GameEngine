//////////////////////////////////////////////////////////////////////////////////
///             @file   UISlider.hpp
///             @brief  Slider object
///             @author Toide Yutaro
///             @date   2022_12_27
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef UISLIDER_HPP
#define UISLIDER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "UIImage.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include <vector>
#include <functional>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class GPUResourceView;
	class GPUResourceCache;
};

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::ui
{
	struct UIResource
	{
	private:
		using ResourceViewPtr = gu::SharedPointer<rhi::core::GPUResourceView>;
	public:
		ResourceViewPtr ResourceView;
		Image           Image;
	};
	/****************************************************************************
	*				  			    Slider
	*************************************************************************//**
	*  @class     Slider
	*  @brief     UI : Slider (現在は左から右までの範囲のみ対応してます. 上下方向は今後追加する予定です.)
	*****************************************************************************/
	class Slider
	{
		using ListenerFunction = std::function<void(float)>;
		using FunctionPtr      = gu::SharedPointer<ListenerFunction>;
		using ResourceCachePtr = gu::SharedPointer<rhi::core::GPUResourceCache>;
		using ResourceViewPtr  = gu::SharedPointer<rhi::core::GPUResourceView>;
	public:
		/****************************************************************************
		**                Enum
		*****************************************************************************/
		enum RenderType
		{
			BackGround,
			Color,
			CountOf
		};

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void CreateInScreenSpace(const gm::Float3& position,
			const gm::Float2& rectSize,
			const gm::Float4& fillColor = gm::Float4(1, 1, 1, 1));

		void CreateInNDCSpace(
			const gm::Float3& position  = gm::Float3(0, 0, 0),
			const gm::Float2& rectSize  = gm::Float2(2, 2),
			const gm::Float4& fillColor = gm::Float4(1, 1, 1, 1));

		/* @brief : Add the difference to the current slider value */
		void Add(const float difference);

		/* @brief : Add step size to the current slider value*/
		void Step(const bool isPlus); // + or -. add stepsize

		// onValueChanged function
		void AddListener   (FunctionPtr function) {_functionList.push_back(function); };
		void RemoveListener(FunctionPtr function) { std::erase(_functionList, function); }; // C++20

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const UIResource& GetRenderResource(const RenderType type) { return _resourceList[type]; };

		/* Colorの場合はResources/Preset/NullAlbedoMap.pngを指定すると良いです.*/
		void SetTexture(const RenderType type, const ResourceViewPtr& view) { _resourceList[type].ResourceView = view; }

		/*-------------------------------------------------------------------
		-             Slider value
		---------------------------------------------------------------------*/
		float GetValue   () const { return _value; }
		float GetMaxValue() const { return _maxValue; }
		float GetMinValue() const { return _minValue; }

		void SetValue   (float value);
		void SetMaxValue(const float value) { _maxValue = value; }
		void SetMinValue(const float value) { _minValue = value; }

		void SetStepSize(const float stepSize) { _stepSize = stepSize; }

		void SetFillColor(const gm::Float4& fillColor);

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Slider();

		Slider(const float value, const float maxValue = 100, const float minValue = 0, const float stepSize = 1.0f);

		~Slider();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/* 値が変化したとき登録している関数を順番に実行する.*/
		bool OnValueChanged(const float newValue);

		bool InitializeSliderCanvas(const bool isNDC, const gm::Float3& position, const gm::Float2& rectSize, const gm::Float4 fillColor);

		void RecreateSliderUI();
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		bool _isInteractive = true;

		// slider value
		float _minValue = 0.0f;
		float _maxValue = 100.0f;
		float _value    = 50.0f;
		
		// The amount to adjust the value by, when using a controller or keyboard
		float _stepSize = 1.0f;

		gm::Float3 _leftEdgePosition   = { 0,0,0 };
		gm::Float2 _fullRectSize       = { 0,0 };
		gm::Float4 _fillColor          = {1,1,1,1};

		bool _initialized = false;

		// callback function list (関数登録時は必ずvoid型を返り値とするfloat引数(slider value)を持った関数を設定してください. )
		// When registering a function, be sure to set a function with a float argument (slider value) with return void. 
		std::vector<FunctionPtr> _functionList = {};

		std::vector<UIResource> _resourceList = {}; // 0: Background image, 1: Fill rectangle
	};
}
#endif