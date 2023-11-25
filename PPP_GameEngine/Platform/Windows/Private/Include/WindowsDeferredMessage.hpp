//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_DEFERRED_MESSAGE_HPP
#define WINDOWS_DEFFERED_MESSAGE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include <Windows.h>
#include <cstdint>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace platform::windows
{
	/****************************************************************************
	*				  			    DefferedMessage
	*************************************************************************//**
	*  @class     DefferedMessage
	*  @brief     Windowsの関数を一気にまとめて表示するためのクラス
	*****************************************************************************/
	struct DeferredMessage
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		HWND WindowHandle = NULL;

		// message code
		std::uint32_t MessageCode = 0;

		// Message data
		WPARAM WParam = NULL;
		LPARAM LParam = NULL;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		DeferredMessage(HWND hwnd, const std::uint32_t messageCode, const WPARAM wParam, const LPARAM lParam)
			: WindowHandle(hwnd), MessageCode(messageCode), WParam(wParam), LParam(lParam)
		{

		}
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/

	};
}
#endif
#endif