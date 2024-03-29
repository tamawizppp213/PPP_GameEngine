//////////////////////////////////////////////////////////////////////////////////
///             @file   SampleSky.hpp
///             @brief  Skybox sample
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef SAMPLE_AUDIO_HPP
#define SAMPLE_AUDIO_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "MainGame/Core/Include/Scene.hpp"
#include <memory>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gc::audio
{
	class AudioSource;
	class AudioSubmix;
	class AudioMaster;
}
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace sample
{
	enum AudioMode
	{
		None,
		VolumeSelect,
		PitchSelect,
		PanSelect,
		CountOf
	};
	/****************************************************************************
	*				  			SampleEmpty
	*************************************************************************//**
	*  @class     SampleEmpty
	*  @brief     Empty sample
	*****************************************************************************/
	class SampleAudio : public Scene
	{
		using AudioSourcePtr = std::shared_ptr<gc::audio::AudioSource>;
		using AudioSubmixPtr = std::shared_ptr<gc::audio::AudioSubmix>;
		using AudioMasterPtr = std::shared_ptr<gc::audio::AudioMaster>;
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
		SampleAudio();
		~SampleAudio();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void LoadMaterials() override;
		void OnKeyboardInput() override;
		void OnMouseInput  () override;
		void OnGamePadInput() override;
		void ExecuteSceneTransition() override;

		void ShowVolumeLog();
		void Act(const float adjustValue);
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		AudioMode _mode = AudioMode::None;
		AudioSourcePtr _audioSource = nullptr;
		AudioSubmixPtr _audioSubmix = nullptr;
		AudioMasterPtr _audioMaster = nullptr;
		float _pan = 0.5f;
		bool _isOn = true;
		bool _useReverb = true;
	};
}
#endif