//////////////////////////////////////////////////////////////////////////////////
///             @file   GPUDescriptorHeap.hpp
///             @brief  GPU DescriptorHeap 
///             @author Toide Yutaro
///             @date   2022_07_08
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHIDescriptorHeap.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace rhi::core;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
RHIDescriptorHeap::~RHIDescriptorHeap()
{
	if (!_heapInfo.IsEmpty()) { _heapInfo.Clear(); }
}