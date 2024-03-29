//////////////////////////////////////////////////////////////////////////////////
///             @file   Font.hpp
///             @brief  Font
///             @author Toide Yutaro
///             @date   2022_04_16
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef FONT_HPP
#define FONT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Math/Include/GMVector.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace rhi::core
{
	class GPUTexture;
	class GPUResourceView;
}
namespace gc::ui
{
	/****************************************************************************
	*				  			FontLoader
	*************************************************************************//**
	*  @class     FontLoader
	*  @brief     Font load and register static class
	*****************************************************************************/
	class Font: public gu::NonCopyable
	{
		using LowLevelGraphicsEnginePtr = gu::SharedPointer<LowLevelGraphicsEngine>;
		using TexturePtr      = gu::SharedPointer<rhi::core::GPUTexture>;
		using ResourceViewPtr = gu::SharedPointer<rhi::core::GPUResourceView>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Load texture*/
		bool Load(const LowLevelGraphicsEnginePtr& engine, const gu::tstring& imagePath, const gm::Float2& pixelPerChar, const float imagePixelWidth);

		/* @brief: Reset texture resource and infomation*/
		void Reset();

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*@brief: Return shared_ptr<GPUResourceView> font image and font resource view*/
		ResourceViewPtr GetFontResourceView() const noexcept;

		/* @brief: Return font texture shared_ptr*/
		TexturePtr GetFontTexture() const noexcept;

		/* @brief : Call load function. (Has existed texture view pointer)*/
		bool HasLoaded() const noexcept { return !_textureView; }
		
		/* @brief : Each character pixel size*/
		const gm::Float2& GetPixelPerChar() const { return _pixelPerChar; }

		/* @brief : Total image width */
		float GetImagePixelWidth() const { return _imagePixelWidth; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Font() = default;

		/* @brief : Load font constructor*/
		Font(const LowLevelGraphicsEnginePtr& engine, const gu::tstring& imagePath, const gm::Float2& pixelPerChar, const float imagePixelWidth);
		
		~Font();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		/* @brief : Each character pixel size*/
		gm::Float2      _pixelPerChar    = {};

		/* @brief : Total image width */
		float           _imagePixelWidth = 0;

		/* @brief : Texture and texture view resource*/
		ResourceViewPtr _textureView    = nullptr;
	};
}
#endif