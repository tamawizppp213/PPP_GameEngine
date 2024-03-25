//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  �E�B���h�E���쐬����N���X
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CORE_COMMON_STATE_HPP
#define CORE_COMMON_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUType.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::core
{
	/****************************************************************************
	*				  			    PlatformType
	*************************************************************************//**
	*  @class     PlatformType
	*  @brief     �v���b�g�t�H�[���̎�ނ��`���Ă��܂�
	*****************************************************************************/
	enum class PlatformType
	{
		Windows,

	};

#pragma region Mouse
	/****************************************************************************
	*				  			    CursorType
	*************************************************************************//**
	*  @class     CursorType
	*  @brief     cursor look 
	*****************************************************************************/
	enum class CursorType
	{
		None,            // Causes no mouse cursor to be visible.
		Default,         // arrow cursor
		TextEditBeam,    // text edit beam
		ResizeLeftRight, // resize horizontal
		ResizeUpDown,    // resize vertical
		ResizeSouthEast, // resize diagonal
		ResizeSouthWest, // resize other diagonal
		CardinalCross,   // move item
		Crosshairs,      // target cross
		Hand,            // hand cursor
		Prohibit,        // prohibit
		Original,        // your original cursor
		CountOf,
	};

	/****************************************************************************
	*				  			  ActivationType
	*************************************************************************//**
	*  @class     ActivationType
	*  @brief     �E�B���h�E�̋N����, ��N�����̏u�ԂɌĂ΂��enum class�ł�.
	*****************************************************************************/
	enum class ActivationType
	{
		InActive,      // ��A�N�e�B�u
		Activate,      // �}�E�X�N���b�N�ȊO�̃��\�b�h�ɂ���ăA�N�e�B�u��
		ClickActivate, // �}�E�X�N���b�N�ŃA�N�e�B�u��
	};

#pragma endregion Mouse

#pragma region Window
	/****************************************************************************
	*				  			  ActivationPolicy
	*************************************************************************//**
	*  @class     ActivationPolicy
	*  @brief     �E�B���h�E�̕\�����̃A�N�e�B�u���̕��j�����߂���
	*****************************************************************************/
	enum class ActivationPolicy
	{
		Never,        // ������Active�����Ȃ�
		Always,       // �\�������Ƃ��ɕK��window��Active�ɂ���
		FirstOnlyShow // �ŏ��̈�񂾂�
	};

	/****************************************************************************
	*				  			    WindowMode
	*************************************************************************//**
	*  @class     WindowMode
	*  @brief     window mode
	*****************************************************************************/
	enum class WindowMode
	{
		FullScreen,
		BorderLess,
		Windowed,
		CountOf
	};

	/****************************************************************************
	*				  	   EngineWindowType
	*************************************************************************//**
	*  @class     EngineWindowType
	*  @brief     engine�Ŏg�p����window�̎��
	*****************************************************************************/
	enum class EngineWindowType
	{
		Normal,
		Menu,
		Game,
	};

	/****************************************************************************
	*				  			    WindowMode
	*************************************************************************//**
	*  @class     WindowMode
	*  @brief     window mode
	*****************************************************************************/
	enum class WindowTransparency
	{
		None,      // ���ߓx���T�|�[�g���Ȃ�
		PerWindow, // �E�B���h�E�E���x���œ��ߓx���T�|�[�g���Ă��邱�Ƃ������l�i1�̕s�����x���E�B���h�E�S�̂ɓK�p�����
		PerPixel,  // �s�N�Z���P�ʂ̃A���t�@�������߂��T�|�[�g���Ă��邱�Ƃ������l�B
	};

	/****************************************************************************
	*				  			  CoreWindowDesc
	*************************************************************************//**
	*  @struct     CoreWindowDesc
	*  @brief     �E�B���h�E�p�̃f�B�X�N���v�^�ł�
	*****************************************************************************/
	struct CoreWindowDesc
	{
		float DesiredScreenPositionX  = 0.0f;  // �ŏ��ɖ]�܂����Ƃ����X�N���[���̉��̈ʒu
		float DesiredScreenPositionY  = 0.0f;  // �ŏ��ɖ]�܂����Ƃ����X�N���[���̏c�̈ʒu
		float DesiredScreenWidth      = 0.0f;  // �ŏ��ɖ]�܂����Ƃ���镝
		float DesiredScreenHeight     = 0.0f;  // �ŏ��ɖ]�܂����Ƃ���鍂��
		float ExpectedMaxWidth        = -1.0f; // �E�B���h�E�̗\�z�ő啝 (-1�͖���)
		float ExpectedMaxHeight       = -1.0f; // �E�B���h�E�̗\�z�ő啝 (-1�͖���)
		int   CornerRadius            = 0;     // �p�̊ۂ�
		float Opacity                 = 1.0f;
		wchar_t* Title                = L"PPP Game Window";
		bool  HasWindowOSWindowBorder = true; // OS�̃E�B���h�E�̋��E�����g�p�������ꍇ��true
		bool  IsVanillaWindow         = true;
		bool  SupportMaximize         = true;  // �E�B���h�E�̍ő剻�����邩  
		bool  SupportMinimize         = true;  // �E�B���h�E�̍ŏ��������邩
		bool  SupportResize           = true;  // �E�B���h�E�̃��T�C�Y���\�ɂ��邩
		bool  AppearsInTaskbar        = false; // �^�X�N�o�[�ɃE�B���h�E��\�����邩
		bool  IsTopmostWindow         = false; // �������őO�ʂɃE�B���h�E��\���������ꍇ�Ɏg�p���܂�.
		bool  AcceptInput             = true;  // interactive�ɂ��邩
		bool  UseTouchFeedback        = false; // ui�{�^�����̃^�b�`���͂��\�ɂ��邩
		bool  ShouldPreserveAspectRatio = false; // �A�X�y�N�g������ɂ��邩
		bool  SizeWillChangeOften       = false; // �p�ɂɃT�C�Y���������邩
		bool  HasCloseButton            = true;  // ����{�^����K�v�Ƃ��邩
		ActivationPolicy ActivationPolicy = ActivationPolicy::FirstOnlyShow;
		WindowMode         WindowMode       = WindowMode::Windowed;
		WindowTransparency TransparencyType = WindowTransparency::PerWindow;
		EngineWindowType  EngineWindowType = EngineWindowType::Normal;
	};
	
	struct Rectangle
	{
		long Left   = 0;
		long Top    = 0;
		long Right  = 0;
		long Bottom = 0;
	};

	/****************************************************************************
	*				  			 MonitorInfo
	*************************************************************************//**
	*  @struct    MonitorInfo
	*  @brief     �g�p���Ă��郂�j�^�[�̏����擾���܂�
	*****************************************************************************/
	struct MonitorInfo
	{
		gu::tchar* Name             = nullptr;  // ���j�^�[�̖��O
		gu::tchar* ID               = nullptr;  // ID�̖��O
		gu::int32  NativeWidth      = 0;        // width
		gu::int32  NativeHeight     = 0;        // height
		gu::int32  DPI              = 0;        // Dots per inch (1�C���`������Ƀh�b�g�������邩)
		Rectangle  DisplayRectangle = {};       // ���j�^�[�S�̂�Rectangle
		Rectangle  WorkArea         = {};       // �^�X�N�o�[�Ƃ����܂܂Ȃ���Ɨ̈掩�̂�Rectangle
		bool       IsPrimary        = false;    // �v���C�}���[���j�^�[��
	};

#pragma endregion Window
}

#endif