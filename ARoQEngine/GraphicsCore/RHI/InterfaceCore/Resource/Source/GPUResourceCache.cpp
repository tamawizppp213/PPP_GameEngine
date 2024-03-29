//////////////////////////////////////////////////////////////////////////////////
//              @file   GPUResourceCache.cpp
///             @brief  GPUResourceCache
///             @author Toide Yutaro
///             @date   2022_11_13
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceCache.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUResourceView.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Resource/Include/GPUTexture.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDescriptorHeap.hpp"
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDevice.hpp"
#include <functional>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi;
using namespace rhi::core;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
GPUResourceCache::GPUResourceViewPtr GPUResourceCache::Load(const gu::tstring& filePath)
{
	/*-------------------------------------------------------------------
	-           Get hash code 
	---------------------------------------------------------------------*/
	const auto name = filePath + SP("_SRV");
	const auto stdName = std::wstring(name.CString());
	size_t hashCode = std::hash<std::wstring>()(stdName);
	if (_resourceViews.Contains(hashCode))
	{
		return _resourceViews.At(hashCode);
	}
	else // create texture and texture resource view
	{
		/*-------------------------------------------------------------------
		-           Load texture
		---------------------------------------------------------------------*/
		const auto texture = _device->CreateTextureEmpty();
		texture->Load(filePath, _commandList);
		texture->SetName(name);
		/*-------------------------------------------------------------------
		-           Load texture view
		---------------------------------------------------------------------*/
		const auto view = _device->CreateResourceView(core::ResourceViewType::Texture, texture, _customHeap);
		// regist resource view
		_resourceViews[hashCode] = view;
		return view;
	}

}

bool GPUResourceCache::Find(const gu::tstring& filePath)
{
	const auto name    = filePath + SP("_SRV");
	const auto stdName = std::wstring(name.CString());
	size_t hashCode = std::hash<std::wstring>()(stdName);
	
	return _resourceViews.Contains(hashCode);
}