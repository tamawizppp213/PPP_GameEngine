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
#include "GameUtility/Base/Include/GUString.hpp"
#include "GameUtility/Base/Include/GUEnumClassFlags.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
#pragma warning(disable: 4061)
//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::core
{
#pragma region API
	/****************************************************************************
	*				  			APIVersion
	*************************************************************************//**
	*  @enum      APIVersion
	* 
	*  @brief     Graphics api version (Add as needed.)
	*****************************************************************************/
	enum class APIVersion : gu::uint8
	{
		Unknown    = 0,
		DirectX12  = 1,
		Vulkan     = 2
	};
#pragma endregion           API
#pragma region CommandList
	/****************************************************************************
	*				  			CommandListType
	*************************************************************************//**
	*  @enum      CommandListType
	*  @brief     Command list (graphics, compute, or copy)
	*****************************************************************************/
	enum class CommandListType
	{
		Unknown,    // For Initialize
		Graphics,   // Graphics command list (directX12 api includes all command list type (use this) )
		Compute,    // Compute command list. This type is used to async compute command
		Copy,       // Copy command list
		CountOfType
	};
#pragma endregion   CommandList
#pragma region Index
	/****************************************************************************
	*				  			IndexType
	*************************************************************************//**
	*  @enum      IndexType
	*  @brief     UINT 32 or 16. This value shows byte count.
	*****************************************************************************/
	enum class IndexType
	{
		UInt32 = 4,
		UInt16 = 2,
	};
	/****************************************************************************
	*				  			InputFormat
	*************************************************************************//**
	*  @enum      InputFormat
	*  @brief     Input layout format.  
	*****************************************************************************/
	enum class InputFormat
	{
		Unknown,
		R32_FLOAT,          // 4  byte format
		R32G32_FLOAT,       // 8  byte format
		R32G32B32_FLOAT,    // 12 byte format
		R32G32B32A32_FLOAT, // 16 byte format
		R32G32B32A32_INT,   // 16 byte format
	};
	class InputFormatSizeOf
	{
	public :
		InputFormatSizeOf() = delete;
		static size_t Get(const core::InputFormat inputFormat)
		{
			switch (inputFormat)
			{
				case InputFormat::R32G32_FLOAT      : return 8;
				case InputFormat::R32G32B32_FLOAT   : return 12;
				case InputFormat::R32G32B32A32_INT  :
				case InputFormat::R32G32B32A32_FLOAT: return 16;
				case InputFormat::R32_FLOAT         : return 4;
				default: return 0;
			}
		}
	};
#pragma endregion         Index
	#pragma region Window Surface
	/****************************************************************************
	*				  			Viewport 
	*************************************************************************//**
	*  @class     Viewport 
	*  @brief     Rect Viewport 
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
		long Left   = 0; // Left window position
		long Top    = 0; // top window position
		long Right  = 0; // right window position
		long Bottom = 0; // bottom window position
		ScissorRect() = default;
		ScissorRect(long left, long top, long right, long bottom)
		{
			this->Left = left; this->Top = top; this->Right = right; this->Bottom = bottom;
		}
	};

	/****************************************************************************
	*				  			WindowInfo
	*************************************************************************//**
	*  @struct    WindowInfo
	*  @brief     Window size and window handle pointer
	*****************************************************************************/
	struct WindowInfo
	{
		size_t Width     = 0;       // window width
		size_t Height    = 0;       // window height
		void*  Handle    = nullptr; // window handle pointer 
		void*  HInstance = nullptr; // window instance for Windows API

		WindowInfo()  = default;

		WindowInfo(size_t width, size_t height, void* handle, void* hInstance = nullptr)
		{
			this->Width = width; this->Height = height; this->Handle = handle; this->HInstance = hInstance;
		}

	};

#pragma endregion    Window Surface
#pragma region Pixel

	enum class ShadingRate
	{
		K_1x1,
		K_1x2,
		K_2x1,
		K_2x2,
		K_2x4,
		K_4x2,
		K_4x4
	};

	/****************************************************************************
	*				  			DisplayOutputFormat
	*************************************************************************//**
	*  @enum      DisplayOutputFormat
	*  @brief     Color format
	*****************************************************************************/
	enum class DisplayOutputFormat
	{
		SDR_SRGB,
		SDR_Rec709,
		HDR_ACES_1000nit_ST2084,
		HDR_ACES_2000nit_ST2084,
		HDR_ACES_1000nit_ScRGB,
		HDR_ACES_2000nit_ScRGB,
		HDR_Linear_NoToneCurve,
		HDR_Linear_WithToneCurve
	};

	/****************************************************************************
	*				  			DisplayColorGamut
	*************************************************************************//**
	*  @enum      DisplayColorGamut
	*  @brief     Color range
	*             https://uwatechnologies.hatenablog.com/entry/2022/04/09/001938
	* 　　　　　 　　　https://garagefarm.net/jp-blog/what-is-color-space-and-why-you-should-use-aces
	*             https://qiita.com/UWATechnology/items/2a40dbc66bf48041d405
	*****************************************************************************/
	enum class DisplayColorGamut
	{
		SRGB_D65,    // srgb color format    + white point D65 (Windows標準色域)
		DCIP3_D65,   // dcpi3   color format + white point D65 (映像撮影に使われるカラーフィルムの色域に対応した広範囲の色域を表現できる規格)
		Rec2020_D65, // rec2020 color format + white point D65 (HDR用に使われる色域)
		ACES_D60,    // aces    color format + white point D60 (ダイナミックレンジが広い)  
		ACEScg_D60   // aces cg color format 
	};

	/****************************************************************************
	*				  			HDRDisplayInfo
	*************************************************************************//**
	*  @enum      HDRDisplayInfo
	*  @brief     HDR display settings (RHIDeviceにて設定を行います)
	*             https://qiita.com/dgtanaka/items/672d2e7b3152f4e5ed49
	*****************************************************************************/
	struct HDRDisplayInfo
	{
		DisplayColorGamut   ColorGamut    = DisplayColorGamut::SRGB_D65;
		DisplayOutputFormat DisplayFormat = DisplayOutputFormat::SDR_SRGB;
		float RedPrimary[2]         = {0,0}; // red   xy coordinate in the color space
		float GreenPrimary[2]       = {0,0}; // green xy coordinate in the color space
		float BluePrimary[2]        = {0,0}; // blue  xy coordinate in the color space
		float WhitePoint[2]         = {0,0}; // white xy coordinate in the color space
		float MinLuminance          = 0.0f;  // nits
		float MaxLuminacnce         = 0.0f;  // nits
		float MaxFullFrameLuminance = 0.0f;
		ScissorRect Rect = {};
	};


	/****************************************************************************
	*				  			PixelFormat
	*************************************************************************//**
	*  @class     PixelFormat
	*  @brief     Render pixel format 
	*****************************************************************************/
	enum class PixelFormat
	{
		Unknown,  

		// RGBA format
		R32G32B32A32_FLOAT,
		R16G16B16A16_FLOAT, 
		R8G8B8A8_UNORM,
		B8G8R8A8_UNORM,

		// RGB format
		R32G32B32_FLOAT,
		R16G16B16_FLOAT,

		// RG format
		R32G32_FLOAT,

		// Single format
		D32_FLOAT,
		R32_FLOAT,
		R32_SINT,
		R32_UINT,
		R16_FLOAT,
		R16_SINT,
		R16_UINT,
		R8_SINT,
		R8_UINT,

		// Block compression format (画像を4x4ピクセル単位のブロックに分割して, それぞれのブロックごとに圧縮を行う方式)
		// https://www.webtech.co.jp/blog/optpix_labs/format/6993/
		BC1_UNORM,   // 8byte : RGB, RGBA画像
		BC2_UNORM,   // 16byte:RGBA(16諧調アルファ)
		BC3_UNORM,   // 16byte:RGBA(多階調アルファ)
		BC4_UNORM,   // 8byte :1成分のデータ (ハイトマップ等)
		BC5_UNORM,   // 16byte:2成分のデータ (法線マップ等)
		BC7_UNORM,   // 16byte:HDRのRGB画像
		BC6H_UNORM,  // 16byte:RGB画像, RGBA画像 (多階調アルファ)

		R10G10B10A2_UNORM,
		D24_UNORM_S8_UINT,
		B8G8R8A8_UNORM_SRGB,
		CountOfPixelFormat
	};

	/****************************************************************************
	*				  			PixelFormatSizeOf
	*************************************************************************//**
	*  @class     PixelFormatSizeOf
	*  @brief     Get pixel size. Call the static function "Get"
	*****************************************************************************/
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
			case PixelFormat::BC1_UNORM:
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
				case PixelFormat::D32_FLOAT: 
				{
					return true;
				}
				default: 
				{
					return false;
				}
			}
		}
	};
	/****************************************************************************
	*				  			ClearValue
	*************************************************************************//**
	*  @class     ClearValue
	*  @brief     Clear value 
	*             Pixel用とDepth, Stencil用は必ず分けてClearValueを作成してください. 
	*****************************************************************************/
	struct ClearValue
	{
		enum ColorType { Red, Green, Blue, Alpha };
		gu::float32        Color[4] = { 1.0f, 1.0f, 1.0f, 1.0f }; 
		gu::float32        Depth    = 1.0f;
		gu::uint8 Stencil  = 0;

		ClearValue() = default;
		ClearValue(float red, float green, float blue, float alpha)
		{
			Color[0] = red; Color[1] = green; Color[2] = blue; Color[3] = alpha;
		};
		
		explicit ClearValue(float depth, gu::uint8 stencil = 0)
		{
			Depth = depth; Stencil = stencil; 
		}
	};
#pragma endregion         Pixel
#pragma region Shader
	/****************************************************************************
	*				  			ShaderVisibleFlag
	*************************************************************************//**
	*  @enum      ShaderVisibleFlag
	*  @brief     Visible shader stage type
	*****************************************************************************/
	enum class ShaderVisibleFlag : gu::int16
	{
		// directX12に準拠しています D3D12_ROOT_SIGNATURE_FLAGS
		Vertex        = 0x0002, // vertex   shader stage only
		Hull          = 0x0004, // hull     shader stage only
		Domain        = 0x0008, // domain   shader stage only
		Geometry      = 0x0010, // geometry shader stage only
		Pixel         = 0x0020, // pixel    shader stage only
		Amplification = 0x0100, // amplification shader stage only,
		Mesh          = 0x0200, // mesh shader stage only
		All = Vertex | Pixel | Hull | Domain | Geometry | Amplification | Mesh, // all shader stage visible
		CountOfPipeline = 7
	};

	__forceinline ShaderVisibleFlag operator | (const ShaderVisibleFlag& left, const ShaderVisibleFlag& right)
	{
		return static_cast<ShaderVisibleFlag>(static_cast<gu::uint8>(left) | static_cast<gu::uint8>(right));
	}
	__forceinline ShaderVisibleFlag operator & (const ShaderVisibleFlag& left, const ShaderVisibleFlag& right)
	{
		return static_cast<ShaderVisibleFlag>(static_cast<gu::uint8>(left) & static_cast<gu::uint8>(right));
	}

	__forceinline bool EnumHas(const ShaderVisibleFlag& left, const ShaderVisibleFlag& right)
	{
		return (left & right) == right;
	}

	/****************************************************************************
	*				  			ShaderType
	*************************************************************************//**
	*  @enum      ShaderType
	*  @brief     Shader type
	*****************************************************************************/
	enum class ShaderType
	{
		Vertex,           // vertex shader 
		Pixel,            // pixel  shader
		Compute,          // compute shader
		Hull,             // hull shader
		Domain,           // domain shader
		Geometry,         // geometry shader
		Amplification,    // amplication shader
		Mesh,             // mesh shader
		RayGeneration,    // ray tracing ray generation shader
		Intersection,     // ray tracing intersection shader
		AnyHit,           // ray tracing any hit shader
		ClosestHit,       // ray tracing closest hit shader
		Miss              // ray tracing miss shader
	};
	
	struct BlobData
	{
		void*  BufferPointer = nullptr;
		size_t BufferSize = 0;
		BlobData() = default;
		~BlobData() = default;
		BlobData(void* bufferPointer, size_t bufferSize) : BufferPointer(bufferPointer), BufferSize(bufferSize) {};
	};
#pragma endregion        Shader Type
#pragma region Sampler State
	/****************************************************************************
	*				  			SamplerAddressMode
	*************************************************************************//**
	*  @class     SamplerAddressMode
	*  @brief     Texture addressing mode // reference : https://learn.microsoft.com/ja-jp/windows/uwp/graphics-concepts/texture-addressing-modes
	*****************************************************************************/
	enum class SamplerAddressMode : gu::uint8
	{
		Wrap    = 1, // repeat texture pattern
		Mirror  = 2, // mirror and repeat texture pattern
		Clamp   = 3, // cut over 1.0 and below 0.0
		Border  = 4, // set border color 
	};

	/****************************************************************************
	*				  			BorderColor
	*************************************************************************//**
	*  @class     BorderColor
	*  @brief     Specifies the border color for a sampler
	*****************************************************************************/
	enum class BorderColor : gu::uint8
	{
		TransparentBlack, // Indicates black, with the alpha component as fully transparent
		OpaqueBlack,      // Indicates black, with the alpha component as fully opaque(完全不透明)
		OpaqueWhite       // Indicates white, with the alpha component as fully opaque
	};

	/****************************************************************************
	*				  			FilterMask
	*************************************************************************//**
	*  @class     FilterMask
	*  @brief     Sample mask
	*****************************************************************************/
	enum class FilterMask : gu::uint8
	{
		Mip = 0x1,
		Mag = 0x2,
		Min = 0x4
	};

	/****************************************************************************
	*				  			FilterOption
	*************************************************************************//**
	*  @class     FilterOption
	*  @brief     Sampling filter option. linear -> linear sampling, point -> point sampling
	*****************************************************************************/
	enum class FilterOption : gu::uint8
	{
		MinPointMagPointMipPoint    = 0,
		MinPointMagPointMipLinear   = 1,
		MinPointMagLinearMipPoint   = 2,
		MinPointMagLinearMipLinear  = 3,
		MinLinearMagPointMipPoint   = 4,
		MinLinearMagPointMipLinear  = 5,
		MinLinearMagLinearMipPoint  = 6,
		MinLinearMagLinearMipLinear = 7,
		Anisotropy = 8
	};

	/****************************************************************************
	*				  			Default Sampler Type
	*************************************************************************//**
	*  @enum      DefaultSamplerType
	*  @brief     Default Sampler state (Point, Linear or Anisotropic + Wrap or Clamp)
	*****************************************************************************/
	enum DefaultSamplerType
	{
		SamplerPointWrap,         // Point  + Wrap
		SamplerPointClamp,        // Point  + Clamp
		SamplerLinearWrap,        // Linear + Wrap
		SamplerLinearClamp,       // Linear + Clamp
		SamplerLinearBorder,      /// Linear + Border
		SamplerAnisotropicWrap,   // Anisotropic + Wrap
		SamplerAnisotropicClamp   // Anisotropic + Clamp
	};

	/****************************************************************************
	*				  			SamplerInfo
	*************************************************************************//**
	*  @class     SamplerInfo
	*  @brief     Custum sampler state. (Normally you should use default sampler type, and call GetDefaultSampler)
	*****************************************************************************/
	struct SamplerInfo
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		FilterOption        Filter        = FilterOption::MinPointMagPointMipPoint; // Specify sampling method for image enlargement/shirinkage*
		SamplerAddressMode  AddressModeU  = SamplerAddressMode::Wrap;               // Texture addressing mode in the U direction
		SamplerAddressMode  AddressModeV  = SamplerAddressMode::Wrap;               // Texture addressing mode in the V direction
		SamplerAddressMode  AddressModeW  = SamplerAddressMode::Wrap;               // Texture addressing mode in the W direction
		BorderColor         Border        = BorderColor::TransparentBlack;          // Border color 
		size_t              MaxAnisotropy = 1;                                      // Max anisotropy
		gu::float32         MipLODBias    = 0.0f;                                   // Defined LOD = normalLOD + bias
		gu::float32         MinLOD        = 0.0f;                                   // Min LOD size
		gu::float32         MaxLOD        = MAX_FLOAT32;                            // Max LOD size: FLT_MAX 上限を指定しない.

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		SamplerInfo() = default;
		explicit SamplerInfo(
			const FilterOption filter,
			const SamplerAddressMode addressU = SamplerAddressMode::Wrap,
			const SamplerAddressMode addressV = SamplerAddressMode::Wrap,
			const SamplerAddressMode addressW = SamplerAddressMode::Wrap,
			const BorderColor border = BorderColor::TransparentBlack,
			float minLOD = 0.0f,
			float maxLOD = MAX_FLOAT32,
			float mipLODBias = 0.0f) :
			Filter(filter),
			AddressModeU(addressU),
			AddressModeV(addressV),
			AddressModeW(addressW),
			Border(border),
			MipLODBias(mipLODBias),
			MinLOD(minLOD),
			MaxLOD(maxLOD)
		{
		}

		explicit SamplerInfo(
			const gu::uint32 maxAnisotropy,
			const SamplerAddressMode addressU = SamplerAddressMode::Wrap,
			const SamplerAddressMode addressV = SamplerAddressMode::Wrap,
			const SamplerAddressMode addressW = SamplerAddressMode::Wrap,
			const BorderColor border = BorderColor::TransparentBlack) :
			Filter(FilterOption::Anisotropy),
			AddressModeU(addressU),
			AddressModeV(addressV),
			AddressModeW(addressW),
			MaxAnisotropy(maxAnisotropy),
			Border(border)
		{
		}

		static SamplerInfo GetDefaultSampler(DefaultSamplerType type);
	};

#pragma endregion Sampler State
#pragma region Blend State
	/****************************************************************************
	*				  			BlendFactor
	*************************************************************************//**
	*  @class     BlendFactor
	*  @brief     Color blend factor (directX12 based)
	*             Source : Color to be rendered from now on
	*             Dest   : Color of rendering destination
	*****************************************************************************/
	enum class BlendFactor : gu::uint8
	{
		Zero = 1,             // * 0
		One,                  // * 1
		Source_Color,         // * Rs, *Gs, *Bs, *As
		Inverse_Source_Color, // * (1 - Rs), *(1 - Gs), *(1 - Bs), *(1 - As)
		Source_Alpha,         // * As
		Inverse_Source_Alpha, // * (1 - As)
		Dest_Alpha,           // * Ad
		Inverse_Dest_Alpha,   // * (1 - Ad)
		Dest_Color,           // * Rd, *Gd, *Bd, *Ad
		Inverse_Dest_Color,   // * (1 - Rd), *(1 - Gd), *(1 - Bd), *(1 - Ad)
		Source_Alpha_Saturate,// * mih(1 - Ad, As)
	};

	/****************************************************************************
	*				  			BlendOperator
	*************************************************************************//**
	*  @class     BlendOperator
	*  @brief     Color blend calculate opration (directX12 based)
	*****************************************************************************/
	enum class BlendOperator : gu::uint8
	{
		Add              = 1, // Default color blend operator : destination + source
		Subtract         = 2, // source - destination
		Reverse_Subtract = 3, // destination - source
		Min              = 4, // min(destination, source)
		Max              = 5  // max(destination, source)
	};

	/****************************************************************************
	*				  			ColorMask
	*************************************************************************//**
	*  @class     ColorMask 
	*  @brief     Color mask bit flag
	*****************************************************************************/
	enum class ColorMask : gu::uint8
	{
		None  = 0,   // All Disable 
		Red   = 0x1, // Red WriteEnable
		Green = 0x2, // Green WriteEnable
		Blue  = 0x4, // Blue WriteEnable
		Alpha = 0x8, // Alpha WriteEnable
		RGB   = Red | Green | Blue,
		RG    = Red | Green,
		BA    = Blue | Green,
		All   = Red | Green | Blue | Alpha // AllEnable
	};

	inline ColorMask operator | (const ColorMask& left, const ColorMask& right)
	{
		return static_cast<ColorMask>( static_cast<gu::uint8>(left) | static_cast<gu::uint8>(right));
	}

	/****************************************************************************
	*				  			BlendProperty
	*************************************************************************//**
	*  @class     BlendProperty
	*  @brief     Property
	*             Source      : これからレンダリングする色 (ピクセルシェーダー)
	*             Destination : レンダリング先 (レンダーターゲット)
	*****************************************************************************/
	struct BlendProperty
	{
		BlendOperator ColorOperator    = BlendOperator::Add;   // RGB Color Blend Type 
		BlendOperator AlphaOperator    = BlendOperator::Add;   // Alpha Blend Type
		BlendFactor   DestinationAlpha = BlendFactor::Zero;    // Multiply to Render Target alpha(a) element blend mode
		BlendFactor   DestinationRGB   = BlendFactor::Zero;    // Multiply to Render Target color(rgb) element blend mode
		BlendFactor   SourceAlpha      = BlendFactor::One;     // Multiply to Pixel Shader alpha element(a) blend mode
		BlendFactor   SourceRGB        = BlendFactor::One;     // Multiply to Pixel Shader color element(rgb) blend mode
		ColorMask     ColorMask        = ColorMask::All;       // color mask
		bool AlphaToConverageEnable    = false;                // Multi sample時に使用する. ピクセルシェーダーから出力されたあrぷふぁを取得し, Multi sanling aliasingを適用する

		bool Enable = false; // レンダーターゲットの独立したブレンドを使用するかを設定する

		BlendProperty() = default;

		BlendProperty(BlendOperator colorOperator, BlendOperator alphaOperator, BlendFactor destAlpha, BlendFactor dest,
			BlendFactor srcAlpha, BlendFactor src, core::ColorMask colorMask = ColorMask::All, bool alphaToConverageEnable = false, bool enable = false) :
			ColorOperator(colorOperator), AlphaOperator(alphaOperator), DestinationAlpha(destAlpha), DestinationRGB(dest), SourceAlpha(srcAlpha),
			SourceRGB(src), ColorMask(colorMask), Enable(enable), AlphaToConverageEnable(alphaToConverageEnable) { };
		
		/*----------------------------------------------------------------------
		*  @brief : NoColorWrite (Display the render target as it is)
		/*----------------------------------------------------------------------*/
		static BlendProperty NoColorWrite(const bool useAlphaToCoverage = false);

		/*----------------------------------------------------------------------
		*  @brief : OverWrite (Displays the source as it is.)
		/*----------------------------------------------------------------------*/
		static BlendProperty OverWrite (const bool useAlphaToCoverage = false);

		/*----------------------------------------------------------------------
		*  @brief : Alpha blending : destination * (1 - source.Alpha) + source * 1
		/*----------------------------------------------------------------------*/
		static BlendProperty AlphaBlend(const bool useAlphaToCoverage = false);
	};

#pragma endregion       Blend   State
#pragma region Rasterizer State
	/****************************************************************************
	*				  			MultiSample
	*************************************************************************//**
	*  @class     MultiSample
	*  @brief     Basically use count1 , in use MSAA, we use count4 
	*****************************************************************************/
	enum class MultiSample : gu::uint8
	{
		Count1 = 1,       // 1
		Count2 = 2,       // 2
		Count4 = 4,       // 4
		Count8 = 8,       // 8
		Count16 = 16,     // 16
		Count32 = 32,     // 32
		Count64 = 64      // 64
	};

	class MultiSampleSizeOf
	{
	public: 
		MultiSampleSizeOf() = default;
		static size_t Get(const MultiSample sample) { return static_cast<size_t>(sample); }
	};

	/****************************************************************************
	*				  			CullingMode
	*************************************************************************//**
	*  @enum      CullingMode
	*  @brief     Sets the triangle facing the specified direction 
	              not to be drawn in the left hand coordinate
	*****************************************************************************/
	enum class CullingMode : gu::uint8
	{
		None,  // all face render
		Front, // front culling
		Back   // back  culling
	};

	/****************************************************************************
	*				  			FrontFace
	*************************************************************************//**
	*  @class     FrontFace
	*  @brief     Determine if all sides are counterclockwise
	*****************************************************************************/
	enum class FrontFace : gu::uint8
	{
		CounterClockwise, // for right hand coordinate
		Clockwise,        // for left  hand coordinate
	};

	/****************************************************************************
	*				  			FillMode
	*************************************************************************//**
	*  @class     FillMode
	*  @brief     Polygon fill mode
	*****************************************************************************/
	enum class FillMode
	{
		WireFrame, // wire frame polygon 
		Solid,     // fill polygon face
		Point,     // point cloud. (only vulkan API)
	};

	/****************************************************************************
	*				  			RasterizerProperty
	*************************************************************************//**
	*  @class     RasterizerProperty
	*  @brief     RasterizerProperty
	*****************************************************************************/
	struct RasterizerProperty
	{
		FrontFace   FaceType              = FrontFace::Clockwise; //  Determine if all sides are counterclockwise
		CullingMode CullingType           = CullingMode::None;    // Sets the triangle facing the specified direction not to be drawn in the left hand coordinate
		FillMode    FillType              = FillMode::Solid;      // Polygon fill mode
		bool        UseDepthClamp         = true;                 // use clipping base on the depth length.
		bool        UseMultiSample        = true;
		bool        UseAntiAliasLine      = false;                // DirectX12 only use
		bool        UseConservativeRaster = false;                // Perform rasterization if it hangs on a pixel for even a second.

		// How to calculate : https://learn.microsoft.com/ja-jp/windows/win32/direct3d11/d3d10-graphics-programming-guide-output-merger-stage-depth-bias?redirectedfrom=MSDN
		// Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope;
		float  DepthBias           = 0.0f;  // Depth value added to a given pixel.
		float  SlopeScaleDepthBias = 0.0f;  // Bias = (float)DepthBias * r + SlopeScaledDepthBias * MaxDepthSlope;
		float  ClampMaxDepthBias   = 0.0f;  // Clamp MaxDepth

		RasterizerProperty() = default;

		RasterizerProperty
		(
			const FrontFace   frontFace,
			const CullingMode cullingMode,
			const FillMode    fillMode,
			const bool  useDepthClamp,
			const bool  useMultiSample,
			const float depthBias           = 0.0f,
			const float slopeScaleDepthBias = 0.0f,
			const float clampMaxDepthBias   = 0.0f
		) :
			FaceType(frontFace), CullingType(cullingMode), FillType(fillMode), 
			UseDepthClamp(useDepthClamp), UseMultiSample(useMultiSample),
			DepthBias(depthBias), SlopeScaleDepthBias(slopeScaleDepthBias), 
			ClampMaxDepthBias(clampMaxDepthBias)
		{
		};

		/*----------------------------------------------------------------------
		*  @brief : fill polygon face rasterize mode
		/*----------------------------------------------------------------------*/
		static RasterizerProperty Solid
		(
			const bool        useMultiSample      = false, 
			const FrontFace   frontFace           = FrontFace::Clockwise, 
			const CullingMode cullingMode         = CullingMode::None,
			const float       depthBias           = 0.0f,
			const float       slopeScaleDepthBias = 0.0f,
			const float       clampMaxDepthBias   = 0.0f
		);
		
		/*----------------------------------------------------------------------
		*  @brief : wire frame polygon rasterize mode
		/*----------------------------------------------------------------------*/
		static RasterizerProperty WireFrame
		(
			const bool        useMultiSample      = false, 
			const FrontFace   frontFace           = FrontFace::Clockwise, 
			const CullingMode cullingMode         = CullingMode::None,
			const float       depthBias           = 0.0f,
			const float       slopeScaleDepthBias = 0.0f,
			const float       clampMaxDepthBias   = 0.0f
		);
	};
#pragma endregion   Rasterizer State
#pragma region DepthStencilState
	/****************************************************************************
	*				  			CompareOperator
	*************************************************************************//**
	*  @enum      CompareOperator
	*  @brief     Compare operator
	*****************************************************************************/
	enum class CompareOperator : gu::uint8
	{
		Never,          // Always false
		Less,           // reference < test
		Equal,          // reference = test
		LessEqual,      // reference <= test
		Greater,        // reference > test
		NotEqual,       // reference not equal test
		GreaterEqual,   // reference >= test
		Always          // Always true
	};

	/****************************************************************************
	*				  			StencilOperator
	*************************************************************************//**
	*  @class     StencilOperator
	*  @brief     Stencil operator (設定したテストが失敗, または成功した場合に格納されたステンシル値に何が起こるかを指定する) 
	*             良い感じの説明: https://www.asawicki.info/news_1654_stencil_test_explained_using_code
	*****************************************************************************/
	enum class StencilOperator : gu::uint8
	{
		Keep,                 // keep the current value
		Zero,                 // set the value to 0
		Replace,              // sets the value to reference value.
		IncrementAndClamp,    // increments the current value and clamps to the maximum representable unsigned value
		DecrementAndClamp,    // decrement the current value and clamps to 0
		Invert,               // bitwise-inverts the current value
		IncrementAndWrap,     // increment the current value and wrap to 0 when the maximum value would have been exceeded
		DecrementAndWrap      // decrements the current value and wrap to the maximum possible value when the value wourld bo below 0
	};

	/****************************************************************************
	*				  			StencilOperatorInfo
	*************************************************************************//**
	*  @class     StencilOperatorInfo
	*  @brief     StencilOperatorInfo https://www.asawicki.info/news_1654_stencil_test_explained_using_code
	*****************************************************************************/
	struct StencilOperatorInfo
	{
		CompareOperator CompareOperator   = CompareOperator::Always; // Stencil test
		StencilOperator FailOperator      = StencilOperator::Keep;   // Failed stencil action
		StencilOperator PassOperator      = StencilOperator::Keep;   // Succeed stencil action
		StencilOperator DepthFailOperator = StencilOperator::Keep;   // Failed depth test action
		gu::uint32   Reference         = 0;
		StencilOperatorInfo() = default;
	};

	/****************************************************************************
	*				  			DepthStencilProperty
	*************************************************************************//**
	*  @class     DepthStencilProperty
	*  @brief     When the depthStencilState class is used, you use the structure. 
	*****************************************************************************/
	struct DepthStencilProperty
	{
		bool                UseDepthTest       = true;                       // Use depth test
		bool                DepthWriteEnable   = true;                       // Enable to write depth
		bool                StenciWriteEnable  = false;                      // Enable to write Stencil (stencil test: 描画マスクみたいなやつ)  
		bool                UseDepthBoundsTest = false;                      // Use depth bounds test (vulkan api only)https://shikihuiku.wordpress.com/2012/06/27/depth-bounds-test1/
		float               MinDepthBounds     = 0.0f;                       // Min depth bounds test region
		float               MaxDepthBounds     = 0.0f;                       // Max depth bounds test region
		CompareOperator     DepthOperator      = CompareOperator::LessEqual; // Depth test operator
		StencilOperatorInfo Front              = StencilOperatorInfo();      // Use depth test and stencil test results for pixels with surface normals facing the camera
		StencilOperatorInfo Back               = StencilOperatorInfo();      // Use depth test and stencil test results for pixels where the surface normal is away from the camera
	};

#pragma endregion  DepthStencilState
#pragma region InputAssemblyState
	enum class PrimitiveTopology : gu::uint8
	{
		Undefined     = 0,
		PointList     = 1,
		LineList      = 2,
		LineStrip     = 3,
		TriangleList  = 4,
		TriangleStrip = 5,
		CountOfPrimitiveTopology
	};

	enum class InputClassification : gu::uint8
	{
		PerVertex   = 0,
		PerInstance = 1,
	};

	#pragma endregion InputAssemblyState
#pragma region GPUResource
	enum class RootSignatureType : gu::uint8
	{
		Rasterize,
		RayTracingGlobal,
		RayTracingLocal
	};

	enum class ResourceDimension : gu::uint8
	{
		Buffer,
		Dimension1D,
		Dimension2D,
		Dimension3D
	};

	enum class ResourceType : gu::uint8
	{
		Unknown                           = 0,
		Buffer                            = 1,
		Texture1D                         = 2,
		Texture1DArray                    = 3,
		Texture2D                         = 4,
		Texture2DArray                    = 5,
		Texture2DMultiSample              = 6,
		Texture2DArrayMultiSample         = 7,
		Texture3D                         = 8,
		TextureCube                       = 9,
		TextureCubeArray                  = 10,
		RaytracingAccelerationStructure   = 11
	};

	/****************************************************************************
	*				  			ResourceLayout
	*************************************************************************//**
	*  @class     ResourceState
	*  @brief     How to use resource
	*****************************************************************************/
	enum class ResourceState : gu::uint32
	{
		Common,
		GeneralRead,
		VertexAndConstantBuffer,
		IndexBuffer,
		UnorderedAccess,
		RenderTarget,
		DepthStencil,
		CopyDestination,
		CopySource,
		Present,
		Indirected,
		RayTracingAccelerationStructure,
		ShadingRateSource,
		CountOfResourceLayout
	};

	/****************************************************************************
	*				  			MemoryHeap
	*************************************************************************//**
	*  @class     MemoryHeap
	*  @brief     memory type
	*****************************************************************************/
	enum class MemoryHeap : gu::uint8
	{
		Default, // Memory area visible only from GPU  
		Upload,  // Memory area visible to CPU and GPU (Read from GPU is used for one time.)
		Readback,// 
		Custom   // for directX12
	};

	enum class DescriptorHeapType
	{
		CBV,         // const buffer view
		SRV,         // shader resource view
		UAV,         // unordered access view
		SAMPLER,     // dynamic sampler state

		// directX12 api only heap type
		RTV,         // render target (only directX12)
		DSV,         // depth stencil (only directX12)
	};

	struct DefaultHeapCount
	{
		gu::uint32 CBVDescCount = 0;
		gu::uint32 SRVDescCount = 0;
		gu::uint32 UAVDescCount = 0;
		gu::uint32 SamplerDescCount = 0;
		gu::uint32 RTVDescCount = 0;
		gu::uint32 DSVDescCount = 0;
	};
	/****************************************************************************
	*				  			ResourceViewType
	*************************************************************************//**
	*  @enum      ResourceViewType
	*  @brief     How to read gpu resource buffer
	*****************************************************************************/
	enum class ResourceViewType : gu::uint32
	{
		Unknown,
		ConstantBuffer       , // 256 alighment buffer
		Sampler              , // dynamic sampler state
		Texture              , // readonly texture resource 
		RWTexture            , // read-write texture resource
		Buffer               , // readonly buffer resource
		RWBuffer             , // read-write buffer resource
		StructuredBuffer     , // readonly 
		RWStructuredBuffer   , // read-write buffer resoure
		AccelerationStructure, // raytracing acceleration structure
		ShadingRateSource    ,
		RenderTarget         , // render target
		DepthStencil         , // depth and stencil 
	};

	/****************************************************************************
	*				  			ResourceUsage
	*************************************************************************//**
	*  @enum      ResourceUsage
	*  @brief     resource 
	*****************************************************************************/
	enum class ResourceUsage : gu::uint32
	{
		None = 0,

		// バッファへの書き込みは一度だけ行われる
		Static                = 0x000001,

		// バッファは時々書き込まれ, GPUは読み取り専用でCPUは書き込み専用となる. 
		Dynamic               = 0x000002,

		// バッファの寿命は1フレーム. フレームごとに書き込むか新しいものを作るかの対応が必要となる.
		Volatile              = 0x000004,

		//　Unorder access viewを作成する
		UnorderedAccess       = 0x000008,

		// byte adress buffer : バイト単位でインデックスが作成される読み取り専用バッファ (基本uint32)
		ByteAddress           = 0x000010,

		// コピー元として使用するGPUバッファ
		SourceCopy            = 0x000020,

		// Stream出力ターゲットとして使用するGPUバッファ
		StreamOutput          = 0x000040,

		// DispatchIndirectまたはDrawIndirectが使用する引数を含むバッファを作成する
		DrawIndirect          = 0x000080,

		// シェーダ・リソースとしてバインドできるバッファを作成します。 
		// 頂点バッファのような、通常はシェーダリソースとして使用されないバッファにのみ必要です。
		ShaderResource        = 0x000100,

		// CPUに直接アクセス可能なバッファ
		CPUAccessible         = 0x000200,

		// バッファは高速VRAMに置く
		FastVRAM              = 0x000400,

		// 外部のRHIやプロセスと共有できるバッファを作成する
		Shared                = 0x000800,

		// RayTracing用のAcceleration structureを作成する
		AccelerationStructure = 0x001000,

		// 頂点バッファとして使用する
		VertexBuffer          = 0x002000,

		// インデックスバッファとして使用する
		IndexBuffer           = 0x004000,

		// 構造化バッファ
		StructuredBuffer      = 0x008000,

		// 定数バッファ
		ConstantBuffer        = 0x010000,

		// レンダーターゲット
		RenderTarget          = 0x020000, // allow render target

		// デプスステンシル
		DepthStencil          = 0x040000, // allow depth stencil

		// バッファメモリは、ドライバのエイリアシングによって共有されるのではなく、複数のGPUに対して個別に割り当てられる
		MultiGPUAllocate      = 0x080000,

		// バッファをレイトレーシングのアクセラレーション構造を構築するためのスクラッチバッファとして使用できるようにします
		// バッファのアライメントのみを変更し、他のフラグと組み合わせることができます
		Scratch = 0x100000,

		AnyDynamic = (Dynamic | Volatile),
	};

	ENUM_CLASS_FLAGS(ResourceUsage);
	/*inline ResourceUsage operator | (const ResourceUsage& left, const ResourceUsage& right)
	{
		return static_cast<ResourceUsage>(static_cast<gu::uint32>(left) | static_cast<gu::uint32>(right));
	}

	inline ResourceUsage operator & (const ResourceUsage& left, const ResourceUsage& right)
	{
		return static_cast<ResourceUsage>( static_cast<gu::uint32>(left) & static_cast<gu::uint32>(right));
	}

	inline bool EnumHas(const ResourceUsage& left, const ResourceUsage& right)
	{
		if ((left & right) == right) return true;
		return false;
	}*/

	enum class BindlessResourceType
	{
		Unsupported = 0,
		OnlyRayTracing = 1,
		AllShaderTypes = 2,
	};
#pragma region GPUBuffer
	/****************************************************************************
	*				  			BufferType
	*************************************************************************//**
	*  @enum      BufferType
	*  @brief     buffer type 
	*****************************************************************************/
	enum class BufferType
	{
		Vertex,   // Vertex buffers
		Index,    // Index buffer 
		Constant, // 256 byte alignment buffer
		Upload,   // CPU readonly buffer
		Default,  // GPU readonly buffer
	};

	/****************************************************************************
	*				  			GPUBufferInfo
	*************************************************************************//**
	*  @class     GPUBufferInfo
	*  @brief     GPUBufferInfo
	*****************************************************************************/
	struct GPUBufferMetaData
	{
	public:
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		size_t         Stride        = 0;                           // element byte size
		size_t         Count         = 0;                           // element array count 
		size_t         ByteSize      = 0;
		ResourceType   ResourceType  = ResourceType::Unknown;       // GPU resource type
		ResourceUsage  ResourceUsage = ResourceUsage::None;         // how to use resource 
		ResourceState  State         = ResourceState::GeneralRead; // resource layout
		MemoryHeap     HeapType      = MemoryHeap::Default;         // memory heap type
		BufferType     BufferType    = BufferType::Upload;          // static or dynamic buffer
		PixelFormat    Format        = PixelFormat::Unknown;        // 基本的には使用しないが, Vulkanのビュー指定に必要となる場合がある
		void*          InitData      = nullptr; // Init Data

		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool IsCPUAccessible() const
		{
			return HeapType == MemoryHeap::Upload || HeapType == MemoryHeap::Readback
			|| (HeapType == MemoryHeap::Custom);
		}

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUBufferMetaData() = default;

		GPUBufferMetaData(size_t stride, size_t count, core::ResourceUsage usage, ResourceState layout, MemoryHeap heapType, core::BufferType bufferType, void* initData = nullptr);
		/****************************************************************************
		**                Static Function
		*****************************************************************************/
		//static GPUBufferMetaData UploadBuffer(const PixelFormat format, const size_t count, const MemoryHeap heap = MemoryHeap::Upload, void* initData = nullptr);
		static GPUBufferMetaData UploadBuffer  (const size_t stride, const size_t count, const MemoryHeap heap = MemoryHeap::Upload, void* initData = nullptr);
		static GPUBufferMetaData DefaultBuffer (const size_t stride, const size_t count, const MemoryHeap heap = MemoryHeap::Default, void* initData = nullptr);
		static GPUBufferMetaData ConstantBuffer(const size_t stride, const size_t count, const MemoryHeap heap = MemoryHeap::Upload , const ResourceState state = ResourceState::Common, void* initData = nullptr); // auto alignment 
		static GPUBufferMetaData VertexBuffer  (const size_t stride, const size_t count, const MemoryHeap heap = MemoryHeap::Default, const ResourceState state = ResourceState::GeneralRead, void* initData = nullptr);
		static GPUBufferMetaData IndexBuffer   (const size_t stride, const size_t count, const MemoryHeap heap = MemoryHeap::Default, const ResourceState state = ResourceState::Common, void* initData = nullptr);
	private:
		size_t CalcConstantBufferByteSize(const size_t byteSize) { return (byteSize + 255) & ~255; }
	};
#pragma endregion         GPUBuffer
#pragma region GPUTexture
	
	
	/****************************************************************************
	*				  			GPUTextureMetaData
	*************************************************************************//**
	*  @class     GPUTextureMetaData
	*  @brief     GPU total infomation
	*****************************************************************************/
	struct GPUTextureMetaData
	{
	public:
	
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		size_t            Width            = 1;                              // texture width
		size_t            Height           = 1;                              // texture height
		size_t            DepthOrArraySize = 1;                              // texture depth or array size
		size_t            ByteSize         = 0;                              // total byte size
		size_t            MipLevels        = 1;                              // mipmap levels
		PixelFormat       PixelFormat      = PixelFormat::Unknown;           // pixel color format 
		MultiSample       Sample           = MultiSample::Count1;            // multi sample count
		ResourceDimension Dimension        = ResourceDimension::Dimension1D; // texture resource dimension
		ResourceType      ResourceType     = ResourceType::Unknown;          // GPU resource type
		ResourceUsage     ResourceUsage    = ResourceUsage::None;            // how to use resource 
		ClearValue        ClearColor       = ClearValue();                   // clear color 
		ResourceState     State            = ResourceState::GeneralRead;    // resource layout
		MemoryHeap        HeapType         = MemoryHeap::Default;            // gpu heap type
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool IsCPUAccessible() const
		{
			return HeapType == MemoryHeap::Upload || HeapType == MemoryHeap::Readback
				|| (HeapType == MemoryHeap::Custom);
		}
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GPUTextureMetaData() = default;
		
		/****************************************************************************
		**                Static Function
		*****************************************************************************/
		static GPUTextureMetaData Texture1D                (const size_t width, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture1DArray           (const size_t width, const size_t length, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture2D                (const size_t width, const size_t height, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture2DArray           (const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture3D                (const size_t width, const size_t height, const size_t depth, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture2DMultiSample     (const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData Texture2DArrayMultiSample(const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const core::MultiSample sample, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData CubeMap                  (const size_t width, const size_t height, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData CubeMapArray             (const size_t width, const size_t height, const size_t length, const core::PixelFormat format, const size_t mipLevels = 1, const core::ResourceUsage usage = core::ResourceUsage::None);
		static GPUTextureMetaData RenderTarget             (const size_t width, const size_t height, const core::PixelFormat format, const core::ClearValue& clearValue = core::ClearValue());
		static GPUTextureMetaData RenderTargetMultiSample  (const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const core::ClearValue& clearValue = ClearValue());
		static GPUTextureMetaData DepthStencil             (const size_t width, const size_t height, const core::PixelFormat format, const core::ClearValue& clearValue = core::ClearValue());
		static GPUTextureMetaData DepthStencilMultiSample  (const size_t width, const size_t height, const core::PixelFormat format, const core::MultiSample sample, const core::ClearValue& clearValue = ClearValue());

	private:
		inline void CalculateByteSize() { ByteSize =  Width * Height * (Dimension == ResourceDimension::Dimension3D ? DepthOrArraySize : 1) * PixelFormatSizeOf::Get(PixelFormat) * MultiSampleSizeOf::Get(Sample); }
	};

#pragma endregion        GPUTexture
#pragma region RayTracing
	/****************************************************************************
	*				  			RayTracingGeometryFlags
	*************************************************************************//**
	*  @enum      RayTracingGeometryFlags
	*  @brief     Specifies flags for raytracing geometry
	*****************************************************************************/
	enum class RayTracingGeometryFlags
	{
		None,                         // No options specified  
		Opaque,                       // When ray encounter this geometry, the geometry acts as if no any hit shader is present.
		NoDuplicateAnyHitInvocation   // require that intersections be reported to the any hit shader at most once. This flag enables that guarantee for the given geometry, potentially with some performance impact.
	};
	/****************************************************************************
	*				  		 RayTracingInstanceFlags
	*************************************************************************//**
	*  @enum      RayTracingInstanceFlags
	*  @brief     Flags for a raytracing acceleration structure instance
	*****************************************************************************/
	enum class RayTracingInstanceFlags
	{
		None                          = 0x0,
		TriangleCullDisable           = 0x1, // Disables front/back face culling for this instance.
		TriangleFrontCounterClockwise = 0x2, // This flag reverses front and back facings, which is useful if the application’s natural winding order differs from the default (left coordinate). 
		ForceOpaque                   = 0x4, // All geometry in the BLAS Opaque
		ForceNonOpaque                = 0x8  // All geometry in the BLAS Non Opaque
	};
	/****************************************************************************
	*				  		 BuildAccelerationStructureFlags
	*************************************************************************//**
	*  @enum      BuildAccelerationStructureFlags
	*  @brief     How to build acceleration structure
	*****************************************************************************/
	enum class BuildAccelerationStructureFlags
	{
		None = 0x0000,
		AllowUpdate     = 0x0001, // Support update, 
		AllowCompaction = 0x0002, // Compact AS 
		PreferFastTrace = 0x0004, // RayTracing Performance Maximize. (AS Build time will increase)
		PreferFastBuild = 0x0008, // Build Time Minimize.
		MinimizeMemory  = 0x0010, // Minimize Memory. RayTracing Performance will decrease
		PreformUpdate   = 0x0020, // Update AS
	};
	/****************************************************************************
	*				  	RayTracingASPrebuildInfo
	*************************************************************************//**
	*  @struct    RayTracingASPrebuildInfo
	*  @brief     Acceleration Structure memory data size
	*****************************************************************************/
	struct RayTracingASPrebuildInfo
	{
		gu::uint64 AccelerationStructureSize = 0;
		gu::uint64 BuildScratchDataSize      = 0;
		gu::uint64 UpdateScratchDataSize    = 0;
	};
	
#pragma endregion RayTracing
#pragma endregion GPUResource
#pragma region Render Pass
	enum class AttachmentLoad : gu::uint8
	{
		Clear,    // at the beginning of a render path, erase already existing data with a specific value
		Load,     
		DontCare
	};
	enum class AttachmentStore
	{
		Store,
		DontCare
	};
	/****************************************************************************
	*				  			Attachment
	*************************************************************************//**
	*  @struct    Attachment
	*  @brief     attachment
	*****************************************************************************/
	struct Attachment
	{
	public:
		/****************************************************************************
		**                Static Function
		*****************************************************************************/
		static Attachment RenderTarget( const PixelFormat format,
			const ResourceState  initialState  = ResourceState::RenderTarget,
			const ResourceState  finalState    = ResourceState::Present,
			const AttachmentLoad load          = AttachmentLoad::Clear,
			const AttachmentStore store        = AttachmentStore::Store )
		{
			return Attachment(format, initialState, finalState, load, store, AttachmentLoad::DontCare, AttachmentStore::DontCare, MultiSample::Count1);
		}

		static Attachment RenderTargetMultiSample( const PixelFormat format, const MultiSample sample,
			const ResourceState   initialState  = ResourceState::RenderTarget,
			const ResourceState   finalState    = ResourceState::Present,
			const AttachmentLoad  load          = AttachmentLoad::Clear,
			const AttachmentStore store         = AttachmentStore::Store
		)
		{
			return Attachment(format, initialState, finalState, load, store, AttachmentLoad::DontCare, AttachmentStore::DontCare, sample);
		}

		static Attachment DepthStencil(const PixelFormat format,
			const ResourceState   initialState  = ResourceState::Common,
			const ResourceState   finalState    = ResourceState::DepthStencil,
			const AttachmentLoad  load          = AttachmentLoad::Clear,
			const AttachmentStore store         = AttachmentStore::Store,
			const AttachmentLoad  stencilLoad   = AttachmentLoad::DontCare,
			const AttachmentStore stencilStore  = AttachmentStore::DontCare
		)
		{
			return Attachment(format, initialState, finalState, load, store, stencilLoad, stencilStore, MultiSample::Count1);
		}

		static Attachment DepthStencilMultiSample(const PixelFormat format, const MultiSample sample,
			const ResourceState   initialState = ResourceState::DepthStencil,
			const ResourceState   finalState   = ResourceState::GeneralRead,
			const AttachmentLoad  load         = AttachmentLoad::Clear,
			const AttachmentStore store        = AttachmentStore::Store,
			const AttachmentLoad  stencilLoad  = AttachmentLoad::DontCare,
			const AttachmentStore stencilStore = AttachmentStore::DontCare
		)
		{
			return Attachment(format, initialState, finalState, load, store, stencilLoad, stencilStore, sample);
		}

		/****************************************************************************
		**                Public Function
		*****************************************************************************/

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		PixelFormat     Format        = PixelFormat::Unknown;         // pixel format
		MultiSample     SampleCount   = MultiSample::Count1;          // multi sample count (default: single sample count)
		ResourceState   InitialLayout = ResourceState::Common;        // initial resource layout  
		ResourceState  FinalLayout    = ResourceState::Present;       // final desired resource layout
		AttachmentLoad  LoadOp        = AttachmentLoad::Clear;        // at the beginning of a render path, erase already existing data with a specific value
		AttachmentStore StoreOp       = AttachmentStore::Store;       // at the end of a render pass, save data on memory
		AttachmentLoad  StencilLoad   = AttachmentLoad::DontCare;     // stencil; 
		AttachmentStore StencilStore  = AttachmentStore::DontCare;
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		Attachment() = default;
		Attachment(
			const PixelFormat     format,
			const ResourceState   initialState,
			const ResourceState   finalState,
			const AttachmentLoad  load,
			const AttachmentStore store,
			const AttachmentLoad  stencilLoad,
			const AttachmentStore stencilStore,
			const MultiSample     sample = MultiSample::Count1
		) : Format(format), SampleCount(sample), InitialLayout(initialState), FinalLayout(finalState), LoadOp(load), StoreOp(store), StencilLoad(stencilLoad), StencilStore(stencilStore){ };

		
	};
#pragma endregion       Render Pass
#pragma region Query
	/****************************************************************************
	*				  			QueryType
	*************************************************************************//**
	*  @enum      QueryType
	*  @brief     GPU情報の計測のために使用するHeapの設定
	*****************************************************************************/
	enum class QueryHeapType : gu::uint8
	{
		Occulusion,         // 遮蔽されていたら0, それ以外は遮蔽されていないことを示す
		TimeStamp,          // GPU, CPUのタイムスタンプに使用します
		PipelineStatistics, // Graphics pipelineの統計を示します
		CopyQueueTimeStamp, // copy queue用のタイムスタンプ
		CountOf,
	};

	/****************************************************************************
	*				  			GPUTimingCalibrationTimestamp
	*************************************************************************//**
	*  @enum      GPUTimingCalibrationTimestamp
	*  @brief     GPUとCPUの時間計測
	*****************************************************************************/
	struct GPUTimingCalibrationTimestamp
	{
		gu::uint64 GPUMicroseconds = 0;
		gu::uint64 CPUMicroseconds = 0;

		GPUTimingCalibrationTimestamp() = default;

		GPUTimingCalibrationTimestamp(const gu::uint64 gpuMicroseconds, const gu::uint64 cpuMicroseconds)
		{
			GPUMicroseconds = gpuMicroseconds;
			CPUMicroseconds = cpuMicroseconds;
		}
	};


#pragma endregion Query
}
#endif