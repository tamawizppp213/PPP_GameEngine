//////////////////////////////////////////////////////////////////////////////////
///             @file   AudioManager.cpp
///             @brief  Audio Master Voice Management.
///             @author Toide Yutaro
///             @date   2021_01_03
//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Audio/Core/Include/AudioMaster.hpp"
#include <xaudio2.h>
#include <xaudio2fx.h>
#include <x3daudio.h>

#pragma comment(lib, "xaudio2.lib")

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gc::audio;

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor 
AudioMaster::AudioMaster()
{
	if (_hasInitialized)         { return; }
	if (!CreateXAudio2())        { return; }
	if (!CreateMasteringVoice()) { return; }
	if (!CreateX3DAudio())       { return; }
	if (!CreateVolumeMeter())    { return; }
	_hasInitialized = true;
}

AudioMaster::~AudioMaster()
{
	/*-------------------------------------------------------------------
	-              Destroy Master Voice
	---------------------------------------------------------------------*/
	if (_masteringVoice)
	{
		_masteringVoice->DestroyVoice();
		//_masteringVoice.reset();
	}

	/*-------------------------------------------------------------------
	-              Release XAudio2 Object
	---------------------------------------------------------------------*/
	if (_xAudio)
	{
		_xAudio->Release();
		_xAudio = nullptr;
	}

	/*-------------------------------------------------------------------
	-              Destroy COM Library
	---------------------------------------------------------------------*/
	CoUninitialize();
}
#pragma endregion Constructor and Destructor
#pragma region Public Function

/****************************************************************************
*                       GetAudioInterface
*************************************************************************//**
*  @fn        IXAudio2* AudioMaster::GetAudioInterface() const noexcept
* 
*  @brief     Get XAudio2 Interface Pointer
* 
*  @param[in] void
* 
*  @return    IXAudio2Ptr (Comptr<IXAudio2>)
*****************************************************************************/
AudioMaster::IXAudio2Ptr AudioMaster::GetAudioInterface() const noexcept
{
	_xAudio->AddRef();
	return _xAudio;
}

/****************************************************************************
*                       GetMasteringVoice
*************************************************************************//**
*  @fn        IXAudio2MasteringVoice* AudioMaster::GetMasteringVoice() const noexcept
* 
*  @brief     Get Mastering Voice
* 
*  @param[in] void
* 
*  @return 　　IXAudio2MasteringVoice*
*****************************************************************************/
AudioMaster::IXAudio2MasteringVoicePtr AudioMaster::GetMasteringVoice() const noexcept
{
	return _masteringVoice;
}

/****************************************************************************
*                       GetX3DAudioInterface
*************************************************************************//**
*  @fn        X3DAUDIO_HANDLE& AudioMaster::GetX3DAudioInterface() const
*  @brief     Get X3DAudio2 Interface
*  @param[in] void
*  @return 　　X3DAUDIO_HANDLE&
*****************************************************************************/
const AudioMaster::X3DAudioHandler& AudioMaster::GetX3DAudioInterface()
{
	return _x3dAudio;
}

bool AudioMaster::GetVolumeParamters(float* peakLevels, float* rmsParams, const UINT32 channel)
{
	XAUDIO2FX_VOLUMEMETER_LEVELS levels =
	{
		.pPeakLevels  = peakLevels,
		.pRMSLevels   = rmsParams,
		.ChannelCount = channel
	};

	if(FAILED(_masteringVoice->GetEffectParameters(0, &levels, sizeof(levels)))){return false;}
	
	return true;
}
#pragma endregion Public Function
#pragma region Protected Function
/****************************************************************************
*                       CreateXAudio2
*************************************************************************//**
*  @fn        bool AudioMaster::CreateXAudio2()
* 
*  @brief     Create XAudio2 Instance and Set debug mode
* 
*  @param[in] void
* 
*  @return    bool (true : success create XAudio2, false : failed to create XAudio2)
*****************************************************************************/
bool AudioMaster::CreateXAudio2()
{
	/*-------------------------------------------------------------------
	-              COM Initialize
	---------------------------------------------------------------------*/
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		OutputDebugStringA("can't initialize COM.");
		return false;
	}

	/*-------------------------------------------------------------------
	-              Check debug flag (Under Win7 version)
	---------------------------------------------------------------------*/
#if _WIN32_WINNT < _WIN32_WINNT_WIN8 && defined(_DEBUG)
	_debugFlag |= XAUDIO2_DEBUG_ENGINE;
#endif // _DEBUG

	/*-------------------------------------------------------------------
	-              Create XAudio2
	---------------------------------------------------------------------*/
	if (FAILED(XAudio2Create(AudioMaster::_xAudio.GetAddressOf(), _debugFlag)))
	{
		OutputDebugStringA("can't initialize COM.");
		CoUninitialize();
		return false;
	}
	
	/*-------------------------------------------------------------------
	-              Check debug flag (Over Win8 version)
	---------------------------------------------------------------------*/
#if (_WIN32_WINNT >= _WIN32_WINNT_WIN8)
	XAUDIO2_DEBUG_CONFIGURATION debug = {};
	debug.TraceMask       = XAUDIO2_LOG_ERRORS | XAUDIO2_LOG_WARNINGS;
	debug.BreakMask       = XAUDIO2_LOG_ERRORS;
	debug.LogFileline     = TRUE;
	debug.LogFunctionName = TRUE;
	debug.LogTiming       = TRUE;
	debug.LogThreadID     = TRUE;
	_xAudio->SetDebugConfiguration(&debug, nullptr);
#endif

	return true;
}

/****************************************************************************
*                       CreateMasterVoice
*************************************************************************//**
*  @fn        bool AudioMaster::CreateMasteringVoice()
* 
*  @brief     Create Mastering Voice for Initialize
* 
*  @param[in] void
* 
*  @return 　　bool (true : success create MasteringVoice, false : failed to create MasteringVoice)
*****************************************************************************/
bool AudioMaster::CreateMasteringVoice()
{
	if (FAILED(_xAudio->CreateMasteringVoice(&_masteringVoice))) // 今後各引数を導入する.
	{
		OutputDebugStringA("Failed to create Mastering Voice.");

		// Safe Release XAudio
		if (_xAudio)
		{
			_xAudio->Release();
			_xAudio = nullptr;
		}

		CoUninitialize();
		return false;
	}

	return true;
}

/****************************************************************************
*                       CreateX3DAudio
*************************************************************************//**
*  @fn        bool AudioMaster::CreateX3DAudio()
* 
*  @brief     Create X3DAudio for Initialize
* 
*  @param[in] void
* 
*  @return    bool
*****************************************************************************/
bool AudioMaster::CreateX3DAudio()
{
	if (FAILED(X3DAudioInitialize(SPEAKER_STEREO, X3DAUDIO_SPEED_OF_SOUND, _x3dAudio)))
	{
		OutputDebugStringA("can't initialize x3dAudio.");
		return false;
	}
	return true;
}

/****************************************************************************
*                       CreateVolumeMeter
*************************************************************************//**
*  @fn        bool AudioMaster::CreateVolumeMeter()
*
*  @brief     Create total volume logger
*
*  @param[in] void
*
*  @return    bool
*****************************************************************************/
bool AudioMaster::CreateVolumeMeter()
{
	IUnknown* volumeMeter;
	if (FAILED(XAudio2CreateVolumeMeter(&volumeMeter)))
	{
		OutputDebugStringA("Couldn't create volume meter");
		return false;
	}

	XAUDIO2_EFFECT_DESCRIPTOR effectDesc =
	{
		.pEffect        = volumeMeter,
		.InitialState   = true,
		.OutputChannels = 8 // なぜ8だと大丈夫なのか謎. 
	};
	
	XAUDIO2_EFFECT_CHAIN effectChain = 
	{
		.EffectCount = 1,
		.pEffectDescriptors = &effectDesc
	};

	/*-------------------------------------------------------------------
	-              Set effect chain to submix voice
	---------------------------------------------------------------------*/
	if (FAILED(_masteringVoice->SetEffectChain(&effectChain)))
	{
		OutputDebugStringA("Coundn't set effect chain");
		return false;
	};

	volumeMeter->Release();
	return true;
}
#pragma endregion Protected Function