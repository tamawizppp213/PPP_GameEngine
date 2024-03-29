//////////////////////////////////////////////////////////////////////////////////
///             @file   RHICommandAllocator.hpp
///             @brief  CommandAllocator
///                     This class is used to store the command list memory.
///                     Every time a command list is reset, it will allocate a block of memory.
///                     Even if a command list is reset, these memories aren't lost. 
///                     When the Reset function in this class is called, these memories cleans up. 
///                     To acieve the maximum frame rate, you should create each command list one by one.
///             @author Toide Yutaro
///             @date   2022_06_23
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMAND_ALLOCATOR_HPP
#define RHI_COMMAND_ALLOCATOR_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHICommonState.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
#include "GameUtility/Base/Include/GUSmartPointer.hpp"
#include "GameUtility/Base/Include/GUAssert.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Allocator class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	class RHICommandList;
	class RHIDevice;
	/****************************************************************************
	*				  			RHICommandAllocator
	*************************************************************************//**
	*  @class     RHICommandAllocator
	*  @brief     Allocation of memory used by command list
	*****************************************************************************/
	class RHICommandAllocator : public gu::NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/*---------------------------------------------------------------
		　　　　　@brief : Reset command allocator (clear command list buffer)
			   @note 　: All binded command lists must be closed before calling this function.
					　　In addition, until command execution in GPU, this function mustn't be called.
		-----------------------------------------------------------------*/
		virtual void CleanUp() = 0;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		/*---------------------------------------------------------------
		　　　　　@brief : Return command list type (Graphics, compute, copy)
		-----------------------------------------------------------------*/
		core::CommandListType GetCommandListType() const { return _commandListType; }

		/*---------------------------------------------------------------
		　　　　　@brief : For Debugging name
		-----------------------------------------------------------------*/
		virtual void SetName(const gu::tstring& name) = 0;

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		RHICommandAllocator () = default;

		virtual ~RHICommandAllocator() = default;

		explicit RHICommandAllocator(const gu::SharedPointer<RHIDevice>& device, const core::CommandListType commandListType)
			: _device(device),  _commandListType(commandListType) 
		{
			Checkf(_device, "device is nullptr.\n");
		};
		
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		gu::SharedPointer<RHIDevice> _device = nullptr;

		CommandListType   _commandListType = core::CommandListType::Unknown;
	};
}
#endif