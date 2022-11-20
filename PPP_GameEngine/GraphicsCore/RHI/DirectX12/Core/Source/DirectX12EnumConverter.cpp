//////////////////////////////////////////////////////////////////////////////////
//              @file   DirectX12EnumConverter.cpp
///             @brief  Enum Converter
///             @author Toide Yutaro
///             @date   2022_06_29
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12EnumConverter.hpp"
#include <stdexcept>
#include <vector>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region CommandList
D3D12_COMMAND_LIST_TYPE EnumConverter::Convert(const rhi::core::CommandListType type)
{
	switch (type)
	{
		case core::CommandListType::Graphics: return D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_DIRECT;
		case core::CommandListType::Compute : return D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COMPUTE;
		case core::CommandListType::Copy    : return D3D12_COMMAND_LIST_TYPE::D3D12_COMMAND_LIST_TYPE_COPY;
		default:
			throw std::runtime_error("Not supported command list type (directX12 api)");
	}
}
#pragma endregion CommandList
#pragma region Shader
D3D12_SHADER_VISIBILITY EnumConverter::Convert(const rhi::core::ShaderVisibility visibility)
{
	switch (visibility)
	{
		case core::ShaderVisibility::All          : return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_ALL;
		case core::ShaderVisibility::Vertex       : return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_VERTEX;
		case core::ShaderVisibility::Pixel        : return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_PIXEL;
		case core::ShaderVisibility::Geometry     : return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_GEOMETRY;
		case core::ShaderVisibility::Mesh         : return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_MESH;
		case core::ShaderVisibility::Amplification: return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_AMPLIFICATION;
		case core::ShaderVisibility::Hull         : return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_HULL;
		case core::ShaderVisibility::Domain       : return D3D12_SHADER_VISIBILITY::D3D12_SHADER_VISIBILITY_DOMAIN;
		default:
			throw std::runtime_error("Not supported shader visibility (directX12 api)");
	}
}
#pragma endregion Shader
#pragma region Sampler State
D3D12_TEXTURE_ADDRESS_MODE  EnumConverter::Convert(const rhi::core::SamplerAddressMode addressingMode)
{
	return static_cast<D3D12_TEXTURE_ADDRESS_MODE>(addressingMode); // all the same
}

D3D12_STATIC_BORDER_COLOR EnumConverter::Convert(const rhi::core::BorderColor borderColor)
{
	switch (borderColor)
	{
		case core::BorderColor::TransparentBlack: return D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		case core::BorderColor::OpaqueWhite     : return D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_OPAQUE_WHITE;
		case core::BorderColor::OpaqueBlack     : return D3D12_STATIC_BORDER_COLOR::D3D12_STATIC_BORDER_COLOR_OPAQUE_BLACK;
		default:
			throw std::runtime_error("failed to convert static border color (directX12 api)");
	}
}

D3D12_FILTER EnumConverter::Convert(const rhi::core::FilterOption filter)
{
	switch (filter)
	{
		case core::FilterOption::MinPointMagPointMipPoint   : return D3D12_FILTER_MIN_MAG_MIP_POINT;
		case core::FilterOption::MinPointMagPointMipLinear  : return D3D12_FILTER_MIN_MAG_POINT_MIP_LINEAR;
		case core::FilterOption::MinPointMagLinearMipPoint  : return D3D12_FILTER_MIN_POINT_MAG_LINEAR_MIP_POINT;
		case core::FilterOption::MinPointMagLinearMipLinear : return D3D12_FILTER_MIN_POINT_MAG_MIP_LINEAR;
		case core::FilterOption::MinLinearMagPointMipPoint  : return D3D12_FILTER_MIN_LINEAR_MAG_MIP_POINT;
		case core::FilterOption::MinLinearMagPointMipLinear : return D3D12_FILTER_MIN_LINEAR_MAG_POINT_MIP_LINEAR;
		case core::FilterOption::MinLinearMagLinearMipPoint : return D3D12_FILTER_MIN_MAG_LINEAR_MIP_POINT;
		case core::FilterOption::MinLinearMagLinearMipLinear: return D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		case core::FilterOption::Anisotropy                 : return D3D12_FILTER_ANISOTROPIC;
		default:
			throw std::runtime_error("not supported filter option (directX12 api)");
	}
}
#pragma endregion Sampler State
DXGI_FORMAT  EnumConverter::Convert(const rhi::core::PixelFormat pixelFormat)
{
	switch (pixelFormat)
	{
		case core::PixelFormat::R8G8B8A8_UNORM     : return DXGI_FORMAT_R8G8B8A8_UNORM;
		case core::PixelFormat::B8G8R8A8_UNORM     : return DXGI_FORMAT_B8G8R8A8_UNORM;
		case core::PixelFormat::R16G16B16A16_FLOAT : return DXGI_FORMAT_R16G16B16A16_FLOAT;
		case core::PixelFormat::R32G32B32A32_FLOAT : return DXGI_FORMAT_R32G32B32A32_FLOAT;
		case core::PixelFormat::R32G32B32_FLOAT    : return DXGI_FORMAT_R32G32B32_FLOAT;
		case core::PixelFormat::D24_UNORM_S8_UINT  : return DXGI_FORMAT_D24_UNORM_S8_UINT;
		case core::PixelFormat::R10G10B10A2_UNORM  : return DXGI_FORMAT_R10G10B10A2_UNORM;
		case core::PixelFormat::D32_FLOAT          : return DXGI_FORMAT_D32_FLOAT;
		case core::PixelFormat::B8G8R8A8_UNORM_SRGB: return DXGI_FORMAT_B8G8R8A8_UNORM_SRGB;
		case core::PixelFormat::BC1_UNORM          : return DXGI_FORMAT_BC1_UNORM;
		case core::PixelFormat::Unknown: return DXGI_FORMAT_UNKNOWN;
		default:
			throw std::runtime_error("not supported Pixel Format type (directX12 api)");
	}
}
DXGI_FORMAT EnumConverter::Convert(const rhi::core::IndexType indexFormat)
{
	switch (indexFormat)
	{
		case core::IndexType::UInt32: return DXGI_FORMAT_R32_UINT;
		case core::IndexType::UInt16: return DXGI_FORMAT_R16_UINT;
		default:
			throw std::runtime_error("not supported Index Format type (directX12 api)");
	}
}
#pragma region BlendState
D3D12_BLEND_OP EnumConverter::Convert(const rhi::core::BlendOperator blendOperator)
{
	return static_cast<D3D12_BLEND_OP>(blendOperator); // all the same
}
D3D12_BLEND   EnumConverter::Convert(const rhi::core::BlendFactor blendFactor)
{
	return static_cast<D3D12_BLEND>(blendFactor); // all the same
}
D3D12_COLOR_WRITE_ENABLE EnumConverter::Convert(const rhi::core::ColorMask colorMask)
{
	return static_cast<D3D12_COLOR_WRITE_ENABLE>(colorMask); // all the same
}
#pragma endregion        BlendState
#pragma region RasterizerState
D3D12_FILL_MODE EnumConverter::Convert(const rhi::core::FillMode fillMode)
{
	switch (fillMode)
	{
		case core::FillMode::Solid    : return D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID;
		case core::FillMode::WireFrame: return D3D12_FILL_MODE::D3D12_FILL_MODE_WIREFRAME;
		default:
			throw std::runtime_error("not supported fill mode type (directX12 api)");
	}
}
D3D12_CULL_MODE EnumConverter::Convert(const rhi::core::CullingMode cullingMode)
{
	switch (cullingMode)
	{
		case core::CullingMode::None : return D3D12_CULL_MODE::D3D12_CULL_MODE_NONE;
		case core::CullingMode::Front: return D3D12_CULL_MODE::D3D12_CULL_MODE_FRONT;
		case core::CullingMode::Back : return D3D12_CULL_MODE::D3D12_CULL_MODE_BACK;
		default:
			throw std::runtime_error("not supported culling mode type (directX12 api)");
	}
}
bool EnumConverter::Convert(const rhi::core::FrontFace   frontFace)
{
	return frontFace == rhi::core::FrontFace::CounterClockwise;
}
#pragma endregion   RasterizerState
#pragma region DepthStencilState
D3D12_COMPARISON_FUNC EnumConverter::Convert(const core::CompareOperator op)
{
	switch (op)
	{
		case core::CompareOperator::Never       : return D3D12_COMPARISON_FUNC_NEVER;
		case core::CompareOperator::Less        : return D3D12_COMPARISON_FUNC_LESS;
		case core::CompareOperator::Equal       : return D3D12_COMPARISON_FUNC_EQUAL;
		case core::CompareOperator::LessEqual   : return D3D12_COMPARISON_FUNC_LESS_EQUAL;
		case core::CompareOperator::Greater     : return D3D12_COMPARISON_FUNC_GREATER;
		case core::CompareOperator::NotEqual    : return D3D12_COMPARISON_FUNC_NOT_EQUAL;
		case core::CompareOperator::GreaterEqual: return D3D12_COMPARISON_FUNC_GREATER_EQUAL;
		case core::CompareOperator::Always      : return D3D12_COMPARISON_FUNC_ALWAYS;
		default:
			throw std::runtime_error("not supported compare operator type (directX12 api)");
	}
}

D3D12_STENCIL_OP EnumConverter::Convert(const core::StencilOperator op)
{
	switch (op)
	{
		case core::StencilOperator::Keep             : return D3D12_STENCIL_OP_KEEP;
		case core::StencilOperator::Zero             : return D3D12_STENCIL_OP_ZERO;
		case core::StencilOperator::Replace          : return D3D12_STENCIL_OP_REPLACE;
		case core::StencilOperator::IncrementAndClamp: return D3D12_STENCIL_OP_INCR_SAT;
		case core::StencilOperator::DecrementAndClamp: return D3D12_STENCIL_OP_DECR_SAT;
		case core::StencilOperator::Invert           : return D3D12_STENCIL_OP_INVERT;
		case core::StencilOperator::IncrementAndWrap : return D3D12_STENCIL_OP_INCR;
		case core::StencilOperator::DecrementAndWrap : return D3D12_STENCIL_OP_DECR;
		default:
			throw std::runtime_error("not supported stencil operator type (directX12 api)");
	}
}
#pragma endregion DepthStencilState
#pragma region Input Layout
D3D_PRIMITIVE_TOPOLOGY  EnumConverter::Convert(const rhi::core::PrimitiveTopology primitiveTopology)
{
	return static_cast<D3D_PRIMITIVE_TOPOLOGY>(primitiveTopology);
}
D3D12_PRIMITIVE_TOPOLOGY_TYPE EnumConverter::Convert1(const rhi::core::PrimitiveTopology primitiveTopology)
{
	switch (primitiveTopology)
	{
		case core::PrimitiveTopology::PointList    : return D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_POINT;
		case core::PrimitiveTopology::LineList     : return D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		case core::PrimitiveTopology::LineStrip    : return D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE;
		case core::PrimitiveTopology::TriangleList : return D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		case core::PrimitiveTopology::TriangleStrip: return D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
		default:
			throw std::runtime_error("not supported primitive topology type (directX12 api)");
	}
}
DXGI_FORMAT EnumConverter::Convert(const core::InputFormat inputFormat)
{
	switch (inputFormat)
	{
		case core::InputFormat::R32G32_FLOAT      : return DXGI_FORMAT::DXGI_FORMAT_R32G32_FLOAT;
		case core::InputFormat::R32G32B32_FLOAT   : return DXGI_FORMAT::DXGI_FORMAT_R32G32B32_FLOAT;
		case core::InputFormat::R32G32B32A32_FLOAT: return DXGI_FORMAT::DXGI_FORMAT_R32G32B32A32_FLOAT;
		case core::InputFormat::R32_FLOAT         : return DXGI_FORMAT::DXGI_FORMAT_R32_FLOAT;
		default:
			throw std::runtime_error("not supported input format");
	}
}
#pragma endregion      Input Layput
#pragma region GPUResource
D3D12_RESOURCE_FLAGS EnumConverter::Convert(const rhi::core::ResourceUsage usage)
{
	static std::vector<core::ResourceUsage> sourcePool = {
		core::ResourceUsage::None,
		core::ResourceUsage::VertexBuffer,
		core::ResourceUsage::IndexBuffer,
		core::ResourceUsage::ConstantBuffer,
		core::ResourceUsage::RenderTarget,
		core::ResourceUsage::DepthStencil,
		core::ResourceUsage::UnorderedAccess
	};
	static std::vector<D3D12_RESOURCE_FLAGS> targetPool =
	{
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_NONE,
		D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET,
		D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL,
		D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS,
	};

	auto res = D3D12_RESOURCE_FLAG_NONE;

	for (size_t index = 0; index < sourcePool.size(); index++)
	{
		if (core::EnumHas(usage, sourcePool[index]))
			res = res | targetPool[index];
	}

	return res;
}
D3D12_RESOURCE_DIMENSION   EnumConverter::Convert(const rhi::core::ResourceDimension dimension)
{
	switch (dimension)
	{
		case core::ResourceDimension::Dimension1D: return D3D12_RESOURCE_DIMENSION_TEXTURE1D;
		case core::ResourceDimension::Dimension2D: return D3D12_RESOURCE_DIMENSION_TEXTURE2D;
		case core::ResourceDimension::Dimension3D: return D3D12_RESOURCE_DIMENSION_TEXTURE3D;
		case core::ResourceDimension::Buffer: return D3D12_RESOURCE_DIMENSION_BUFFER;
		default:
			throw std::runtime_error("not supported resource dimension (directX12 api)");
	}
}
D3D12_SRV_DIMENSION        EnumConverter::Convert(const rhi::core::ResourceType type)
{
	return static_cast<D3D12_SRV_DIMENSION>(type);
}
D3D12_DESCRIPTOR_HEAP_TYPE EnumConverter::Convert(const rhi::core::DescriptorHeapType heapType)
{
	switch (heapType)
	{
		case core::DescriptorHeapType::CBV:
		case core::DescriptorHeapType::SRV:
		case core::DescriptorHeapType::UAV        : return D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		case core::DescriptorHeapType::SAMPLER    : return D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
		case core::DescriptorHeapType::RTV        : return D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		case core::DescriptorHeapType::DSV        : return D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
		default:
			throw std::runtime_error("not supported descriptor heap type (directX12 api)");
	}
}
D3D12_DESCRIPTOR_RANGE_TYPE EnumConverter::Convert1(const rhi::core::DescriptorHeapType descriptorType)
{
	switch (descriptorType)
	{
		case core::DescriptorHeapType::CBV: return D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		case core::DescriptorHeapType::SRV: return D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		case core::DescriptorHeapType::UAV: return D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		case core::DescriptorHeapType::SAMPLER: return D3D12_DESCRIPTOR_RANGE_TYPE::D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER;
		default:
			throw std::runtime_error("not supported descriptor range type (directX12 api)");
	}
}
#pragma region GPUBuffer
D3D12_HEAP_TYPE  EnumConverter::Convert(const rhi::core::MemoryHeap memoryHeap)
{
	switch (memoryHeap)
	{
		case core::MemoryHeap::Default: return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_DEFAULT;
		case core::MemoryHeap::Upload : return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD;
		case core::MemoryHeap::Custom : return D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_CUSTOM;
		default:
			throw std::runtime_error("not supported heap type (directX12 api)");
	}
}
D3D12_RESOURCE_STATES EnumConverter::Convert(const rhi::core::ResourceState resourceState)
{
	static D3D12_RESOURCE_STATES states[] = {
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COMMON,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_GENERIC_READ,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_INDEX_BUFFER,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_UNORDERED_ACCESS,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RENDER_TARGET,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_DEPTH_WRITE,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_DEST,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_COPY_SOURCE,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_PRESENT,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_INDIRECT_ARGUMENT,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_RAYTRACING_ACCELERATION_STRUCTURE,
		D3D12_RESOURCE_STATES::D3D12_RESOURCE_STATE_SHADING_RATE_SOURCE
	};

	return states[(int)resourceState];
}
#pragma endregion GPUBuffer
#pragma endregion       GPUResource
#pragma region Render Pass
D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE EnumConverter::Convert(const rhi::core::AttachmentLoad op)
{
	switch (op)
	{
		case core::AttachmentLoad::Load    : return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_PRESERVE;
		case core::AttachmentLoad::Clear   : return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_CLEAR;
		case core::AttachmentLoad::DontCare: return D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE_DISCARD;
		default: 
			throw std::runtime_error("not support render pass beginning type (directX12 api)");
	}
}
D3D12_RENDER_PASS_ENDING_ACCESS_TYPE    EnumConverter::Convert(const rhi::core::AttachmentStore op)
{
	switch (op)
	{
		case core::AttachmentStore::Store   : return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_PRESERVE;
		case core::AttachmentStore::DontCare: return D3D12_RENDER_PASS_ENDING_ACCESS_TYPE_DISCARD;
		default:
			throw std::runtime_error("not support render pass ending type (directX12 api)");
	}
}
#pragma endregion Render Pass