//////////////////////////////////////////////////////////////////////////////////
///             @file   GUSmartPointer.hpp
///             @brief  参照カウントが0になったら自動で消去されるスマートポインタです.
///              
///                     SharedPointer : 参照が増えるたびに参照カウントが1増加する. 
///                                     参照カウントが0になったら自動で消去される
///                     WeakPointer   : Observerとして使用. 所有権は保持せず, リソースの破棄も行われない
///                     UniquePointer : 所有権を一つしか持てない. 
/// 　　　　　　　　　　　　　　　　　　　　　　　 所有権を移譲しながら, 唯一の所有権を破棄するタイミングでリソースが破棄される
///                     
///             @author Toide Yutaro
///             @date   2022_03_16
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GU_SMART_POINTER_HPP
#define GU_SMART_POINTER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameUtility/Base/Private/Memory/Include/GUSharedPointer.hpp"
#include "GameUtility/Base/Private/Memory/Include/GUWeakPointer.hpp"
#include "GameUtility/Base/Private/Memory/Include/GUUniquePointer.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////

#endif