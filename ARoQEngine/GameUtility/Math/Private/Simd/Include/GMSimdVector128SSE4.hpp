//////////////////////////////////////////////////////////////////////////////////
///             @file   GMSimdVector128SSE4.hpp
///             @brief   SSE4を使ったSimd演算
///             @author toide
///             @date   2024/03/11 2:27:05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GM_SIMD_VECTOR_128_SSE4_HPP
#define GM_SIMD_VECTOR_128_SSE4_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GMSimdMacros.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#if PLATFORM_CPU_INSTRUCTION_SSE4_1 && !defined(PLATFORM_CPU_INSTRUCTION_NOT_USE)
#include "GMSimdVector128SSE3.hpp"

#include <smmintrin.h>
#if PLATFORM_CPU_INSTRUCTION_SSE4_2
#include <nmmintrin.h>
#endif

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gm::simd::sse
{
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<4, 1, 2, 3>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0x1); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<0, 5, 2, 3>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0x2); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<4, 5, 2, 3>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0x3); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<0, 1, 6, 3>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0x4); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<4, 1, 6, 3>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0x5); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<0, 5, 6, 3>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0x6); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<4, 5, 6, 3>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0x7); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<0, 1, 2, 7>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0x8); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<4, 1, 2, 7>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0x9); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<0, 5, 2, 7>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0xA); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<4, 5, 2, 7>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0xB); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<0, 1, 6, 7>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0xC); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<4, 1, 6, 7>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0xD); }
	template<> inline Vector128 SIMD_CALL_CONVENTION VectorPermute<0, 5, 6, 7>(Vector128Utility::ConstVector128 V1, Vector128Utility::ConstVector128 V2) noexcept { return _mm_blend_ps(V1, V2, 0xE); }
}
namespace gm::simd::sse4
{
	/****************************************************************************
	*				  			   GVector128Utility
	*************************************************************************//**
	*  @class     Vector128Utility
	*  @brief     SSE4を使ったSimd演算
	*****************************************************************************/
	class Vector128Utility : public gm::simd::sse3::Vector128Utility
	{ 
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
	#pragma region Setter
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION SetY(ConstVector128 vector, const float y) noexcept;
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION SetZ(ConstVector128 vector, const float z) noexcept;
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION SetW(ConstVector128 vector, const float w) noexcept;

		__forceinline static void SIMD_CALL_CONVENTION StoreFloat3(float* destination, ConstVector128 source) noexcept;
	#pragma endregion Setter

	#pragma region Getter
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION LoadFloat3(const float* source) noexcept;
	#pragma endregion Getter

	#pragma region Math
		/*----------------------------------------------------------------------
		*  @brief : 2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION LengthVector2(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthVector3(ConstVector128 vector) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION LengthVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 内積を算出 (ただし, 返り値はfloatではなくVector128型で返されますので, 後々GetXなどで対応してください.)
		/*----------------------------------------------------------------------*/
		__forceinline static float SIMD_CALL_CONVENTION DotVector2(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION DotVector3(ConstVector128 left, ConstVector128 right) noexcept;
		__forceinline static float SIMD_CALL_CONVENTION DotVector4(ConstVector128 left, ConstVector128 right) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 入力ベクトルの単位ベクトルを返します
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION NormalizeVector2(ConstVector128 vector) noexcept;
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION NormalizeVector3(ConstVector128 vector) noexcept;
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION NormalizeVector4(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 最も近い整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION Round(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION Truncate(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 負の方向に丸める
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION Floor(ConstVector128 vector) noexcept;

		/*----------------------------------------------------------------------
		*  @brief : 切り上げ 数直線上で正の方向にある次の整数に丸める
		/*----------------------------------------------------------------------*/
		__forceinline static sse::Vector128 SIMD_CALL_CONVENTION Ceiling(ConstVector128 vector) noexcept;

	#pragma endregion Math
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

#pragma region Implement
	#pragma region Setter
	/****************************************************************************
	*                       SetY
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION SetY(ConstVector128 vector, const float y) noexcept
	*
	*  @brief     Floatを使ってVectorのYに値を代入する
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float y
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetY(ConstVector128 vector, const float y) noexcept
	{
		return _mm_insert_ps(vector, _mm_set_ss(y), 0x10);
	}

	/****************************************************************************
	*                       SetZ
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION SetZ(ConstVector128 vector, const float z) noexcept
	*
	*  @brief     Floatを使ってVectorのZに値を代入する
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float z
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetZ(ConstVector128 vector, const float z) noexcept
	{
		return _mm_insert_ps(vector, _mm_set_ss(z), 0x20);
	}

	/****************************************************************************
	*                       SetW
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION SetW(ConstVector128 vector, const float w) noexcept
	*
	*  @brief     Floatを使ってVectorのWに値を代入する
	*
	*  @param[in] ConstVector128 vector
	*  @param[in] const float w
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::SetW(ConstVector128 vector, const float w) noexcept
	{
		return _mm_insert_ps(vector, _mm_set_ss(w), 0x30);
	}

	/****************************************************************************
	*                       StoreFloat3
	*************************************************************************//**
	*  @fn        inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat3(float* destination, ConstVector128 source) noexcept
	*
	*  @brief     Vector128をもとにFloat配列に代入します
	*
	*  @param[in] float* destination
	*  @param[in] ConstVector128 source
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline void SIMD_CALL_CONVENTION Vector128Utility::StoreFloat3(float* destination, ConstVector128 source) noexcept
	{
		*reinterpret_cast<int*>(&destination[0]) = _mm_extract_ps(source, 0);
		*reinterpret_cast<int*>(&destination[1]) = _mm_extract_ps(source, 1);
		*reinterpret_cast<int*>(&destination[2]) = _mm_extract_ps(source, 2);
	}


	#pragma endregion Setter

	#pragma region Getter
	/****************************************************************************
	*                       LoadFloat3
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat3(const float* source) noexcept
	*
	*  @brief     Floatの配列を使って格納する (source->x, source->y, source->z, source->w)
	*
	*  @param[in] const float* source
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::LoadFloat3(const float* source) noexcept
	{
		__m128 xy = _mm_castpd_ps(_mm_load_sd(reinterpret_cast<const double*>(source)));
		__m128 z = _mm_load_ss(&source[2]);
		return _mm_insert_ps(xy, z, 0x20);
	}

	#pragma endregion Getter
	#pragma region Math
	/****************************************************************************
	*                      LengthVector2
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector2(ConstVector128 vector) noexcept
	*
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector2(ConstVector128 vector) noexcept
	{
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(vector, vector, 0x3f)));
	}

	/****************************************************************************
	*                      LengthVector3
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector3(ConstVector128 vector) noexcept
	*
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector3(ConstVector128 vector) noexcept
	{
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(vector, vector, 0x7f)));
	}

	/****************************************************************************
	*                      LengthVector4
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector4(ConstVector128 vector) noexcept
	*
	*  @brief     2次ノルム(各要素の2乗和をルートを取ったときのベクトルの大きさ)を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::LengthVector4(ConstVector128 vector) noexcept
	{
		return _mm_cvtss_f32(_mm_sqrt_ps(_mm_dp_ps(vector, vector, 0xff)));
	}

	/****************************************************************************
	*                      DotVector2
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector2(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector2(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cvtss_f32(_mm_dp_ps(left, right, 0x3f));
	}

	/****************************************************************************
	*                      DotVector3
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector3(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector3(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cvtss_f32(_mm_dp_ps(left, right, 0x7f));
	}

	/****************************************************************************
	*                      DotVector4
	*************************************************************************//**
	*  @fn        inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector4(ConstVector128 left, ConstVector128 right)
	*
	*  @brief     内積を算出
	*
	*  @param[in] ConstVector128 left
	*  @param[in] ConstVector128 right
	*
	*  @return 　　float
	*****************************************************************************/
	inline float SIMD_CALL_CONVENTION Vector128Utility::DotVector4(ConstVector128 left, ConstVector128 right) noexcept
	{
		return _mm_cvtss_f32(_mm_dp_ps(left, right, 0xff));
	}

	/****************************************************************************
	*                      NormalizeVector2
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	*
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector2(ConstVector128 vector) noexcept
	{
		sse::Vector128 squareLength = _mm_dp_ps(vector, vector, 0x3f);
		sse::Vector128 norm         = _mm_sqrt_ps(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		sse::Vector128 zeroMask = _mm_setzero_ps();
		zeroMask           = _mm_cmpneq_ps(zeroMask, norm);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		sse::Vector128 infinityMask = _mm_cmpneq_ps(squareLength, sse::VECTOR_128F_INFINITY);

		// 正規化の実行
		sse::Vector128 result = _mm_div_ps(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm_and_ps(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
		sse::Vector128 temp  = _mm_andnot_ps(infinityMask, sse::VECTOR_128F_QNAN);
		sse::Vector128 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp, temp2);
		return result;
	}

	/****************************************************************************
	*                      NormalizeVector3
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector3(ConstVector128 vector) noexcept
	*
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector3(ConstVector128 vector) noexcept
	{
		sse::Vector128 squareLength = _mm_dp_ps(vector, vector, 0x7f);
		sse::Vector128 norm         = _mm_sqrt_ps(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		sse::Vector128 zeroMask = _mm_setzero_ps();
		zeroMask           = _mm_cmpneq_ps(zeroMask, norm);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		sse::Vector128 infinityMask = _mm_cmpneq_ps(squareLength, sse::VECTOR_128F_INFINITY);

		// 正規化の実行
		sse::Vector128 result = _mm_div_ps(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm_and_ps(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
		sse::Vector128 temp  = _mm_andnot_ps(infinityMask, sse::VECTOR_128F_QNAN);
		sse::Vector128 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp, temp2);
		return result;
	}

	/****************************************************************************
	*                      NormalizeVector4
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector4(ConstVector128 vector) noexcept
	*
	*  @brief     入力ベクトルの単位ベクトルを返します
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::NormalizeVector4(ConstVector128 vector) noexcept
	{
		sse::Vector128 squareLength = _mm_dp_ps(vector, vector, 0xff);
		sse::Vector128 norm = _mm_sqrt_ps(squareLength);

		// 0で割られる可能性があるかどうかを調べる. 値を持っていたら1
		sse::Vector128 zeroMask = _mm_setzero_ps();
		zeroMask = _mm_cmpneq_ps(zeroMask, norm);

		// 無限大でない場合は対応する要素が1になるマスクの作成
		sse::Vector128 infinityMask = _mm_cmpneq_ps(squareLength, sse::VECTOR_128F_INFINITY);

		// 正規化の実行
		sse::Vector128 result = _mm_div_ps(vector, norm);

		// 0付近で割られて無限大になったものに対しては0に設定
		result = _mm_and_ps(result, zeroMask);

		// 無限大かに基づいてqnanまたはresultを選択する
		sse::Vector128 temp = _mm_andnot_ps(infinityMask, sse::VECTOR_128F_QNAN);
		sse::Vector128 temp2 = _mm_and_ps(result, infinityMask);
		result = _mm_or_ps(temp, temp2);
		return result;
	}

	/****************************************************************************
	*                       Round
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Round(ConstVector128 vector) noexcept
	*
	*  @brief     最も近い整数に丸める
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::Round(ConstVector128 vector) noexcept
	{
		return _mm_round_ps(vector, _MM_FROUND_TO_NEAREST_INT | _MM_FROUND_NO_EXC);
	}

	/****************************************************************************
	*                       Truncate
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Truncate(ConstVector128 vector) noexcept
	*
	*  @brief     切り捨て (指定した数値の端数を切り捨てた整数に丸める。数直線上で0の方向にある次の整数
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::Truncate(ConstVector128 vector) noexcept
	{
		return _mm_round_ps(vector, _MM_FROUND_TO_ZERO | _MM_FROUND_NO_EXC);
	}

	/****************************************************************************
	*                       Floor
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Floor(ConstVector128 vector) noexcept
	*
	*  @brief     負の方向に丸める
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::Floor(ConstVector128 vector) noexcept
	{
		return _mm_floor_ps(vector);
	}

	/****************************************************************************
	*                      Ceiling
	*************************************************************************//**
	*  @fn        inline Vector128 SIMD_CALL_CONVENTION Vector128Utility::Ceiling(ConstVector128 vector) noexcept
	*
	*  @brief     切り上げ 数直線上で正の方向にある次の整数に丸める
	*
	*  @param[in] ConstVector128 vector
	*
	*  @return 　　Vector128
	*****************************************************************************/
	inline sse::Vector128 SIMD_CALL_CONVENTION Vector128Utility::Ceiling(ConstVector128 vector) noexcept
	{
		return _mm_ceil_ps(vector);
	}
	#pragma endregion Math
}


#pragma endregion Implement
#endif
#endif