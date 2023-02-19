//////////////////////////////////////////////////////////////////////////////////
///             @file   GameWorldInfo.hpp
///             @brief  Game object information to exist in the game world. (World position etc...)
///             @author Toide Yutaro
///             @date   2023_02_03
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GameWorldInfo.hpp"
#include "GraphicsCore/Engine/Include/LowLevelGraphicsEngine.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gm;
using namespace gc::core;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//							Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
GameWorldInfo::GameWorldInfo(const LowLevelGraphicsEnginePtr& engine, const std::uint64_t maxInstanceCount)
	: _engine(engine), _maxInstanceCount(maxInstanceCount)
{
#ifdef _DEBUG
	assert(engine);
	assert(maxInstanceCount > 0);
#endif

	const auto device = _engine->GetDevice();

	/*-------------------------------------------------------------------
	-            Prepare constant and upload buffer
	---------------------------------------------------------------------*/
	const GameWorldConstant world = { .World = gm::MatrixIdentityF() };
	const auto bufferInfo = GPUBufferMetaData::ConstantBuffer(sizeof(GameWorldConstant), maxInstanceCount, MemoryHeap::Upload, ResourceState::Common, (void*)&world);
	
	_gameWorldConstants = device->CreateBuffer(bufferInfo);
	_resourceView       = device->CreateResourceView(ResourceViewType::ConstantBuffer, _gameWorldConstants);

}

GameWorldInfo::~GameWorldInfo()
{
	_resourceView.reset();
	_gameWorldConstants.reset();
	_engine.reset();
}

#pragma endregion Constructor and Destructor

#pragma region Main Function
void GameWorldInfo::Bind(const GraphicsCommandListPtr& commandList, const std::uint32_t index)
{
	_resourceView->Bind(commandList, index);
}
#pragma endregion Main Function