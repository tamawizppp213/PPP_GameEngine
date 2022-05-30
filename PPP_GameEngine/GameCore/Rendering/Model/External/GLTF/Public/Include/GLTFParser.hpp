//////////////////////////////////////////////////////////////////////////////////
///             @file   Json.hpp
///             @brief  Json reader and writer:
///                     How To : Load -> std::map like Object->call["name"] or Array->call[index]-> Save etc
///             @author Toide Yutaro (dependency : rapidjson)
///             @date   2022_05_12  
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef GLTF_PARSER_HPP
#define GLTF_PARSER_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GLTFDocument.hpp"
#include "GLTFSerialize.hpp"
#include <vector>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                         Template Class
//////////////////////////////////////////////////////////////////////////////////
namespace gltf
{
	using namespace gm;
	struct GLTFVertex
	{
		Float3 Position;
		Float3 Normal;
		Float2 UV;

	};

	/****************************************************************************
	*				  			JsonDocument
	*************************************************************************//**
	*  @class     JsonDocument
	*  @brief     Document
	*****************************************************************************/
	class GLTFFile
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Load(const std::string& filePath); 
		void Save(const std::string& filePath, SerializeFlags flags = SerializeFlags::None); // まだ使用しないでください
		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GLTFDocument Document;
	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		
	};


}
#endif