//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIResourceLayout.hpp
///             @brief  Resource Layout (DirectX12 :: Root Signature) 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_RESOURCE_LAYOUT_HPP
#define RHI_RESOURCE_LAYOUT_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIResourceLayoutElement.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <vector>
#include <optional>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////]
namespace rhi::core
{
	class RHIDevice;

	/****************************************************************************
	*				  			GPUResource
	*************************************************************************//**
	*  @class     GPUResource
	*  @brief     Resource 
	*****************************************************************************/
	class RHIResourceLayout : public NonCopyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		ResourceLayoutElement GetResourceElement(const size_t index) const { return _elements[index]; }
		SamplerLayoutElement  GetSamplerElement(const size_t index) const { return _samplers[index]; }
		std::optional<Constant32Bits> GetConstant32Bits() const noexcept { return _constant32Bits; }

		const std::vector<ResourceLayoutElement>& GetResourceElements() const noexcept { return _elements; }
		const std::vector<SamplerLayoutElement>&  GetSamplerElements() const noexcept { return _samplers; }
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		RHIResourceLayout() = default;
		~RHIResourceLayout() = default;
		explicit RHIResourceLayout(
			const std::shared_ptr<RHIDevice>& device,
			const std::vector<core::ResourceLayoutElement>& elements = {},
			const std::vector<core::SamplerLayoutElement>& samplers = {},
			const std::optional<core::Constant32Bits>& constant32Bits = std::nullopt
		) : _device(device), _elements(elements), _samplers(samplers), _constant32Bits(constant32Bits){ };
		

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		std::shared_ptr<RHIDevice> _device = nullptr;
		std::vector<core::ResourceLayoutElement> _elements       = {};
		std::vector<core::SamplerLayoutElement>  _samplers       = {};
		std::optional<core::Constant32Bits>      _constant32Bits = std::nullopt;
	};
}
#endif