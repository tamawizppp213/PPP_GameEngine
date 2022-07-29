//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12EnumConverter.cpp
///             @brief  Enum Converter
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/Vulkan/Core/Include/VulkanEnumConverter.hpp"
#include <stdexcept>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::vulkan;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Shader Stage
VkShaderStageFlagBits  EnumConverter::Convert(const rhi::core::ShaderType type)
{
	switch (type)
	{
		case core::ShaderType::Vertex       : return VkShaderStageFlagBits::VK_SHADER_STAGE_VERTEX_BIT;
		case core::ShaderType::Pixel        : return VkShaderStageFlagBits::VK_SHADER_STAGE_FRAGMENT_BIT;
		case core::ShaderType::Compute      : return VkShaderStageFlagBits::VK_SHADER_STAGE_COMPUTE_BIT;
		case core::ShaderType::Geometry     : return VkShaderStageFlagBits::VK_SHADER_STAGE_GEOMETRY_BIT;
		case core::ShaderType::Amplification: return VkShaderStageFlagBits::VK_SHADER_STAGE_TASK_BIT_NV;
		case core::ShaderType::Mesh         : return VkShaderStageFlagBits::VK_SHADER_STAGE_MESH_BIT_NV;
		case core::ShaderType::Hull         : return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
		case core::ShaderType::Domain       : return VkShaderStageFlagBits::VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
		default:
			throw std::runtime_error("Not support shader stage");
	}
}
#pragma endregion Shader Stage
#pragma region Sampler State
VkFilter               EnumConverter::Convert(const rhi::core::FilterOption filter, const rhi::core::FilterMask mask)
{
	if (filter == core::FilterOption::Anisotropy) { return VkFilter::VK_FILTER_LINEAR; }
	return (static_cast<std::uint8_t>(filter) & static_cast<std::uint8_t>(mask)) != 0 ? VkFilter::VK_FILTER_LINEAR : VkFilter::VK_FILTER_NEAREST;

}
VkSamplerMipmapMode    EnumConverter::Convert(const rhi::core::FilterOption filter)
{
	if (filter == core::FilterOption::Anisotropy) return VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR;
	return (static_cast<std::uint8_t>(filter) & 1) != 0 ?
		VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_LINEAR:
		VkSamplerMipmapMode::VK_SAMPLER_MIPMAP_MODE_NEAREST;
}
VkSamplerAddressMode   EnumConverter::Convert(const rhi::core::SamplerAddressMode addressingMode)
{
	switch (addressingMode)
	{
		case core::SamplerAddressMode::Wrap   : return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_REPEAT;
		case core::SamplerAddressMode::Mirror : return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT;
		case core::SamplerAddressMode::Clamp  : return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
		case core::SamplerAddressMode::Boarder: return VkSamplerAddressMode::VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER;
		default:
			throw std::runtime_error("Not support texture addressing mode");
	}
}
VkBorderColor         EnumConverter::Convert(const rhi::core::BorderColor borderColor)
{
	switch (borderColor)
	{
		case core::BorderColor::TransparentBlack: return VkBorderColor::VK_BORDER_COLOR_FLOAT_TRANSPARENT_BLACK;
		case core::BorderColor::OpaqueBlack: return VkBorderColor::VK_BORDER_COLOR_FLOAT_OPAQUE_BLACK;
		case core::BorderColor::OpaqueWhite: return VkBorderColor::VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
		default:
			throw std::runtime_error("Not support border color");
	}
}
#pragma endregion Sampler State
VkFormat EnumConverter::Convert(const rhi::core::PixelFormat pixelFormat)
{
	switch (pixelFormat)
	{
		case core::PixelFormat::R8G8B8A8_UNORM    : return VkFormat::VK_FORMAT_R8G8B8A8_UNORM;
		case core::PixelFormat::B8G8R8A8_UNORM    : return VkFormat::VK_FORMAT_B8G8R8A8_UNORM;
		case core::PixelFormat::R16G16B16A16_FLOAT: return VkFormat::VK_FORMAT_R16G16B16A16_SFLOAT;
		case core::PixelFormat::R32G32B32A32_FLOAT: return VkFormat::VK_FORMAT_R32G32B32A32_SFLOAT;
		case core::PixelFormat::R32G32B32_FLOAT   : return VkFormat::VK_FORMAT_R32G32B32_SFLOAT;
		case core::PixelFormat::D24_UNORM_S8_UINT : return VkFormat::VK_FORMAT_D24_UNORM_S8_UINT;
		case core::PixelFormat::R10G10B10A2_UNORM : return VkFormat::VK_FORMAT_A2R10G10B10_UNORM_PACK32;
		case core::PixelFormat::D32_FLOAT         : return VkFormat::VK_FORMAT_D32_SFLOAT;
		case core::PixelFormat::Unknown           : return VkFormat::VK_FORMAT_UNDEFINED;
		default:
			throw std::runtime_error("not supported Pixel Format type");
	}
}
#pragma region BlendState
VkBlendOp EnumConverter::Convert(const rhi::core::BlendOperator blendOperator)
{
	switch (blendOperator)
	{
		case core::BlendOperator::Add             : return VkBlendOp::VK_BLEND_OP_ADD;
		case core::BlendOperator::Subtract        : return VkBlendOp::VK_BLEND_OP_SUBTRACT;
		case core::BlendOperator::Reverse_Subtract: return VkBlendOp::VK_BLEND_OP_REVERSE_SUBTRACT;
		case core::BlendOperator::Min             : return VkBlendOp::VK_BLEND_OP_MIN;
		case core::BlendOperator::Max             : return VkBlendOp::VK_BLEND_OP_MAX;
		default:
			throw std::runtime_error("not supported Blend operator type");
	}
}
VkBlendFactor  EnumConverter::Convert(const rhi::core::BlendFactor blendFactor)
{
	switch (blendFactor)
	{
		case core::BlendFactor::Zero: return VkBlendFactor::VK_BLEND_FACTOR_ZERO;
		case core::BlendFactor::One: return VkBlendFactor::VK_BLEND_FACTOR_ONE;
		case core::BlendFactor::Source_Color: return VkBlendFactor::VK_BLEND_FACTOR_SRC_COLOR;
		case core::BlendFactor::Inverse_Source_Color: return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR;
		case core::BlendFactor::Source_Alpha: return VkBlendFactor::VK_BLEND_FACTOR_SRC_ALPHA;
		case core::BlendFactor::Inverse_Source_Alpha: return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		case core::BlendFactor::Dest_Alpha: return VkBlendFactor::VK_BLEND_FACTOR_DST_ALPHA;
		case core::BlendFactor::Inverse_Dest_Alpha: return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA;
		case core::BlendFactor::Dest_Color: return VkBlendFactor::VK_BLEND_FACTOR_DST_COLOR;
		case core::BlendFactor::Inverse_Dest_Color: return VkBlendFactor::VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR;
		default:
			throw std::runtime_error("not supported Blend Factor type");
	}
}
VkColorComponentFlags EnumConverter::Convert(const rhi::core::ColorMask colorMask)
{
	return static_cast<VkColorComponentFlags>(colorMask); // all the same
}
#pragma endregion BlendState
#pragma region RasterizerState
VkPolygonMode   EnumConverter::Convert(const rhi::core::FillMode fillMode)
{
	switch (fillMode)
	{
		case core::FillMode::Solid    : return VkPolygonMode::VK_POLYGON_MODE_FILL;
		case core::FillMode::WireFrame: return VkPolygonMode::VK_POLYGON_MODE_LINE;
		case core::FillMode::Point    : return VkPolygonMode::VK_POLYGON_MODE_POINT;
		default: 
			throw std::runtime_error("not support fillmode type");
	}
}
VkCullModeFlags EnumConverter::Convert(const rhi::core::CullingMode cullingMode)
{
	switch (cullingMode)
	{
		case core::CullingMode::None : return VkCullModeFlagBits::VK_CULL_MODE_NONE;
		case core::CullingMode::Front: return VkCullModeFlagBits::VK_CULL_MODE_FRONT_BIT;
		case core::CullingMode::Back : return VkCullModeFlagBits::VK_CULL_MODE_BACK_BIT;
		default:
			throw std::runtime_error("not support culling mode");
	}
}
VkFrontFace     EnumConverter::Convert(const rhi::core::FrontFace frontFace)
{
	switch (frontFace)
	{
		case core::FrontFace::Clockwise       : return VkFrontFace::VK_FRONT_FACE_CLOCKWISE;
		case core::FrontFace::CounterClockwise: return VkFrontFace::VK_FRONT_FACE_COUNTER_CLOCKWISE;
		default:
			throw std::runtime_error("not support front face type.");
	}
}
#pragma endregion   RasterizerState (Done)
#pragma region DepthStencilState
VkCompareOp  EnumConverter::Convert(const rhi::core::CompareOperator compareOperator)
{
	switch (compareOperator)
	{
		case core::CompareOperator::Never       : return VkCompareOp::VK_COMPARE_OP_NEVER;
		case core::CompareOperator::Less        : return VkCompareOp::VK_COMPARE_OP_LESS;
		case core::CompareOperator::Equal       : return VkCompareOp::VK_COMPARE_OP_EQUAL;
		case core::CompareOperator::LessEqual   : return VkCompareOp::VK_COMPARE_OP_LESS_OR_EQUAL;
		case core::CompareOperator::Greater     : return VkCompareOp::VK_COMPARE_OP_GREATER;
		case core::CompareOperator::NotEqual    : return VkCompareOp::VK_COMPARE_OP_NOT_EQUAL;
		case core::CompareOperator::GreaterEqual: return VkCompareOp::VK_COMPARE_OP_GREATER_OR_EQUAL;
		case core::CompareOperator::Always      : return VkCompareOp::VK_COMPARE_OP_ALWAYS;
		default:
			throw std::runtime_error("not supported compare operator type");
	}
}
VkStencilOp  EnumConverter::Convert(const rhi::core::StencilOperator stencilOperator)
{
	switch (stencilOperator)
	{
		case core::StencilOperator::Keep             : return VkStencilOp::VK_STENCIL_OP_KEEP;
		case core::StencilOperator::Zero             : return VkStencilOp::VK_STENCIL_OP_ZERO;
		case core::StencilOperator::Replace          : return VkStencilOp::VK_STENCIL_OP_REPLACE;
		case core::StencilOperator::IncrementAndClamp: return VkStencilOp::VK_STENCIL_OP_INCREMENT_AND_CLAMP;
		case core::StencilOperator::DecrementAndClamp: return VkStencilOp::VK_STENCIL_OP_DECREMENT_AND_CLAMP;
		case core::StencilOperator::Invert           : return VkStencilOp::VK_STENCIL_OP_INVERT;
		case core::StencilOperator::IncrementAndWrap : return VkStencilOp::VK_STENCIL_OP_INCREMENT_AND_WRAP;
		case core::StencilOperator::DecrementAndWrap : return VkStencilOp::VK_STENCIL_OP_DECREMENT_AND_WRAP;
		default:
			throw std::runtime_error("not supported depth stencil operator type");
	}
}
#pragma endregion DepthStencilState
#pragma region Input Layout
VkPrimitiveTopology EnumConverter::Convert(const rhi::core::PrimitiveTopology primitiveTopology)
{
	switch (primitiveTopology)
	{
		case core::PrimitiveTopology::LineList     : return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
		case core::PrimitiveTopology::LineStrip    : return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_LINE_STRIP;
		case core::PrimitiveTopology::TriangleList : return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		case core::PrimitiveTopology::TriangleStrip: return VkPrimitiveTopology::VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP;
		default:
			throw std::runtime_error("not supported primitive topology type (vulkan api) ");
	}
}
#pragma endregion Input Layput
#pragma region GPUResource
VkImageType      EnumConverter::Convert(const rhi::core::ResourceDimension dimension)
{
	switch (dimension)
	{
		case core::ResourceDimension::Dimension1D: return VkImageType::VK_IMAGE_TYPE_1D;
		case core::ResourceDimension::Dimension2D: return VkImageType::VK_IMAGE_TYPE_2D;
		case core::ResourceDimension::Dimension3D: return VkImageType::VK_IMAGE_TYPE_3D;
		default:
			throw std::runtime_error("not supported resource dimension (vulkan api) ");
	}
}
VkImageViewType  EnumConverter::Convert(const rhi::core::ResourceType type)
{
	switch (type)
	{
		case core::ResourceType::Texture1D       : return VkImageViewType::VK_IMAGE_VIEW_TYPE_1D;
		case core::ResourceType::Texture1DArray  : return VkImageViewType::VK_IMAGE_VIEW_TYPE_1D_ARRAY;
		case core::ResourceType::Texture2D       : return VkImageViewType::VK_IMAGE_VIEW_TYPE_2D;
		case core::ResourceType::Texture2DArray  : return VkImageViewType::VK_IMAGE_VIEW_TYPE_2D_ARRAY;
		case core::ResourceType::Texture3D       : return VkImageViewType::VK_IMAGE_VIEW_TYPE_3D;
		case core::ResourceType::TextureCube     : return VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE;
		case core::ResourceType::TextureCubeArray: return VkImageViewType::VK_IMAGE_VIEW_TYPE_CUBE_ARRAY;
		default:
			throw std::runtime_error("not supported resource type (vulkan api)");
	}
}
VkDescriptorType EnumConverter::Convert(const rhi::core::DescriptorHeapType heapType)
{
	switch (heapType)
	{
		case core::DescriptorHeapType::CBV    : return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		case core::DescriptorHeapType::SRV    : return VkDescriptorType::VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		case core::DescriptorHeapType::UAV    : return VkDescriptorType::VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
		case core::DescriptorHeapType::SAMPLER: return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLER;
		default:
			throw std::runtime_error("not supported heap type (vulkan api)" );
	}
}
#pragma region GPUBuffer
VkDescriptorType EnumConverter::Convert(const rhi::core::DescriptorType resourceType)
{
	switch (resourceType)
	{
		case core::DescriptorType::Buffer : return VkDescriptorType::VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		case core::DescriptorType::Texture: return VkDescriptorType::VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		default:
			throw std::runtime_error("not supported descriptor type (vulkan api)");
	}
}
VkMemoryPropertyFlags  EnumConverter::Convert(const rhi::core::MemoryHeap memoryHeap)
{
	switch (memoryHeap)
	{
		case core::MemoryHeap::Default: return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT; // not write in CPU
		case core::MemoryHeap::Upload : return VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT
			                                 | VkMemoryPropertyFlagBits::VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;
		default:
			throw std::runtime_error("not support Memory Heap type");
	}
}
#pragma endregion GPUBuffer
#pragma endregion GPUResource