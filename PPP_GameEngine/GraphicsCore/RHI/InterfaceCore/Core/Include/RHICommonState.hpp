//////////////////////////////////////////////////////////////////////////////////
///             @file   RHICommonState.hpp
///             @brief  CommonState
///             @author Toide Yutaro
///             @date   2022_06_12
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef RHI_COMMON_STATE_HPP
#define RHI_COMMON_STATE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <cstdint>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
	enum class APIVersion : std::uint8_t
	{
		Unknown    = 0,
		DirectX12  = 1,
		Vulkan     = 2
	};
#pragma region Pixel
	enum class PixelFormat
	{
		Unknown,
		R8G8B8A8_UNORM,
		B8G8R8A8_UNORM,
		R10G10B10A2_UNORM,
		R32G32B32A32_FLOAT,
		R16G16B16A16_FLOAT,
		D32_FLOAT,
		D24_UNORM_S8_UINT,
		R32G32B32_FLOAT,
		CountOfPixelFormat
	};
	class PixelFormatSizeOf
	{
	public:
		PixelFormatSizeOf() = delete;
		static size_t Get(const core::PixelFormat pixelFormat)
		{
			switch (pixelFormat)
			{
			case PixelFormat::R16G16B16A16_FLOAT:
				return 8;
			case PixelFormat::R8G8B8A8_UNORM:
			case PixelFormat::B8G8R8A8_UNORM:
			case PixelFormat::D32_FLOAT:
				return 4;
			case PixelFormat::R32G32B32A32_FLOAT:
				return 16;
			case PixelFormat::R32G32B32_FLOAT:
				return 12;
			case PixelFormat::D24_UNORM_S8_UINT:
			case PixelFormat::R10G10B10A2_UNORM:
				return 4;
			default:
				return 0;
			}
		}
		static bool IsDepthOnly(const PixelFormat pixelFormat)
		{
			switch (pixelFormat)
			{
			case PixelFormat::D32_FLOAT: return true;
			default: return false;
			}
		}
	};
#pragma endregion Pixel
#pragma region Shader Type
	enum class ShaderType : std::uint8_t
	{
		Vertex,
		Pixel,
		Compute,
		Hull,
		Domain,
		Geometry,
		Amplification,
		Mesh,
	};
	
	struct BlobData
	{
		void*  BufferPointer = nullptr;
		size_t BufferSize = 0;
		BlobData() = default;
		~BlobData() = default;
		BlobData(void* bufferPointer, size_t bufferSize) : BufferPointer(bufferPointer), BufferSize(bufferSize) {};
	};
#pragma endregion  Shader Type
#pragma region Blend State
	/****************************************************************************
	*				  			BlendFactor
	*************************************************************************//**
	*  @class     BlendFactor
	*  @brief     Color blend factor (directX12 based)
	*****************************************************************************/
	enum class BlendFactor : std::uint32_t
	{
		Zero = 1,
		One,
		Source_Color,
		Inverse_Source_Color,
		Source_Alpha,
		Inverse_Source_Alpha,
		Dest_Alpha,
		Inverse_Dest_Alpha,
		Dest_Color,
		Inverse_Dest_Color,
		Source_Alpha_Saturate,
	};
	/****************************************************************************
	*				  			BlendOperator
	*************************************************************************//**
	*  @class     BlendOperator
	*  @brief     Color blend calculate opration (directX12 based)
	*****************************************************************************/
	enum class BlendOperator : std::uint32_t
	{
		Add = 1,
		Subtract = 2,
		Reverse_Subtract = 3,
		Min = 4,
		Max = 5
	};
	/****************************************************************************
	*				  			ColorMask
	*************************************************************************//**
	*  @class     ColorMask 
	*  @brief     Color mask bit flag
	*****************************************************************************/
	enum class ColorMask : std::uint8_t
	{
		None  = 0,
		Red   = 0x1,
		Green = 0x2,
		Blue  = 0x4,
		Alpha = 0x8,
		All   = Red | Green | Blue | Alpha
	};
	inline ColorMask operator | (const ColorMask& left, const ColorMask& right)
	{
		return static_cast<ColorMask>( static_cast<std::uint32_t>(left) | static_cast<std::uint32_t>(right));
	}
	/****************************************************************************
	*				  			BlendProperty
	*************************************************************************//**
	*  @class     BlendProperty
	*  @brief     Property
	*****************************************************************************/
	struct BlendProperty
	{
		BlendOperator ColorOperator    = BlendOperator::Add;
		BlendOperator AlphaOperator    = BlendOperator::Add;
		BlendFactor   DestinationAlpha = BlendFactor::Zero;
		BlendFactor   Destination      = BlendFactor::Zero;
		BlendFactor   SourceAlpha      = BlendFactor::One;
		BlendFactor   Source           = BlendFactor::One;
		ColorMask     ColorMask        = ColorMask::All;

		bool Enable = false;
		BlendProperty() = default;
	};
#pragma endregion        Blend State
#pragma region Rasterizer State
	/****************************************************************************
	*				  			CullingMode
	*************************************************************************//**
	*  @class     CullingMode
	*  @brief     Culling mode (left hand coordinate)
	*****************************************************************************/
	enum class CullingMode : std::uint8_t
	{
		None,  // all face render
		Front, // front culling
		Back   // back culling
	};
	/****************************************************************************
	*				  			FrontFace
	*************************************************************************//**
	*  @class     FrontFace
	*  @brief     Polygon front face
	*****************************************************************************/
	enum class FrontFace : std::uint8_t
	{
		CounterClockwise, // for right hand coordinate
		Clockwise,        // for left hand coordinate
	};
	/****************************************************************************
	*				  			FillMode
	*************************************************************************//**
	*  @class     FillMode
	*  @brief     Polygon fill mode
	*****************************************************************************/
	enum class FillMode
	{
		WireFrame, 
		Solid,
	};

#pragma endregion   Rasterizer State
#pragma region DepthStencilState
	/****************************************************************************
	*				  			CompareOperator
	*************************************************************************//**
	*  @class     CompareOperator
	*  @brief     Compare operator
	*****************************************************************************/
	enum class CompareOperator : std::uint8_t
	{
		Never,
		Less,
		Equal,
		LessEqual,
		Greater,
		NotEqual,
		GreaterEqual,
		Always
	};
	/****************************************************************************
	*				  			StencilOperator
	*************************************************************************//**
	*  @class     StencilOperator
	*  @brief     Stencil operator
	*****************************************************************************/
	enum class StencilOperator : std::uint8_t
	{
		Keep,
		Zero,
		Replace,
		IncrementAndClamp,
		DecrementAndClamp,
		Invert,
		IncrementAndWrap,
		DecrementAndWrap
	};
	/****************************************************************************
	*				  			StencilOperatorInfo
	*************************************************************************//**
	*  @class     StencilOperatorInfo
	*  @brief     StencilOperatorInfo
	*****************************************************************************/
	struct StencilOperatorInfo
	{
		CompareOperator CompareOperator   = CompareOperator::Always;
		StencilOperator FailOperator      = StencilOperator::Keep;
		StencilOperator PassOperator      = StencilOperator::Keep;
		StencilOperator DepthFailOperator = StencilOperator::Keep;
		StencilOperatorInfo() = default;
	};
#pragma endregion  DepthStencilState
#pragma region InputAssemblyState
	enum class PrimitiveTopology : std::uint8_t
	{
		Undefined     = 0,
		PointList     = 1,
		LineList      = 2,
		LineStrip     = 3,
		TriangleList  = 4,
		TriangleStrip = 5,
		CountOfPrimitiveTopology
	};

	struct InputLayoutElement
	{
		PixelFormat Format = PixelFormat::Unknown;
		std::string Name = "";
		size_t      Slot = 0;
		InputLayoutElement() = default;
		explicit InputLayoutElement(const std::string& name, const PixelFormat format, const size_t slot = 0) : Format(format), Name(name), Slot(slot) {};
	};
	#pragma endregion InputAssemblyState
	enum class TextureAddressingMode : std::uint8_t
	{
		Wrap     = 1,
		Mirror   = 2,
		Clamp    = 3,
		Boarder  = 4,
	};


	/****************************************************************************
	*				  			ClearValue
	*************************************************************************//**
	*  @class     ClearValue
	*  @brief     Clear value
	*****************************************************************************/
	struct ClearValue
	{
		struct DepthClearValue
		{
			float Depth = 1.0f;
			std::uint8_t Stencil = 0;
		};
		union
		{
			float Color[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
			DepthClearValue DepthValue;
		};

		ClearValue() = default;
		ClearValue(const float* color) { for (int i = 0; i < 4; ++i) { Color[i] = color[i]; } }
		ClearValue(float depth, std::uint8_t stencil)
		{
			DepthValue.Depth = depth; DepthValue.Stencil = stencil;
		}
	};
	/****************************************************************************
	*				  			Viewport 
	*************************************************************************//**
	*  @class     Viewport 
	*  @brief     Vireport
	*****************************************************************************/
	struct Viewport
	{
		float TopLeftX  = 0.0f;
		float TopLeftY  = 0.0f;
		float Width     = 0.0f;
		float Height    = 0.0f;
		float MinDepth  = 0.0f;
		float MaxDepth  = 1.0f;
		Viewport() = default;
		Viewport(float topLeftX, float topLeftY, float width, float height, float minDepth = 0.0f, float maxDepth = 1.0f)
		{
			this->TopLeftX = topLeftX; this->TopLeftY = topLeftY; this->Width = width; this->Height = height; this->MinDepth = minDepth; this->MaxDepth = maxDepth;
		}
	};
	/****************************************************************************
	*				  			ScissorRect
	*************************************************************************//**
	*  @class     ScissorRect
	*  @brief     Scissor Rectangle
	*****************************************************************************/
	struct ScissorRect
	{
		long Left   = 0;
		long Top    = 0;
		long Right  = 0;
		long Bottom = 0;
		ScissorRect() = default;
		ScissorRect(long left, long top, long right, long bottom)
		{
			this->Left = left; this->Top = top; this->Right = right; this->Bottom = bottom;
		}
	};
	/****************************************************************************
	*				  			WindowInfo
	*************************************************************************//**
	*  @class     WindowInfo
	*  @brief     Window
	*****************************************************************************/
	struct WindowInfo
	{
		size_t Width  = 0;
		size_t Height = 0;
		void*  Handle    = nullptr;
		void*  HInstance = nullptr; // windows�p
		WindowInfo()  = default;
		WindowInfo(size_t width, size_t height, void* handle, void* hInstance = nullptr)
		{
			this->Width = width; this->Height = height; this->Handle = handle; this->HInstance = hInstance;
		}
	};
}
#endif