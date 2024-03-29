//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioClip.hpp
///             @brief  Audio Clip for audio play (.wav data)
///             @author Toide Yutaro
///             @date   2021_03_12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef AUDIO_CLIP_HPP
#define AUDIO_CLIP_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include <string>
#include <memory>
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         AudioClip
//////////////////////////////////////////////////////////////////////////////////

namespace gc::audio
{

	/****************************************************************************
	*				  		AudioClip
	*************************************************************************//**
	*  @class     Audio Clip
	*  @brief     Save audio data (current .wav only)
	*****************************************************************************/
	class AudioClip : public gu::NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool Load(const std::wstring& filePath);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		const std::wstring& GetFilePath()      const;
		const size_t        GetSoundSize()     const;
		const WAVEFORMATEX& GetFileFormatEx()  const;
		std::shared_ptr<BYTE[]> GetSoundData() const noexcept;
		const size_t        GetSamplingFrequency() const { return _waveFormatEx.nSamplesPerSec; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		AudioClip();
		~AudioClip();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::wstring            _filePath = L"";
		size_t                  _soundSize = 0;
		WAVEFORMATEX            _waveFormatEx;
		std::shared_ptr<BYTE[]> _soundData = nullptr;

	};
}
#endif