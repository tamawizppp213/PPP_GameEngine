////////////////////////////////////////////////////////////////////////////////////
/////             @file   Physics Actor.hpp
/////             @brief  Actor : all physics object 
/////             @author Toide Yutaro
/////             @date   2023_01_09
////////////////////////////////////////////////////////////////////////////////////
//#pragma once
//#ifndef PHYSICS_ACTOR_HPP
//#define PHYSICS_ACTOR_HPP
//
////////////////////////////////////////////////////////////////////////////////////
////                             Include
////////////////////////////////////////////////////////////////////////////////////
//#include "GameUtility/Base/Include/GUClassUtility.hpp"
//#include "GameUtility/Math/Include/GMTransform.hpp"
//#include "GameUtility/Base/Include/GUSmartPointer.hpp"
////////////////////////////////////////////////////////////////////////////////////
////                              Define
////////////////////////////////////////////////////////////////////////////////////
//
////////////////////////////////////////////////////////////////////////////////////
////                               Class
////////////////////////////////////////////////////////////////////////////////////
//namespace physics::geometry
//{
//	class IGeometry;
//}
//
//namespace physics::core
//{
//
//	enum class ActorFlags
//	{
//		None             = 0x0000,
//		DisableGravity   = 0x0001,
//		DisablePhysics   = 0x0002,
//		DisableCollision = 0x0004,
//	};
//
//	enum class ActorType
//	{
//		Primitive,     // NonPhysics 
//		RigidStatic,   // static rigid body
//		RigidDynamic,  // dynamic rigid body
//
//		CountOf,
//
//	};
//	/****************************************************************************
//	*				  			  PhysicsActor
//	*************************************************************************//**
//	*  @class     PhysicsActor
//	*  @brief     Has shape and global pose object
//	*****************************************************************************/
//	class PhysicsActor : public gu::NonCopyable
//	{
//	public:
//		/****************************************************************************
//		**                Public Function
//		*****************************************************************************/
//
//		/****************************************************************************
//		**                Public Member Variables
//		*****************************************************************************/
//		/* @brief : Retrieves the actors world space transform*/
//		gm::Transform GetGlobalPose() const { return gm::Transform(_orientation, _position); }
//
//		/* @brief : Return the rigid body (static or dynamic), or return the */
//		ActorType   GetActorType() const { return _actorType; };
//
//		/* @brief : Return world position*/
//		gm::Vector3 GetPosition() const { return _position; }
//
//		gm::Quaternion GetRotation() const { return _orientation; }
//
//		gu::SharedPointer<physics::geometry::IGeometry> GetGeometry() const noexcept { return _geometry; }
//
//		/* @brief : Set the actors world space transform.
//		   Holds the world position as the Vector3 and orientation as the quaternion.*/
//		void SetGrobalPose(const gm::Transform& globalPose)
//		{
//			_position    = globalPose.LocalPosition;
//			_orientation = globalPose.LocalRotation;
//		}
//
//		/* @brief : Return the actor flags*/
//		void SetActorFlags(const ActorFlags actorFlags) { _actorFlags = actorFlags; }
//		
//		void SetPosition(const gm::Vector3& position) { _position = position; }
//
//		void SetGeometry(const gu::SharedPointer<physics::geometry::IGeometry>& geometry) { _geometry = geometry; }
//
//		/****************************************************************************
//		**                Constructor and Destructor
//		*****************************************************************************/
//		explicit PhysicsActor(const ActorFlags flags) : _actorType(ActorType::Primitive), _actorFlags(flags) {};
//
//		PhysicsActor(const gu::SharedPointer<physics::geometry::IGeometry>& geometry,
//			const gm::Vector3& worldPosition,
//			const gm::Quaternion& rotation = gm::Quaternion(),
//			const ActorFlags flags = ActorFlags::None);
//
//	protected:
//		explicit PhysicsActor(const ActorType type) : _actorType(type) {};
//		/****************************************************************************
//		**                Constructor and Destructor
//		*****************************************************************************/
//
//		/****************************************************************************
//		**                Protected Function
//		*****************************************************************************/
//
//		/****************************************************************************
//		**                Protected Member Variables
//		*****************************************************************************/
//		// Holds the linear position of the rigid body in world space.
//		gm::Vector3 _position = {};
//
//		// Holds the angular orientation of the rigid body in world space.
//		gm::Quaternion _orientation = {};
//
//		ActorType _actorType = ActorType::Primitive;
//
//		ActorFlags _actorFlags = ActorFlags::None;
//
//		/* Holds the geometry (don't have the positition.)*/
//		gu::SharedPointer<physics::geometry::IGeometry> _geometry = nullptr;
//	};
//}
//#endif