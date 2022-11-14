//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleTexture.hpp"
#include "GameCore/Rendering/Sprite/Include/UIRenderer.hpp"
#include "GameCore/Rendering/Sprite/Include/Image.hpp"
#include "GameUtility/Base/Include/Screen.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceCache.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace sample;
using namespace ui;
using namespace rhi;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SampleTexture::SampleTexture()
{

}
SampleTexture::~SampleTexture()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        void SampleEmpty::Initialize(GameTimer* gameTimer)
*  @brief     Initialize scene
*  @param[in] GameTimer* gameTimer
*  @return 　　void
*****************************************************************************/
void SampleTexture::Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, GameTimer* gameTimer)
{
	Scene::Initialize(engine, gameTimer);
}
/****************************************************************************
*                       Update
*************************************************************************//**
*  @fn        void SampleEmpty::Update()
*  @brief     Update Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleTexture::Update()
{
	Scene::Update();

	std::vector<ui::Image> images;
	images.push_back(*_image.get());

	_renderer->AddFrameObject(images, _resourceView);

}
/****************************************************************************
*                       Draw
*************************************************************************//**
*  @fn        void SampleEmpty::Draw()
*  @brief     Draw Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleTexture::Draw()
{
	_engine->BeginDrawFrame();
	/*-------------------------------------------------------------------
	-             Regist graphics pipeline command
	---------------------------------------------------------------------*/
	const auto commandList = _engine->GetCommandList(CommandListType::Graphics, _engine->GetCurrentFrameIndex());
	commandList->SetViewportAndScissor(
		core::Viewport   (0, 0, Screen::GetScreenWidth(), Screen::GetScreenHeight()),
		core::ScissorRect(0, 0, Screen::GetScreenWidth(), Screen::GetScreenHeight()));

	_renderer->Draw();

	_engine->EndDrawFrame();
}
/****************************************************************************
*                       Terminate
*************************************************************************//**
*  @fn        void SampleEmpty::Terminate()
*  @brief     Terminate Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleTexture::Terminate()
{
	_image.reset();
	_resourceView.reset();
	_resourceCache.reset();
}
#pragma endregion Public Function

#pragma region Protected Function
/****************************************************************************
*                       LoadMaterials
*************************************************************************//**
*  @fn        void SampleEmpty::LoadMaterials(GameTimer* gameTimer)
*  @brief     Load Materials
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleTexture::LoadMaterials()
{
	/*-------------------------------------------------------------------
	-             Open Copy CommandList
	---------------------------------------------------------------------*/
	const auto copyCommandList     = _engine->GetCommandList(CommandListType::Copy, _engine->GetCurrentFrameIndex());
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics, _engine->GetCurrentFrameIndex());
	copyCommandList    ->BeginRecording();
	graphicsCommandList->BeginRecording();

	/*-------------------------------------------------------------------
	-             SetUp Resources
	---------------------------------------------------------------------*/
	// Create image sprite
	_image = std::make_shared<Image>();
	_image->CreateInNDCSpace();

	// Create Texture
	_resourceCache = std::make_shared<GPUResourceCache>(_engine->GetDevice(), graphicsCommandList);
	_resourceView  = _resourceCache->Load(L"Resources/BackGround.png");

	// Create UI Renderer
	_renderer = std::make_unique<ui::UIRenderer>(_engine);

	/*-------------------------------------------------------------------
	-             Close Copy CommandList and Flush CommandQueue
	---------------------------------------------------------------------*/
	graphicsCommandList->EndRecording();
	copyCommandList    ->EndRecording();

	_engine->FlushCommandQueue(CommandListType::Graphics);
	_engine->FlushCommandQueue(CommandListType::Copy);
}
/****************************************************************************
*                       OnKeyboardInput
*************************************************************************//**
*  @fn        void SampleEmpty::OnKeyboardInput()
*  @brief     KeyboardInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleTexture::OnKeyboardInput()
{

}
/****************************************************************************
*                       OnMouseInput
*************************************************************************//**
*  @fn        void SampleEmpty::OnMouseInput()
*  @brief     MouseInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleTexture::OnMouseInput()
{

}
/****************************************************************************
*                       OnGamePadInput
*************************************************************************//**
*  @fn        void SampleEmpty::OnGamePadInput()
*  @brief     GamePadInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleTexture::OnGamePadInput()
{

}
/****************************************************************************
*                     ExecuteSceneTransition
*************************************************************************//**
*  @fn        void SampleEmpty::ExecuteSceneTranstion()
*  @brief     Scene Transition
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleTexture::ExecuteSceneTransition()
{

}
#pragma endregion Protected Function