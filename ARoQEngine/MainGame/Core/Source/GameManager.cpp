//////////////////////////////////////////////////////////////////////////////////
//              @file   GameManager.cpp
///             @brief  game manager class
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/GameManager.hpp"
#include "GameCore/Core/Include/GameObject.hpp"
#include "GameCore/Core/Include/ResourceManager.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;
using namespace gc::core;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
void GameManager::GameStart(const engine::setting::StartUpParameters& parameters)
{
	_engine = gu::MakeShared<PPPEngine>();
	_engine->StartUp(parameters);
	_sceneManager.StartUp(_engine, _engine->GetMainThreadTimer());
	_engine->Run();
}

void GameManager::GameUpdateMain()
{
	_sceneManager.CallSceneUpdate();
}

void GameManager::GameDrawMain()
{
	_sceneManager.CallSceneDraw();
}

void GameManager::GameEnd()
{
	_sceneManager.CallSceneTerminate();
	_sceneManager.ShutDown();
	GameObject::ClearAllGameObjects();
	_engine->ShutDown();
}
