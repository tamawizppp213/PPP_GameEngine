//////////////////////////////////////////////////////////////////////////////////
///             @file   GUClassUtility.hpp
///             @brief  クラスのコピー可能, 不可能を決定するクラスです. 
///                     使い方としては, コピーの可否などを決定したい基底クラスに継承する形で使用をお願いします. 
///                     
///                     NonCopyable    : コピー不可でムーブ可能
///                     Copyable       : コピーもムーブも可能
///                     NonCopyAndMove : コピーもムーブも不可能
///                      
///                     注意点として, shared_from_thisを使用する場合は特別にshared_from_thisを直接
///                     呼び出したい派生クラスに継承する形で実装してください. 
/// 
///             @author Toide Yutaro
///             @date   2022_06_21
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_CLASS_UTILITY_HPP
#define GU_CLASS_UTILITY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			NonCopyable
	*************************************************************************//**
	/*  @class     NonCopyable
	*   @brief     コピー不可でムーブ可能なクラスです.
	*****************************************************************************/
	class NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/


		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		NonCopyable() = default;
		virtual ~NonCopyable() = default;
		NonCopyable(const NonCopyable&) = delete;
		NonCopyable& operator=(const NonCopyable&) = delete;
		NonCopyable(NonCopyable&&)        noexcept = default;
		NonCopyable& operator=(NonCopyable&&) noexcept = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};

	/****************************************************************************
	*				  			  Copyable
	*************************************************************************//**
	/*  @class     Copyable
	*   @brief     コピーもムーブも可能なクラスです
	*****************************************************************************/
	class Copyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Copyable() = default;
		virtual ~Copyable() = default;
		Copyable(const Copyable&) = default;
		Copyable& operator=(const Copyable&) = default;
		Copyable(Copyable&&)        noexcept = default;
		Copyable& operator=(Copyable&&) = default;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};

	/****************************************************************************
	*				  			NonCopyAndMove
	*************************************************************************//**
	/*  @class     NonCopyAndMove
	*   @brief     コピーもムーブも不可能なクラスです
	*****************************************************************************/
	class NonCopyAndMove
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		NonCopyAndMove() = default;
		virtual ~NonCopyAndMove() = default;
		NonCopyAndMove(const NonCopyAndMove&) = delete;
		NonCopyAndMove& operator=(const NonCopyAndMove&) = delete;
		NonCopyAndMove(NonCopyAndMove&&)        noexcept = delete;
		NonCopyAndMove& operator=(NonCopyAndMove&&) = delete;
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