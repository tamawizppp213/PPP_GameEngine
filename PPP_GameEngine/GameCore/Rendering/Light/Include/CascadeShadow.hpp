//////////////////////////////////////////////////////////////////////////////////
///             @file   CascadeShadow.hpp
///             @brief  CascadeShadow
///             @author Toide Yutaro
///             @date   2023_04_06
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef CASCADE_SHADOW_HPP
#define CASCADE_SHADOW_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GraphicsCore/RHI/InterfaceCore/Core/Include/RHITypeCore.hpp"
#include "GameUtility/Base/Include/ClassUtility.hpp"
#include <string>
#include <vector>

//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
class LowLevelGraphicsEngine;
namespace gc::core
{
	class GameModel;
}
//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace gc::rendering
{
	struct CascadeShadowDesc
	{
		float Near    = 200.0f;
		float Medium  = 500.0f;
		float Far     = 1000.0f;
	};
	/****************************************************************************
	*				  			    Class
	*************************************************************************//**
	*  @class     CascadeShadow
	*  @brief     Cascade shadow map + soft shadow
	*****************************************************************************/
	class CascadeShadow : public NonCopyable
	{
	protected:
		using LowLevelGraphicsEnginePtr = std::shared_ptr<LowLevelGraphicsEngine>;
		using GameModelPtr              = std::shared_ptr<gc::core::GameModel>;
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		void Draw();

		void Add(const GameModelPtr& gameModel);


		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		CascadeShadow(const LowLevelGraphicsEnginePtr& engine, const CascadeShadowDesc& desc);

		~CascadeShadow();

	protected:
		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		LowLevelGraphicsEnginePtr _engine = nullptr;

		std::vector<GameModelPtr> _gameModels = {};
	};
}
#endif