//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  �E�B���h�E���쐬����N���X
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_PLATFORM_APPLICATION_HPP
#define WINDOWS_PLATFORM_APPLICATION_HPP

#ifdef _WIN32
//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformApplication.hpp"
#include "../Private/Include/WindowsDeferredMessage.hpp"
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	class CoreWindow;
	/****************************************************************************
	*				  			    PlatformApplication
	*************************************************************************//**
	*  @class     PlatformApplication
	*  @brief     �E�B���h�E���Ǘ���, ���b�Z�[�W���[�v���Ǘ�����N���X
	*****************************************************************************/
	class PlatformApplication : public platform::core::PlatformApplication
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : �V�K�̃E�B���h�E�C���X�^���X���쐬���܂�. �����ł̓Z�b�g�A�b�v�͍s���܂���*/
		std::shared_ptr<core::CoreWindow> MakeWindow() override;

		/* @brief : �w��̃E�B���h�E�����ۂɍ쐬����܂ōs���܂�. �Z�b�g�A�b�v���s���܂�*/
		void SetUpWindow(const std::shared_ptr<core::CoreWindow>& window, const core::CoreWindowDesc& desc) override;

		/* @brief : ���b�Z�[�W���o���܂�. ���̊֐��͉��z�L�[���b�Z�[�W���󂯎���, ��������b�Z�[�W�`���ɕϊ���������true��Ԃ��܂�*/
		bool PumpMessage() override;

		/* @brief : �A�v���P�[�V�������I�������������m���܂�.*/
		bool IsQuit() const override { return _isApplicationQuited; };
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		HINSTANCE GetWindowsInstanceHandle() const noexcept { return _instanceHandle; }

		void* GetInstanceHandle() const noexcept override { return _instanceHandle; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		PlatformApplication();

		~PlatformApplication();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		/* @brief : Event driven windows function*/
		static LRESULT CALLBACK StaticWindowProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

		virtual LRESULT ApplicationWindowMessageProcedure(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		HINSTANCE _instanceHandle = NULL;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		bool RegisterWindowClass();

		LRESULT ProcessDeferredWindowsMessage(const DeferredMessage& message);
		
		void ProcessDeferredEvents();
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
		std::vector<std::shared_ptr<windows::CoreWindow>> _windows = {};

		std::vector<DeferredMessage> _messageList = {};

		MSG _windowMessage = { NULL };

		// @brief : ���C�����[�v�̊O�Ŏ��s����Ƃ���true�ɂȂ�܂�
		bool _allowedToDeferredMessageProcessing = false;

		//@brief : �A�v���P�[�V�������I�������Ƃ���true�ɂȂ�
		bool _isApplicationQuited = false;
	};
}
#endif
#endif