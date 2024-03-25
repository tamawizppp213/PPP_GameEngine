//////////////////////////////////////////////////////////////////////////////////
///             @file   Font.hpp
///             @brief  Font
///             @author Toide Yutaro
///             @date   2022_04_16
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/UIFont.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceCache.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::ui;
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
/* @brief : Load font constructor*/
Font::Font(const LowLevelGraphicsEnginePtr& engine, const gu::tstring& imagePath, const gm::Float2& pixelPerChar, const float imagePixelWidth)
	: _engine(engine), _pixelPerChar(pixelPerChar), _imagePixelWidth(imagePixelWidth)
{
	Load(engine, imagePath, pixelPerChar, imagePixelWidth);
}

/* @brief : Font Destructor*/
Font::~Font()
{
	Reset();
}
#pragma endregion Constructor and Destructor

#pragma region Main Function
/****************************************************************************
*					Load
*************************************************************************//**
*  @fn        bool Font::Load(const LowLevelGraphicsEnginePtr& engine, const gu::tstring& imagePath, const gm::Float2& pixelPerChar, const float imagePixelWidth)
*  
*  @brief     Load font texture
* 
*  @param[in] const LowLevelGraphicsEnginePtr& engine
*  @param[in] const std::string& imagePath
*  @param[in] const gm::Float2&  pixelPerChar
*  @param[in] const float        imagePixelWidth
* 
*  @return �@�@bool (true : Load success, false : already loaded, error : failed to find texture image path)
*****************************************************************************/
bool Font::Load(const LowLevelGraphicsEnginePtr& engine, const gu::tstring& imagePath, [[maybe_unused]] const gm::Float2& pixelPerChar, [[maybe_unused]]const float imagePixelWidth)
{
	/*-------------------------------------------------------------------
	-             Load check
	---------------------------------------------------------------------*/
	if (HasLoaded()) { OutputDebugStringA("Already load\n"); return false; }

	/*-------------------------------------------------------------------
	-             Set up
	---------------------------------------------------------------------*/
	const auto device        = engine->GetDevice();
	const auto commandList   = engine->GetCommandList(CommandListType::Graphics);
	const auto resourceCache = gu::MakeShared<GPUResourceCache>(_engine->GetDevice(), commandList);

	/*-------------------------------------------------------------------
	-             Load texture
	---------------------------------------------------------------------*/
	_textureView = resourceCache->Load(imagePath);
	
	return true;
}

/****************************************************************************
*					Reset
*************************************************************************//**
*  @fn        void Font::Reset()
*
*  @brief     Reset font resource
*
*  @param[in] void
*
*  @return �@�@void
*****************************************************************************/
void Font::Reset()
{
	_textureView.Reset();
	_pixelPerChar    = { 0.0f, 0.0f };
	_imagePixelWidth = 0;
	_engine = nullptr;
}
#pragma endregion Main Function

#pragma region Property
Font::ResourceViewPtr Font::GetFontResourceView() const noexcept
{
	return _textureView;
}

Font::TexturePtr Font::GetFontTexture() const noexcept
{
	return _textureView->GetTexture();
}
#pragma endregion Property