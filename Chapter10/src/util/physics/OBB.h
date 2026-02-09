#pragma once
#include "../Math.h"

class AABB;

class OrientedBox
{
	/// Constructor
					OrientedBox() = default;
					OrientedBox(const Matrix4& inOrientation, const Vector3& inHalfExtents) : mOrientation(inOrientation), mHalfExtents(inHalfExtents) {};
					OrientedBox(const Matrix4& inOrientation, const AABB& inBox) : mOrientation(inOrientation), ;

	bool			OverLaps(const AABB& inBox, float inEpsilon = 1.0e-6f) const;
	bool			OverLaps(const OrientedBox& inBox, float inEpsilon = 1.0e-6f);

	Matrix4 mOrientation;
	Vector3 mHalfExtents;
};