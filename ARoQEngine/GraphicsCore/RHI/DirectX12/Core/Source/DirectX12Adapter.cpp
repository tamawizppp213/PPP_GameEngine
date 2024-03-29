//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIAdapter.hpp
///             @brief  Physical Device (adapter), Describe gpu information 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Adapter.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Device.hpp"
#include "GraphicsCore/RHI/DirectX12/Core/Include/DirectX12Debug.hpp"
#include "GameUtility/File/Include/UnicodeUtility.hpp"
#include "Platform/Core/Include/CorePlatformMacros.hpp"
#include "GameUtility/Base/Include/GUString.hpp"
#include <d3d12.h> // これ自体windows.h依存なのか

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::directX12;

//////////////////////////////////////////////////////////////////////////////////
//                              Implement
//////////////////////////////////////////////////////////////////////////////////
#pragma region Constructor and Destructor
RHIDisplayAdapter::RHIDisplayAdapter(const gu::SharedPointer<core::RHIInstance>& instance, const AdapterComPtr& adapter) : core::RHIDisplayAdapter(instance),_adapter(adapter)
{
	Checkf(_adapter, "adapter is nullptr.\n");

	DXGI_ADAPTER_DESC desc = {};
	adapter->GetDesc(&desc);
	
	const auto utf8Name = unicode::ToUtf8String(desc.Description);
	_name     = gu::string(utf8Name.c_str(), utf8Name.length());
	_venderID = desc.VendorId;
	_deviceID = desc.DeviceId; 
	_isDiscreteGPU = desc.DedicatedVideoMemory != 0;
}

RHIDisplayAdapter::~RHIDisplayAdapter()
{
	if (_adapter) { _adapter.Reset(); }
}
#pragma endregion Constructor and Destructor

#pragma region Factory
/****************************************************************************
*                     CreateDevice
*************************************************************************//**
*  @fn        gu::SharedPointer<core::RHIDevice> RHIDisplayAdapter::CreateDevice(const std::uint32_t frameCount)
* 
*  @brief     Return directX12 logical device.
* 
*  @param[in] std::uint32_t frameCount
* 
*  @return    gu::SharedPointer<core::RHIDevice> (directX12)
*****************************************************************************/
gu::SharedPointer<core::RHIDevice> RHIDisplayAdapter::CreateDevice()
{
	return gu::MakeShared<RHIDevice>(SharedFromThis());
}
#pragma endregion Factory

#pragma region Debug Function
/****************************************************************************
*                     PrintInfo
*************************************************************************//**
*  @fn        void RHIAdapter::PrintInfo()
* 
*  @brief     Print physical device information
* 
*  @param[in] void
* 
*  @return    void
*****************************************************************************/
void RHIDisplayAdapter::PrintInfo()
{
	DXGI_ADAPTER_DESC desc = {}; 
	_adapter->GetDesc(&desc);
	
	/*-------------------------------------------------------------------
	-                  Print Adapter Name
	---------------------------------------------------------------------*/
	std::wstring adapterName  
		         = L"\n//////////////////////////\n Adapter : ";
	adapterName += desc.Description;
	adapterName += L"\n//////////////////////////\n";

#if PLATFORM_OS_WINDOWS
	OutputDebugString(adapterName.c_str());
#else
	_RPTWN(_CRT_WARN, L"%s", adapterName.c_str());
#endif

	/*-------------------------------------------------------------------
	-                  memory description
	---------------------------------------------------------------------*/
	const auto systemMemoryStr       = L"System memory: "         + std::to_wstring(desc.DedicatedSystemMemory) + L"\n";
	const auto videoMemoryStr        = L"Video memory : "         + std::to_wstring(desc.DedicatedVideoMemory) + L"\n";
	const auto sharedSystemMemoryStr = L"Shared system memory : " + std::to_wstring(desc.SharedSystemMemory) + L"\n";
	
#if PLATFORM_OS_WINDOWS
	OutputDebugString(systemMemoryStr.c_str());
	OutputDebugString(videoMemoryStr.c_str());
	OutputDebugString(sharedSystemMemoryStr.c_str());
#else
	_RPTWN(_CRT_WARN, L"%s", systemMemoryStr.c_str());
	_RPTWN(_CRT_WARN, L"%s", videoMemoryStr.c_str());
	_RPTWN(_CRT_WARN, L"%s", sharedSystemMemoryStr.c_str());
#endif

	/*-------------------------------------------------------------------
	-                  Print Display Name
	---------------------------------------------------------------------*/
	IOutput* output = nullptr;

	for(int i = 0; _adapter->EnumOutputs(i, (IDXGIOutput**)&output) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		// Get output descriptor
		DXGI_OUTPUT_DESC outputDesc;
		output->GetDesc(&outputDesc);

		// Show Device Name to Output Debugger
		std::wstring text = L"\n***Output: ";
		text += outputDesc.DeviceName;
		text += L"n";

#if PLATFORM_OS_WINDOWS
		OutputDebugString(text.c_str());
#else
		_RPTWN(_CRT_WARN, L"%s", text.c_str());
#endif

		// Release IOutput Pointer
		SAFE_RELEASE(output);
	}
}
#pragma endregion Debug Function