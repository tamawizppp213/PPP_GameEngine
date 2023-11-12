//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
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
	*  @brief     ウィンドウを管理し, メッセージループを管理するクラス
	*****************************************************************************/
	class PlatformApplication : public platform::core::PlatformApplication, public gu::EnableSharedFromThis<PlatformApplication>
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : 新規のウィンドウインスタンスを作成します. ここではセットアップは行いません*/
		gu::SharedPointer<core::CoreWindow> MakeWindow() override;

		/* @brief : 新規でコマンドをまとめたクラスのインスタンスを作成します.*/
		gu::SharedPointer<core::PlatformCommand> MakeCommand() override;

		/* @brief : 指定のウィンドウを実際に作成するまで行います. セットアップも行います*/
		void SetUpWindow(const gu::SharedPointer<core::CoreWindow>& window, const core::CoreWindowDesc& desc) override;

		/* @brief : メッセージを出します. この関数は仮想キーメッセージが受け取られ, それをメッセージ形式に変換した時にtrueを返します*/
		bool PumpMessage() override;

		/* @brief : アプリケーションが終了したかを検知します.*/
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
		std::vector<gu::SharedPointer<windows::CoreWindow>> _windows = {};

		std::vector<DeferredMessage> _messageList = {};

		MSG _windowMessage = { NULL };

		// @brief : メインループの外で実行するときにtrueになります
		bool _allowedToDeferredMessageProcessing = false;

		//@brief : アプリケーションが終了したときにtrueになる
		bool _isApplicationQuited = false;
	};
}
#endif
#endif