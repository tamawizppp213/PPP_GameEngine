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
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Public Function
void Scene::Initialize(GameTimer* gameTimer)
{
	_gameTimer = gameTimer;
	LoadMaterials(gameTimer);
}
void Scene::Update()
{
	OnKeyboardInput();
	OnMouseInput();
	OnGamePadInput();
}
#pragma endregion Public Function