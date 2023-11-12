//////////////////////////////////////////////////////////////////////////////////
//              @file   Scene.cpp
///             @brief  Scene
///             @author Toide Yutaro
///             @date   2022_04_14
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/Scene.hpp"
#include "GameUtility/Base/Include/GUSharedPointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
Scene::Scene()
{

}
Scene::~Scene()
{

}
#pragma region Public Function
void Scene::Initialize(const gu::SharedPointer<PPPEngine>& engine,const std::shared_ptr<GameTimer>& gameTimer)
{
	_gameTimer = gameTimer;
	_pppEngine = engine;
	_engine    = engine->GetLowLevelGraphics();
	LoadMaterials();
}
void Scene::Update()
{
	OnKeyboardInput();
	OnMouseInput();
	OnGamePadInput();
}
#pragma endregion Public Function