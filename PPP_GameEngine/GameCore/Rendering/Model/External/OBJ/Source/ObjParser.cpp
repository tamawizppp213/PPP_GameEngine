//////////////////////////////////////////////////////////////////////////////////
//              @file   OBJParser.cpp
///             @brief  OBJ File Parser
///             @author Toide Yutaro
///             @date   2022_05_07
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GameCore/Rendering/Model/External/OBJ/Include/OBJParser.hpp"
#include "GameUtility/File/Include/FileSystem.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace obj;
//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
bool OBJFile::Load(const std::wstring& filePath)
{
	/*-------------------------------------------------------------------
	-             Open File
	---------------------------------------------------------------------*/
	FILE* filePtr = file::FileSystem::OpenFile(filePath);
	if (filePtr == nullptr) { return false; }

	return true;
}