//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector128Neon.hpp
///             @brief  temp
///             @author toide
///             @date   2024/03/11 2:27:35
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_128_NEON_HPP
#define GM_SIMD_VECTOR_128_NEON_HPP


//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_NEON && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)

#if defined(_MSC_VER) && !defined(__clang__) && (defined(_M_ARM64) || defined(_M_HYBRID_X86_ARM64) || defined(_M_ARM64EC))
	#include <arm64_neon.h>
#else
	#include <arm_neon.h>
#endif

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm::simd::neon
{
	/****************************************************************************
	*				  			   GMSimdVector128Neon
	*************************************************************************//**
	*  @class     GMSimdVector128Neon
	*  @brief     temp
	*****************************************************************************/
	class Vector128Utility
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