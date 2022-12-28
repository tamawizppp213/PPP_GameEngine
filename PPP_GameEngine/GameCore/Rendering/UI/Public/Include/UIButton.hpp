//////////////////////////////////////////////////////////////////////////////////
///             @file   UIButton.hpp
///             @brief  Button object (string, number)
///             @author Toide Yutaro
///             @date   2022_12_27
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef UIBUTTON_HPP
#define UIBUTTON_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "UIImage.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::ui
{
	
	/****************************************************************************
	*				  			    Text
	*************************************************************************//**
	*  @class     Text
	*  @brief     a
	*****************************************************************************/
	class Button : public Image
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : cursolPosition : Screen space*/
		virtual bool OnClick(const int cursolX, const int cursolY) const;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		bool GetIsInteractive() const { return _isInteractive; };

		void SetIsInteractive(const bool isInteractive) { _isInteractive = isInteractive; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Button();

		~Button();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		bool _isInteractive = false;

	};
}
#endif