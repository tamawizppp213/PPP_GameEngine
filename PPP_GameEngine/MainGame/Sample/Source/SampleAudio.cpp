//////////////////////////////////////////////////////////////////////////////////
//              @file   SampleSky.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Sample/Include/SampleAudio.hpp"
#include "GameCore/Input/Include/GameInput.hpp"
#include "GameCore/Audio/Core/Include/AudioSource.hpp"
#include "GameCore/Audio/Core/Include/AudioClip.hpp"
#include "GameCore/Audio/Core/Include/AudioMaster.hpp"
#include <iostream>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace sample;
using namespace rhi;
using namespace rhi::core;
using namespace gc::audio;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
SampleAudio::SampleAudio()
{

}
SampleAudio::~SampleAudio()
{

}
#pragma region Public Function
/****************************************************************************
*                       Initialize
*************************************************************************//**
*  @fn        void SampleEmpty::Initialize( const GameTimerPtr& gameTimer)
*  @brief     Initialize scene
*  @param[in] const GameTimerPtr& gameTimer
*  @return 　　void
*****************************************************************************/
void SampleAudio::Initialize(const std::shared_ptr<LowLevelGraphicsEngine>& engine, const GameTimerPtr& gameTimer)
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
void SampleAudio::Update()
{
	Scene::Update();
}
/****************************************************************************
*                       Draw
*************************************************************************//**
*  @fn        void SampleEmpty::Draw()
*  @brief     Draw Scene
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleAudio::Draw()
{
	_engine->BeginDrawFrame();

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
void SampleAudio::Terminate()
{
	_audioSource->Stop();
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
void SampleAudio::LoadMaterials()
{
	/*-------------------------------------------------------------------
	-             Open Copy CommandList
	---------------------------------------------------------------------*/
	const auto copyCommandList     = _engine->GetCommandList(CommandListType::Copy, _engine->GetCurrentFrameIndex());
	const auto graphicsCommandList = _engine->GetCommandList(CommandListType::Graphics, _engine->GetCurrentFrameIndex());
	copyCommandList    ->BeginRecording();
	graphicsCommandList->BeginRecording();


	/*-------------------------------------------------------------------
	-             Close Copy CommandList and Flush CommandQueue
	---------------------------------------------------------------------*/
	graphicsCommandList->EndRecording();
	copyCommandList->EndRecording();

	_engine->FlushCommandQueue(CommandListType::Graphics);
	_engine->FlushCommandQueue(CommandListType::Copy);


	/*-------------------------------------------------------------------
	-             Sound resources
	---------------------------------------------------------------------*/
	_audioMaster = std::make_shared<AudioMaster>();

	const auto audioClip = std::make_shared<AudioClip>();
	audioClip->Load(L"Resources\\Sound\\digitalworld.wav");

	_audioSource = std::make_shared<AudioSource>(_audioMaster, audioClip, SoundType::BGM, 1.0f);
	_audioSource->Play();
}
/****************************************************************************
*                       OnKeyboardInput
*************************************************************************//**
*  @fn        void SampleEmpty::OnKeyboardInput()
*  @brief     KeyboardInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleAudio::OnKeyboardInput()
{
	Keyboard keyboard = _gameInput.GetKeyboard();
	
	/*-------------------------------------------------------------------
	-             Press check
	---------------------------------------------------------------------*/
	if (keyboard.IsTrigger(DIK_P))
	{
		_mode = (AudioMode)((_mode + 1) % AudioMode::CountOf);
		std::cout << _mode << std::endl;
	}
	if (keyboard.IsTrigger(DIK_O))
	{
		_isOn = _isOn ? false : true;
		
		if (_isOn) 
		{ 
			_audioSource->Replay(); 
		}
		else 
		{
			_audioSource->Pause(); 
		}
	}
	
	/*-------------------------------------------------------------------
	-             Adjust value
	---------------------------------------------------------------------*/
	float adjustValue = 0;
	if (keyboard.IsPress(DIK_RIGHTARROW))
	{
		adjustValue += 0.1f * _gameTimer->DeltaTime();
	}
	if (keyboard.IsPress(DIK_LEFTARROW))
	{
		adjustValue -= 0.1f * _gameTimer->DeltaTime();
	}

	/*-------------------------------------------------------------------
	-             Act 
	---------------------------------------------------------------------*/
	Act(adjustValue);
}
/****************************************************************************
*                       OnMouseInput
*************************************************************************//**
*  @fn        void SampleEmpty::OnMouseInput()
*  @brief     MouseInput
*  @param[in] void
*  @return 　　void
*****************************************************************************/
void SampleAudio::OnMouseInput()
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
void SampleAudio::OnGamePadInput()
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
void SampleAudio::ExecuteSceneTransition()
{

}

void SampleAudio::Act(const float adjustValue)
{
	if (adjustValue == 0.0f) { return; }

	switch (_mode)
	{
		using enum AudioMode;
		case VolumeSelect:
		{
			const auto value = _audioSource->GetVolume() + adjustValue;
			_audioSource->SetVolume(value);
			std::cout << "Volume: " << _audioSource->GetVolume() << std::endl;
			break;
		}
		case PitchSelect:
		{
			_audioSource->SetPitch(_audioSource->GetPitch() + adjustValue);
			std::cout << "Pitch: " << _audioSource->GetPitch() << std::endl;
			break;
		}
		case PanSelect:
		{
			_pan += adjustValue;
			_audioSource->SetPan(_pan);
			std::cout << "Pan: " << _pan << std::endl;
			break;
		}
		default:
		{
			break;
		}
	}
}
#pragma endregion Protected Function