//////////////////////////////////////////////////////////////////////////////////
//              @file   GeometryCapsule.cpp
///             @brief  SampleSky
///             @author Toide Yutaro
///             @date   2022_04_23
//////////////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////////////
//                             Include
//////////////////////////////////////////////////////////////////////////////////
#include "../Include/GeometrySphere.hpp"
//////////////////////////////////////////////////////////////////////////////////
//                              Define
//////////////////////////////////////////////////////////////////////////////////
using namespace physics::geometry;

//////////////////////////////////////////////////////////////////////////////////
//                          Implement
//////////////////////////////////////////////////////////////////////////////////
bool GeometrySphere::IsValid() const
{
	if (_geometryType != GeometryType::Sphere) { return false; }

	if ((!isfinite(Radius))) { return false; }

	if (Radius <= 0.0f) { return false; }

	return true;
}