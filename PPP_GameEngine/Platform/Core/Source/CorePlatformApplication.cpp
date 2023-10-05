//////////////////////////////////////////////////////////////////////////////////
//              @file   WindowsError.cpp
///             @brief  Log windows error code and error sentence for debugging
///             @author Toide Yutaro
///             @date   2023_09_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Core/Include/CorePlatformApplication.hpp"
#include "../../Core/Include/CoreWindowMessageHandler.hpp"
#include "../../Windows/Include/WindowsPlatformApplication.hpp"
#include <stdexcept>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace platform::core;
using namespace platform;
//////////////////////////////////////////////////////////////////////////////////
//                              Implement
/////////////////////////////////////////////////////////////////////////////////
std::shared_ptr<PlatformApplication> PlatformApplication::Create(const PlatformType type)
{
	switch (type)
	{
		case PlatformType::Windows:
		{
			#ifdef _WIN32
			return std::make_shared<windows::PlatformApplication>();
			#endif
		}
		default:
		{
			throw std::runtime_error("Unknown platform");
		}
	}

	OutputDebugStringA("platform application is nullptr");
	return nullptr;
}