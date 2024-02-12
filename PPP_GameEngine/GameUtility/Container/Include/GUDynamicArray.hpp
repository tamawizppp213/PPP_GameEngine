//////////////////////////////////////////////////////////////////////////////////
///             @file   GUArray.hpp
///             @brief  ������
///             @author toide
///             @date   2023/11/25 14:04:42
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_DYNAMIC_ARRAY_HPP
#define GU_DYNAMIC_ARRAY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Include/GUAssert.hpp"
#include "GameUtility/Memory/Include/GUMemory.hpp"
#include "GameUtility/Container/Include/GUInitializerList.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gu
{
	/****************************************************************************
	*				  			   DynamicArray
	*************************************************************************//**
	*  @class     DynamicArray
	*  @brief     ���I�z��
	*****************************************************************************/
	template<class ElementType>
	class DynamicArray
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �T�C�Y�������I�ɕύX���܂�.
		*           �����ɉ����Ĉȉ��̋��������܂�. 
		*           size <= ���X�����Ă���T�C�Y : �����s��Ȃ�
		*           size >  ���X�����Ă���T�C�Y : �w��T�C�Y =�@capacity�̗̈���m�ۂ�����ŃR���X�g���N�^�Ăяo���܂ōs��
		*  
		*  @param[in] const uint64 �z��̃T�C�Y (capacity�ł͂���܂���)
		*  @param[in] const bool   �R�s�[�R���X�g���N�^���g�p���邩 (false�ɂ���ƌ������̂��߂ɖ���������ԂŎn�܂�܂�.)
		*  @param[in] const ElementType& �R�s�[���̗v�f (��{�f�t�H���g�R���X�g���N�^������ꂽ����.)
		/*----------------------------------------------------------------------*/
		void Resize(const uint64 size, const bool useCopyConstructor = true, const ElementType& defaultElement = ElementType());

		/*----------------------------------------------------------------------
		*  @brief : �������̈�����O�Ɋm�ۂ��܂�. (������, �R���X�g���N�^�̌Ăяo���͑S���s���܂���)
		*           Resize�Ƃ̈Ⴂ�Ƃ���, Size�̕ύX�͂Ȃ�, Capacity�������ς��܂�. 
		*           ���̂���, At�֐��Ȃǂ�p���Ĕz��Ƀ����_���A�N�Z�X���s���ƃG���[�ƂȂ�܂�. 
		/*----------------------------------------------------------------------*/
		void Reserve(const uint64 capacity);

		/*----------------------------------------------------------------------
		*  @brief : �T�C�Y��0�ɂ��܂���, Capacity�̈掩�̂͂��̂܂܂ɂ��܂�.
		*           �f�X�g���N�^�͌Ăяo����܂�. 
		/*----------------------------------------------------------------------*/
		void Clear();

		/*----------------------------------------------------------------------
		*  @brief : Capacity���R���e�i��Size�܂Ő؂�l�߂�
		*           capacity��0�̂Ƃ�, capacity�ȏ��size�����鎞�͏������s���܂���.
		/*----------------------------------------------------------------------*/
		void ShrinkToFit();

		/*----------------------------------------------------------------------
		*  @brief : �z������ɒǉ�����. Capacity�𒴂����ꍇ, �S�̂̃�������2�{�̑傫���ōĊ��蓖�Ă��s��.
		/*----------------------------------------------------------------------*/
		void Push(const ElementType& element);

		/*----------------------------------------------------------------------
		*  @brief : �z�����납����o��. ���̍�, �f�X�g���N�^���Ăяo��.
		/*----------------------------------------------------------------------*/
		void Pop();

		/*----------------------------------------------------------------------
		*  @brief : �w�肵���v�f�����Ɋ܂܂�Ă���ꍇ��true��Ԃ��܂�.
		/*----------------------------------------------------------------------*/
		bool Contains(const ElementType& element) const;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*----------------------------------------------------------------------
		*  @brief : �q�[�v�̈�Ɋi�[����Ă���擪�̃|�C���^��Ԃ�
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType* Data()       { return _data; }
		__forceinline const ElementType* Data() const { return _data; }

		/*----------------------------------------------------------------------
		*  @brief : �擪�̗v�f
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& Front()       {return _data;}
		__forceinline const ElementType& Front() const { return _data; }

		/*----------------------------------------------------------------------
		*  @brief :�@�Ō�̗v�f
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& Back()        { return _data[_size - 1]; }
		__forceinline const ElementType& Back() const { return _data[_size - 1]; }

		/*----------------------------------------------------------------------
		*  @brief : �z�񂪋󂩂ǂ���
		/*----------------------------------------------------------------------*/
		__forceinline bool IsEmpty() const { return _size == 0; }

		/*----------------------------------------------------------------------
		*  @brief : �z��̗v�f��
		/*----------------------------------------------------------------------*/
		__forceinline uint64 Size() const { return _size; }

		/*----------------------------------------------------------------------
		*  @brief : �z��ɋl�ߍ��߂�ő�v�f��
		/*----------------------------------------------------------------------*/
		__forceinline uint64 Capacity() const { return _capacity; }

		/*----------------------------------------------------------------------
		*  @brief : �v�f�̃o�C�g����Ԃ�
		/*----------------------------------------------------------------------*/
		__forceinline static constexpr uint32 ByteOfElement() { return sizeof(ElementType); }

		/*----------------------------------------------------------------------
		*  @brief : �z��ɋl�ߍ��߂�c��̗v�f����Ԃ� (capacity - size)
		/*----------------------------------------------------------------------*/
		__forceinline uint64 ResidueCount() const { return _capacity - _size; }

		/*----------------------------------------------------------------------
		*  @brief : �w�肵��Index���̈�����ǂ���, �͈͊O�ł����Ă��r���Ŏ~�߂Ȃ�
		/*----------------------------------------------------------------------*/
		__forceinline bool InRange(const uint64 index) const { return 0 <= index && index <= _size; }

		/*----------------------------------------------------------------------
		*  @brief : �w�肵��Index���̈�����ǂ���, �͈͊O�ł�������r���Ŏ~�߂�
		/*----------------------------------------------------------------------*/
		__forceinline void CheckRange(const uint64 index) const
		{
			Checkf(0 <= index && index <= _size, "index is out of range. \n");
		}

		/*----------------------------------------------------------------------
		*  @brief : �f�[�^��͈͊m�F���s������ŕԂ�. 
		*           �͈͊O�ɓ������ꍇ�̓u���[�N�|�C���g�������I�ɂ�����, �������~�܂�܂�. 
		/*----------------------------------------------------------------------*/
		__forceinline       ElementType& At(const uint64 index)       { CheckRange(index); return _data[index]; }
		__forceinline const ElementType& At(const uint64 index) const { CheckRange(index); return _data[index]; }

#pragma region Operator Function
		// �������̂��߂ɔ͈̓`�F�b�N���{���Ă��܂���. �͈̓`�F�b�N���s�������ꍇ��At���g�p���Ă�������
		__forceinline ElementType& operator[](const uint64 index)
		{
			return _data[index];
		}
		// �������̂��߂ɔ͈̓`�F�b�N���{���Ă��܂���. �͈̓`�F�b�N���s�������ꍇ��At���g�p���Ă�������
		__forceinline const ElementType& operator[](const uint64 index) const
		{
			return _data[index];
		}

		// �g���ʂ��g���đ�����s����悤�ɂ���
		DynamicArray& operator=(std::initializer_list<ElementType> list)
		{
			Clear();
			CreateFromOtherArray(list.begin(), list.size());
			return *this;
		}

		// �ق���Array(�^������)������
		DynamicArray& operator=(const DynamicArray& other)
		{
			if (this != &other)
			{
				Clear();
				CreateFromOtherArray(other.Data(), other.Size());
			}
			return *this;
		}

		// ���[�u�R���X�g���N�^
		DynamicArray& operator=(DynamicArray&& other)
		{
			if (this != &other)
			{
				// �q�[�v�̈�̑S�̎��͎��Ԃ������邽��, �����܂Ń|�C���^�̕t���ւ������őΉ����܂���.
				_data     = other._data;     other._data     = nullptr;
				_size     = other._size;     other._size     = nullptr;
				_capacity = other._capacity; other._capacity = nullptr;
			}
			return *this;
		}
#pragma endregion Operator Function
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		DynamicArray() : _size(0), _capacity(0), _data(nullptr) {};

		// �z��̗v�f����쐬����R���X�g���N�^
		DynamicArray(const ElementType* pointer, const uint64 count)
		{
			CreateFromOtherArray(pointer, count);
		}

		// �g���ʂŏ��������o����R���X�g���N�^
		DynamicArray(std::initializer_list<ElementType> list)
		{
			CreateFromOtherArray(list.begin(), list.size());
		}

		// �R�s�[�R���X�g���N�^
		DynamicArray(const DynamicArray& other) { CreateFromOtherArray(other.Data(), other.Size()); }

		// ���[�u�R���X�g���N�^
		DynamicArray(DynamicArray&& other)
		{
			// �q�[�v�̈�̑S�̎��͎��Ԃ������邽��, �����܂Ń|�C���^�̕t���ւ������őΉ����܂���.
			_data     = other._data;     other._data     = nullptr;
			_size     = other._size;     other._size     = nullptr;
			_capacity = other._capacity; other._capacity = nullptr;
		}

		~DynamicArray()
		{
			if (_data) 
			{
				Memory::ForceExecuteDestructors(_data, _size);
				Memory::Free(_data); 
			} 
		}
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		// @brief : ���ۂɐ��l�����͂���Ă���Index�̌�
		uint64 _size     = 0;

		// @brief : �ő���������e��
		uint64 _capacity = 0;

		// @brief : �v�f���i�[����z��̐擪�|�C���^
		ElementType* _data = nullptr;

	private:
		/****************************************************************************
		**                Private Function
		*****************************************************************************/
		void CreateFromOtherArray(const ElementType* pointer, const uint64 count);

		/****************************************************************************
		**                Private Member Variables
		*****************************************************************************/
	};

#pragma region Implement
#pragma region Constructor and Destructor
	
#pragma endregion Constructor and Destructor
	template<class ElementType>
	void DynamicArray<ElementType>::CreateFromOtherArray(const ElementType* pointer, const uint64 count)
	{
		Check(pointer != nullptr);
		Resize(count, false);

		if constexpr (IS_ARITHMETIC<ElementType>)
		{
			Memory::Copy(_data, pointer, sizeof(ElementType) * count)
		}
		else
		{
			Memory::ForceExecuteCopyConstructors(_data, pointer, count);
		}
	}

	/****************************************************************************
	*                    Resize
	*************************************************************************//**
	*  @fn       �@void DynamicArray<ElementType>::Reserve(const uint64 capacity)
	*
	*  @brief     �T�C�Y��ύX���܂�
	*
	*  @param[in] const uint64 �z��̃T�C�Y (capacity�ł͂���܂���)
	*  @param[in] const bool   �R�s�[�R���X�g���N�^���g�p���邩 (false�ɂ���ƌ������̂��߂ɖ���������ԂŎn�܂�܂�.)
	* �@@param[in] const ElementType& �R�s�[���̗v�f (��{�f�t�H���g�R���X�g���N�^������ꂽ����.)
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Resize(const uint64 size, const bool useConstructor, const ElementType& defaultElement)
	{
		if (size <= _size) { return; }

		// �K��������
		Reserve(size);
		
		// �������R�s�[���s���񐔂�ݒ肷��
		const auto residueSize = size - _size;

		// �R�s�[�R���X�g���N�^���g���ꍇ��������������
		if (useConstructor)
		{
			if constexpr (sizeof(ElementType) == 1) // bool or character or byte
			{
				Memory::Set(_data, static_cast<uint8>(defaultElement), size); // 1 byte�ł��邱�Ƃ͕������Ă���̂�
			}
			else
			{
				for (uint64 i = 0; i < size; ++i)
				{
					_data[i] = defaultElement;
				}
			}
		}

		// �T�C�Y�̕ύX
		_size = size;
	}

	/****************************************************************************
	*                    Reserve
	*************************************************************************//**
	*  @fn       �@void DynamicArray<ElementType>::Reserve(const uint64 capacity)
	*
	*  @brief     �������̈�����O�Ɋm�ۂ��܂�. (������, �R���X�g���N�^�̌Ăяo���͑S���s���܂���)
	*
	*  @param[in] const uint64 capacity (�ő���������e��(�z��), �o�C�g�T�C�Y�ł͂Ȃ����Ƃɒ���)
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Reserve(const uint64 capacity)
	{
		/*-------------------------------------------------------------------
		-           ���Ƃ��Ƃ�菬���������珈�������Ȃ�
		---------------------------------------------------------------------*/
		if (capacity <= _capacity) { return; }

		/*-------------------------------------------------------------------
		-           ���Ƃ��Ɣz�񂪑��݂��Ă���ΑS�Ẵ��������R�s�[���������ō폜����
		---------------------------------------------------------------------*/
		auto newData = Memory::Allocate(capacity);

		if (_data != nullptr && _capacity)
		{
			Memory::Copy(newData, _data, _capacity * sizeof(ElementType));
			Memory::Free(_data);
		}

		/*-------------------------------------------------------------------
		-           Data�̍X�V
		---------------------------------------------------------------------*/
		_data     = (ElementType*)newData;
		_capacity = capacity;
	}

	/****************************************************************************
	*                    Clear
	*************************************************************************//**
	*  @fn       �@void DynamicArray<ElementType>::Clear()
	*
	*  @brief     �T�C�Y��0�ɂ��܂���, Capacity�̈掩�̂͂��̂܂܂ɂ��܂�
	*
	*  @param[in] void
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Clear()
	{
		/*-------------------------------------------------------------------
		-           �T�C�Y��0�Ȃ牽�����Ȃ�
		---------------------------------------------------------------------*/
		if (Size() == 0) { return; }

		/*-------------------------------------------------------------------
		-           �f�X�g���N�^����ʂ�Ăяo���Ă��烊�Z�b�g
		---------------------------------------------------------------------*/
		Memory::ForceExecuteDestructors(_data, Size());

		_size = 0;
	}

	/****************************************************************************
	*                    ShrinkToFit
	*************************************************************************//**
	*  @fn       �@void DynamicArray<ElementType>::ShrinkToFit()
	*
	*  @brief      Capacity���R���e�i��Size�܂Ő؂�l�߂� 
	*
	*  @param[in] void
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::ShrinkToFit()
	{
		/*-------------------------------------------------------------------
		-           �K�؂�Capacity���𒲂ׂ�
		---------------------------------------------------------------------*/
		if (_capacity == 0)     { return; }
		if (_size >= _capacity) { return; }

		/*-------------------------------------------------------------------
		-           ����̏ꍇ��_capacity�̕������size���傫�����Ƃ��������Ă��邽��, 
		            _data�ɒ��ڑ�����s���Ă���. 
		---------------------------------------------------------------------*/
		_data     = (ElementType*)Memory::Reallocate(_data, _size * sizeof(ElementType));
		_capacity = _size;
	}

	/****************************************************************************
	*                    Push
	*************************************************************************//**
	*  @fn       �@void DynamicArray<ElementType>::ShrinkToFit()
	*
	*  @brief      �z������ɒǉ�����. Capacity�𒴂����ꍇ, �S�̂̃�������2�{�̑傫���ōĊ��蓖�Ă��s��.
	*
	*  @param[in] ElementType& element
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Push(const ElementType& element)
	{
		if (_capacity <= _size)
		{
			Reserve(_capacity == 0 ? 1 : _size * 2);
		}

		_data[_size] = element;
		++_size;
	}

	/****************************************************************************
	*                    Pop
	*************************************************************************//**
	*  @fn       �@void DynamicArray<ElementType>::Pop()
	*
	*  @brief     �Ō�̗v�f�����o���ăf�X�g���N�^���Ăяo��. (���������̂̔j���͍s��ꂸ, ���̌�T�C�Y�����ύX���܂�)
	*
	*  @param[in] void
	*
	*  @return �@�@void
	*****************************************************************************/
	template<class ElementType>
	void DynamicArray<ElementType>::Pop()
	{
		if (_size == 0) { return; }

		Memory::ForceExecuteDestructors(&_data[_size - 1], 1);
		_size--;
	}

	/****************************************************************************
	*                    Contains
	*************************************************************************//**
	*  @fn       �@bool DynamicArray<ElementType>::Contains(const ElementType& element) const
	*
	*  @brief     �w�肵���v�f�����Ɋ܂܂�Ă���ꍇ��true��Ԃ��܂�.
	*
	*  @param[in] const ElementType& element
	*
	*  @return �@�@bool �܂܂�Ă���ꍇ��true
	*****************************************************************************/
	template<class ElementType>
	bool DynamicArray<ElementType>::Contains(const ElementType& element) const
	{
		for (uint64 i = 0; i < _size; ++i)
		{
			if(*_data[i] == element)
			{
				return true;
			}
		}
		return false;
	}
#pragma endregion Implement
}

#endif