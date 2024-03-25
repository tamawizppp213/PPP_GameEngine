//////////////////////////////////////////////////////////////////////////////////
///             @file   GeometryBox.hpp
///             @brief  Geometry box interface. 
///             @author Toide Yutaro
///             @date   2023_01_09
//////////////////////////////////////////////////////////////////////////////////
#pragma once
#ifndef PHYSICS_BOX_GEOMETRY_HPP
#define PHYSICS_BOX_GEOMETRY_HPP

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
	*				  			  GeometryBox
	*************************************************************************//**
	*  @class     GeometryBox
	*  @brief     Geometry interface
	*****************************************************************************/
	class GeometryAABB : public IGeometry
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool IsValid() const override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		// @brief: half of the width, height, and depth of the box.
		gm::Float3 HalfExtents = { 0.0f, 0.0f, 0.0f };

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GeometryAABB(const gm::Float3& halfExtents) : IGeometry(GeometryType::AABB), HalfExtents(halfExtents) {};

		GeometryAABB(const float hx = 0.0f, const float hy = 0.0f, const float hz = 0.0f)
			: IGeometry(GeometryType::AABB), HalfExtents({ hx, hy, hz }) 
		{
			Check(IsValid());
		};

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

	/****************************************************************************
	*				  			  GeometryBox
	*************************************************************************//**
	*  @class     GeometryBox
	*  @brief     Geometry interface
	*****************************************************************************/
	class GeometryOBB : public IGeometry
	{
	public:
		/****************************************************************************
		**                Public Function
		*****************************************************************************/
		bool IsValid() const override;

		/****************************************************************************
		**                Public Member Variables
		*****************************************************************************/
		// @brief: half of the width, height, and depth of the box.
		gm::Float3 HalfExtents = { 0.0f, 0.0f, 0.0f };

		/****************************************************************************
		**                Constructor and Destructor
		*****************************************************************************/
		GeometryOBB(const gm::Float3& halfExtents) : IGeometry(GeometryType::OBB), HalfExtents(halfExtents) {};

		GeometryOBB(const float hx = 0.0f, const float hy = 0.0f, const float hz = 0.0f)
			: IGeometry(GeometryType::OBB), HalfExtents({ hx, hy, hz })
		{
			Check(IsValid());
		};

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