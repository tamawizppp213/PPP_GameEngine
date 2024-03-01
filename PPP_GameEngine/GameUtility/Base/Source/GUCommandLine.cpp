//////////////////////////////////////////////////////////////////////////////////
///             @file   GUCommandLine.cpp
///             @brief  temp
///             @author toide
///             @date   2024/03/02 1:38:12
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GUCommandLine.hpp"

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace gu;

bool    CommandLine::_hasInitialized = true;
tstring CommandLine::_commandLine    = SP("");

//////////////////////////////////////////////////////////////////////////////////
//                             Implement
//////////////////////////////////////////////////////////////////////////////////

#pragma region Constructor and Destructor 

#pragma endregion Constructor and Destructor

#pragma region Main Function
void CommandLine::SetUp(const tchar* newCommandLine)
{
	if (_hasInitialized)
	{
		Reset(); 
	}

	_commandLine    = newCommandLine;
	_hasInitialized = true;
}
#pragma endregion Main Function