//////////////////////////////////////////////////////////////////////////////////
///             @file   IGeometry.hpp
///             @brief  Geometry interface. 
///             @author Toide Yutaro
///             @date   2023_01_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PHYSICS_IGEOMETRY_HPP
#define PHYSICS_IGEOMETRY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "GeometryType.hpp"
#include "GameUtility/Base/Include/GUClassUtility.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                               Class
//////////////////////////////////////////////////////////////////////////////////
namespace physics::geometry
{
	/****************************************************************************
	*				  			  IGeometry
	*************************************************************************//**
	*  @class     IGeometry
	*  @brief     Geometry interface
	*****************************************************************************/
	class IGeometry : public gu::Copyable
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		/* @brief : Valid member variables check.*/
		virtual bool IsValid() const = 0; 

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		GeometryType GetGeometryType() const { return _geometryType; }

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		IGeometry(const GeometryType type) : _geometryType(type) {};

		virtual ~IGeometry() = default;

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
		GeometryType _geometryType = GeometryType::Invalid;
	};
}
#endif