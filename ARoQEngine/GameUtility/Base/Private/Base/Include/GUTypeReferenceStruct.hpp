//////////////////////////////////////////////////////////////////////////////////
///             @file   GUTypeReferenceStruct.hpp
///             @brief  RemoveReferenceなどを定義しています.
///             @author toide
///             @date   2024/02/17 12:18:33
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_TYPE_REFERENCE_STRUCT_HPP
#define GU_TYPE_REFERENCE_STRUCT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu::details::type_traits
{
	/*---------------------------------------------------------------
					T型に参照がついていた場合, 参照部分を取り消す
	-----------------------------------------------------------------*/
	template<class T>
	struct RemoveReferenceType { using Type = T; };

	template<class T>
	struct RemoveReferenceType<T&> { using Type = T; };

	template<class T>
	struct RemoveReferenceType<T&&> { using Type = T; };
}

#endif