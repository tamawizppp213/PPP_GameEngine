//////////////////////////////////////////////////////////////////////////////////
///             @file   RHIInstance.hpp
///             @brief  Select device api (このエンジンを使用時最初に呼び出す.) 
///             @author Toide Yutaro
///             @date   2022_09_05
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef VULKAN_RHI_HELPER_HPP
#define VULKAN_RHI_HELPER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <vulkan/vulkan.h>
#include <string>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                              Class
//////////////////////////////////////////////////////////////////////////////////
namespace rhi::vulkan
{
	struct Entry
	{
		std::string   Name          = "";
		bool          Optional      = false;
		void*         FeatureStruct = nullptr;
		std::uint32_t Version       = 0;

		Entry(const char* entryName, bool isOptional = false, void* pointerFeatureStruct = nullptr, std::uint32_t checkVersion = 0)
			: Name(entryName), Optional(isOptional), FeatureStruct(pointerFeatureStruct), Version(checkVersion)
		{

		}
	};
}

#endif