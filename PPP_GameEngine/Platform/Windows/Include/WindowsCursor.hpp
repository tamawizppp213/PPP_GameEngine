//////////////////////////////////////////////////////////////////////////////////
///             @file   GeneralWindow.hpp
///             @brief  ウィンドウを作成するクラス
///             @author Toide Yutaro
///             @date   2023_08_31
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef WINDOWS_CURSOR_HPP
#define WINDOWS_CURSOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformMacros.hpp"

#if PLATFORM_OS_WINDOWS
#include "../../Core/Include/CoreCursor.hpp"
#include <Windows.h>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace platform::windows
{
	/****************************************************************************
	*				  			    ICursor
	*************************************************************************//**
	*  @class     ICursor
	*  @brief     Cursor interface
	*****************************************************************************/
	class ICursor : public core::ICursor
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		virtual void Show() override;

		virtual void Hide() override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		virtual gm::Float2  GetPosition() const override;

		virtual core::CursorType  GetType() const override;

		virtual void SetPosition(const std::int32_t x, const std::int32_t y) const override;

		virtual void SetType(const core::CursorType type) override;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		explicit ICursor(const core::CursorType type);

		~ICursor();
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		HCURSOR _cursor = NULL;
	};
}
#endif
#endif