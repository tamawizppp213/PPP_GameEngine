//////////////////////////////////////////////////////////////////////////////////
///             @file    GUParse.hpp
///             @brief   対象の文字列を検索し, 指定した数値を読み取ります. 
///             @details 読み込み順序が定まっていない文字列(環境変数の取得など)に対する値の読み込みに効果を発揮します. 
/// 　　　　　　　　　　　    数値データが連続して配置されていたり, 読み込み順序が決まっているような構造の場合には, FileSystemクラスを利用することをお勧めします.
///                      この理由は, streamポインタ自身のメモリをずらすことは行わず, streamの先頭から文字列検索が行われるため, 読み込み速度が大きく低下するためです.
///                      
///                      Value -> 読み込み順序が決定されていないものを探すときに使用します.
///                               文字列全体を探し, Matchに合う文字列の箇所の後に続く数値を読み取ります. (DesireToFind=100) 
///                      Bool  -> Valueと同じ使い方です. 
///                               Trueの場合の認識文字列はTrue, Yes, On, 1 (大文字小文字区別なし)
///                               Falseの場合の認識文字列はFalse, No, Off, 0 (大文字小文字区別なし)
/// 　　　　　　　　　　　　　
///             @author toide
///             @date   2024/02/28 0:22:43
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_PARSE_HPP
#define GU_PARSE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GUType.hpp"
#include "GUClassUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	namespace details::string
	{
		template<class Char, int CharSize> class StringBase;
	}
	using tstring = details::string::StringBase<tchar, 2>; // 1byteも含める場合は要調整
}

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GUParse
	*************************************************************************//**
	*  @class     GUParse
	*  @brief     読み込み順序が定まっていない文字列(環境変数の取得など)に対する値の読み込みに効果を発揮します. 
	              Value -> 読み込み順序が決定されていないものを探すときに使用します.
                           文字列全体を探し, Matchに合う文字列の箇所の後に続く数値を読み取ります. (DesireToFind=100) 
　 　　　　　　　　　　Bool  -> Valueと同じ使い方です.
			               Trueの場合の認識文字列はTrue, Yes, On, 1 (大文字小文字区別なし)
                           Falseの場合の認識文字列はFalse, No, Off, 0 (大文字小文字区別なし)
	*****************************************************************************/
	class Parse final
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : 文字列全体を探し, Matchに合う文字列の箇所の後に続く数値を読み取ります. 
		*           型の判定は引数に入れた型で自動判別してしまうため, 事前に型は決めておいてください. 
		/*----------------------------------------------------------------------*/
		static bool Value(const tstring& stream, const tstring& match, int8  & value);
		static bool Value(const tstring& stream, const tstring& match, int16 & value);
		static bool Value(const tstring& stream, const tstring& match, int32 & value);
		static bool Value(const tstring& stream, const tstring& match, int64 & value);
		static bool Value(const tstring& stream, const tstring& match, uint8 & value);
		static bool Value(const tstring& stream, const tstring& match, uint16& value);
		static bool Value(const tstring& stream, const tstring& match, uint32& value);
		static bool Value(const tstring& stream, const tstring& match, uint64& value);

		/*----------------------------------------------------------------------
		*  @brief : 文字列全体を探し, Matchに合う文字列の箇所の後に続くBool値を取得します
		*           検索にひっかかかる文字列は以下の通りです
		*           True  : True, Yes, On, 1
		*           False : False, No, Off, 0 
		/*----------------------------------------------------------------------*/
		static bool Bool(const tstring& stream, const tstring& match, bool& onOff);

		/*----------------------------------------------------------------------
		*  @brief : 文字列全体を探し, Parameter名が文字列中に含まれるかを検索します
		*           文字列は大文字小文字を区別します.
		/*----------------------------------------------------------------------*/
		static bool Contains(const tstring& stream, const tstring& parameter);

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		
		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};
}

#endif