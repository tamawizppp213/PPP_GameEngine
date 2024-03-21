//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdMatrix128SSE.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/20 15:36:48
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_MATRIX_128_SSE3_HPP
#define GM_SIMD_MATRIX_128_SSE3_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_SSE3 && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector128SSE3.hpp"
#include "GMSimdMatrix128SSE2.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////

namespace gm::simd::sse3
{
	/****************************************************************************
	*				  			   GMSimdMatrix128SSE
	*************************************************************************//**
	*  @class     GMSimdMatrix128SSE
	*  @brief     temp
	*****************************************************************************/
	class Matrix128Utility : public gm::simd::sse2::Matrix128Utility
	{
	private:
		using VectorFunction = Vector128Utility;

	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/


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