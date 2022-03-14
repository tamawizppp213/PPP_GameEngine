//////////////////////////////////////////////////////////////////////////////////
///             @file   GraphicsDevice.hpp
///             @brief  Graphics device
///             @author Toide Yutaro
///             @date   2022_03_11
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef IGRAPHICS_DEVICE_HPP
#define IGRAPHICS_DEVICE_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include <Windows.h>
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
enum class GPUAPI
{
	DirectX12,
	CountOfAPI
};
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////


/****************************************************************************
*				  			IGraphicsDevice
*************************************************************************//**
*  @class     IGrahicsDevice
*  @brief     Graphics device interface
*****************************************************************************/
class IGraphicsDevice
{
protected:
	static const UINT32 FRAME_BUFFER_COUNT = 3;
	static const UINT32 VSYNC              = 1; // 0: don't wait, 1:wait(60fps)
public:
	/****************************************************************************
	**                Public Function
	*****************************************************************************/
	virtual void Initialize(HWND hwnd) = 0;
	virtual void OnResize  () = 0;
	virtual void Finalize  () = 0;
	virtual void ClearScreen() = 0;
	virtual void CompleteInitialize() = 0;
	virtual void CompleteRendering() = 0;
	virtual void FlushCommandQueue() = 0;
		
	/****************************************************************************
	**                Public Member Variables
	*****************************************************************************/
	void SetHWND(HWND hwnd) { _hwnd = hwnd; }
	/****************************************************************************
	**                Constructor and Destructor
	*****************************************************************************/
protected:
	/****************************************************************************
	**                Protected Function
	*****************************************************************************/

	/****************************************************************************
	**                Protected Member Variables
	*****************************************************************************/
	bool _hasInitialized = false;
	HWND _hwnd           = nullptr;
	GPUAPI _gpuApi;
};

#endif