//////////////////////////////////////////////////////////////////////////////////
///             @file   GUOptional.hpp
///             @brief  Optional��, �l�̗L���������m�F�ł���N���X�ł�. 
///                     HasValue�Œl���L���l�����m�F����
///                     Value�Ŏ��ۂ̒l���擾����. 
///             @author toide
///             @date   2024/02/26 0:54:45
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_OPTIONAL_HPP
#define GU_OPTIONAL_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUTypeCast.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   GUOptional
	*************************************************************************//**
	/* @class     GUOptional
	*  @brief     Optional��, �l�̗L���������m�F�ł���N���X�ł�. 
    *  @brief     HasValue�Œl���L���l�����m�F����
    *  @brief     Value�Ŏ��ۂ̒l���擾����. 
	*  @brief	  https://qiita.com/ashtkn/items/d6de4a9f7524eadb4222
	*****************************************************************************/
	template<class ElementType>
	class Optional
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		
		/*-------------------------------------------------------------------
		*   @brief   �l���L���l(��������̒l���������Ă�����)�����m�F���܂�. 
		*            true�ł���ΗL���l���ݒ肳��Ă�����̂Ƃ��܂�. 
		*---------------------------------------------------------------------*/
		constexpr bool HasValue() const noexcept { return _hasValue; }

		/*-------------------------------------------------------------------
		*          @brief �l�𖳌��l�̏ꍇ�Ɠ�����Ԃɏ��������s���܂�. 
		*---------------------------------------------------------------------*/
		__forceinline void Reset() { _hasValue = false; _value = ElementType(); }

		/*-------------------------------------------------------------------
		-           �l�̎擾
		---------------------------------------------------------------------*/
		// @brief : �ێ�����l�̍��Ӓl�Q�Ƃ��擾����
		constexpr ElementType& Value()&
		{
			if (!_hasValue) { Check(false); }
			return _value;
		}

		// @brief : �ێ�����l�̉E�Ӓl�Q�Ƃ��擾����
		constexpr ElementType&& Value()&&
		{
			if (!_hasValue) { Check(false); }
			return gu::Forward<ElementType>(_value)
		}

		// @brief : �ێ�����l�̍��Ӓl�Q�Ƃ��擾����
		constexpr const ElementType& Value() const&
		{
			if (!_hasValue) { Check(false); }
			return _value;
		}

		// @brief : �ێ�����l�̉E�Ӓl�Q�Ƃ��擾����
		constexpr const ElementType&& Value() const&&
		{
			if (!_hasValue) { Check(false); }
			return gu::Forward<ElementType>(_value)
		}

#pragma region Operator Function
		// @brief : �L���l���ǂ�����Ԃ�
		constexpr explicit operator bool() const noexcept { return _hasValue; }

		// @brief : �����o�A�N�Z�X���Z�q
		constexpr       ElementType* operator->()       noexcept { return &_value; }
		constexpr const ElementType* operator->() const noexcept { return &_value; }

		// @brief : �ԐڎQ�Ɖ��Z�q (���Ӓl�Q��)
		constexpr ElementType& operator*() & noexcept { return _value; }

		// @brief : �ԐڎQ�Ɖ��Z�q (�E�Ӓl�Q��)
		constexpr ElementType&& operator*() && noexcept { return gu::Forward<Optional<ElementType>>(_value); }

		// @brief : �ԐڎQ�Ɖ��Z�q (const ���Ӓl�Q��)
		constexpr const ElementType& operator* () const& noexcept { return _value; }

		// @brief : �ԐڎQ�Ɖ��Z�q 8const �E�Ӓl�Q��)
		constexpr const ElementType&& operator*() const&& noexcept { return gu::Forward<Optional<ElementType>>(_value); }

#pragma endregion Operator Function

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		// @brief: �l�������Ă��Ȃ��I�u�W�F�N�g���\�z����
		constexpr Optional() noexcept : _value(), _hasValue(false) {};

		// @brief : �l�������Ă���I�u�W�F�N�g���\�z����
		constexpr Optional(const ElementType& value) : _value(value), _hasValue(true) {};

		// @brief : �R�s�[�R���X�g���N�^
		constexpr Optional(const Optional& other) : _value(other._value), _hasValue(other._hasValue) {};

		// @brief : ���[�u�R���X�g���N�^
		constexpr Optional(Optional<ElementType>&& other) : _value(gu::Forward<Optional<ElementType>>(other._value)), _hasValue(other._hasValue)
		{
			other._hasValue = false;
		}

		~Optional() = default;

		// @brief : �R�s�[������Z�q
		Optional& operator=(const Optional& other)
		{
			if (this != &other) { _value = other._value; _hasValue = other._hasValue; }
			return *this;
		}

		// ���[�u������Z�q
		Optional& operator=(Optional<ElementType>&& other)
		{
			if (this != &other)
			{
				_value = Forward<Optional<ElementType>>(other._value);
				_hasValue = other._hasValue;
				other._hasValue = false;
			}
		}

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		// @brief : ���ۂ̒l
		ElementType _value = ElementType();

		// @brief : �l���������Ă��邩 
		bool _hasValue = false;
	};

}
#endif