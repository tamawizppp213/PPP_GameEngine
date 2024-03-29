//////////////////////////////////////////////////////////////////////////////////
///             @file   GUPair.hpp
///             @brief  temp
///             @author toide
///             @date   2024/02/01 21:06:26
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_PAIR_HPP
#define GU_PAIR_HPP

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
	*				  			   GUPair
	*************************************************************************//**
	*  @class     GUPair
	*  @brief     �y�A
	*****************************************************************************/
	template<typename KeyType, typename ValueType>
	struct Pair
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		__forceinline bool operator == (const Pair& other) const { return Key == other.Key; }
		__forceinline bool operator != (const Pair& other) const { return Key != other.Key; }
		__forceinline bool operator <  (const Pair& other) const { return Key <  other.Key;}
		__forceinline bool operator >  (const Pair& other) const { return Key >  other.Key; }
		__forceinline bool operator() (const Pair& a, const Pair& b) const {return a.Key < B.Key; }

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		KeyType Key;

		ValueType Value;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Pair(const KeyType& key, const ValueType& value) : Key(key), Value(value) {};

		Pair(const KeyType& key) : Key(key), Value(ValueType()) {};

		Pair() : Key(KeyType()), Value(ValueType()) {};

		Pair(const Pair& other) = default;

		Pair& operator=(const Pair& other) noexcept
		{
			Key = other.Key; Value = other.Value;
			return *this;
		}

		Pair(Pair&& other) = default;
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