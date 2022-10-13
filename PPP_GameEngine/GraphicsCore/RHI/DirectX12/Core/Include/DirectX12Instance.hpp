//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIInstance.hpp
///             @brief  Select device api (���̃G���W�����g�p���ŏ��ɌĂяo��.) 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef DIRECTX12_RHI_INSTANCE_HPP
#define DIRECTX12_RHI_INSTANCE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIInstance.hpp"
#include "DirectX12Core.hpp"
#include <dxgi1_6.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::directX12
{
	/****************************************************************************
	*				  			RHIInstance
	*************************************************************************//**
	*  @class     RHIInstance
	*  @brief     Select device api and select display adapter
	*****************************************************************************/
	class RHIInstance : public core::RHIInstance
	{
	public:

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* directX12 : (High) xGPU, dGPU iGPU (Low) selected*/
		std::shared_ptr<core::RHIDisplayAdapter> SearchHighPerformanceAdapter() override;
		/* return all available display adapter*/
		std::vector<std::shared_ptr<core::RHIDisplayAdapter>> EnumrateAdapters() override;
		/* OutputDebugString : adapter list*/
		void LogAdapters() override;
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		FactoryComPtr GetFactory() { return _factory; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIInstance() = default;
		RHIInstance(bool enableCPUDebugger, bool enableGPUDebugger); 
		~RHIInstance();


	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/
		void EnabledDebugLayer();           // debug mode only use
		void EnabledShaderBasedValidation(); // It has a significant impact on the frame rate.
		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		FactoryComPtr _factory = nullptr;
	};
}

#endif