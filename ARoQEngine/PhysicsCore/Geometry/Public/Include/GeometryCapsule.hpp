//////////////////////////////////////////////////////////////////////////////////
///             @file   IGeometry.hpp
///             @brief  Geometry interface. 
///             @author Toide Yutaro
///             @date   2023_01_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PHYSICS_CAPSULE_GEOMETRY_HPP
#define PHYSICS_CAPSULE_GEOMETRY_HPP

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../../Private/Include/IGeometry.hpp"
#include "GameUtility/Math/Include/GMVector.hpp"
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
	class GeometryCapsule : public IGeometry
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool IsValid() const override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		float Radius     = 0.0f; // @brief: the radius of the capsule
		float HalfHeight = 0.0f; // @brief: half of the capsule's height,  measured between the centers of the hemispherical ends.

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GeometryCapsule(const float radius = 0.0f, const float halfHeight = 0.0f) :
			IGeometry(GeometryType::Capsule), Radius(radius), HalfHeight(halfHeight)
		{
			Check(IsValid());
		}
	
	protected:
		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/

		/****************************************************************************
		**                Protected Function
		*****************************************************************************/

		/****************************************************************************
		**                Protected Member Variables
		*****************************************************************************/
	};
}
#endif