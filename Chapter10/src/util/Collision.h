#pragma once
#include "Math.h"
#include <vector>

using Points = std::vector<Vector3>;
using Planes = std::vector<float>;
using NormPlanes = std::vector<std::pair<float, Vector3>>;

struct LineSegment
{
	Vector3 mStart;
	Vector3 mEnd;

	LineSegment(const Vector3& start, const Vector3& end);
	Vector3 PointOnSegment(float t) const;
	float MinDistSq(const Vector3& point) const;
};

struct Plane
{
	Vector3 mNormal;
	float mD;

	Plane(const Vector3& a, const Vector3& b, const Vector3& c);
	float SignedDist(const Vector3& point) const;
};

struct Sphere
{
	Vector3 mCenter;
	float mRadius;

	Sphere(const Vector3& center, float radius);
	bool Contains(const Vector3& point) const;
};

struct AABB
{
	Vector3 mMin;
	Vector3 mMax;

	AABB();
	explicit AABB(const Vector3& point);
	AABB(const Vector3& min, const Vector3& max);
	AABB GetAABB(Points points);
	void UpdateMinMax(const Vector3& point);
	
	void Rotate(const Quaternion& q);
	bool Contains(const Vector3& point) const;
	float MinDistSq(const Vector3& point) const;
};

struct Transform
{
	Vector3 position;
	Quaternion rotation;
	float scale;
};

struct OBB
{
	Vector3 mCenter;
	Quaternion mRotation;
	Vector3 mExtents;

	OBB();
	OBB CreateWorldOBB(const Transform& objTransform, const AABB& localAABB);
	bool Contains(const Vector3& point);
};

struct Capsule
{
	LineSegment mSegment;
	float mRadius;

	bool Contains(const Vector3& point) const;
};

struct ConvexPolygon
{
	std::vector<Vector2> mVertices;

	bool Contains(const Vector2& point) const;
};

namespace Collision
{
	bool Intersect(const Sphere& a, const Sphere& b);
	bool Intersect(const AABB& a, const AABB& b);
	bool Intersect(const Sphere& a, const AABB& b);
	bool Intersect(const LineSegment& l, const Plane& p, float& outT);
	bool Intersect(const LineSegment& l, const Sphere& s, float& outT);
	bool Intersect(const LineSegment& l, const AABB& b, float& outT);
	bool Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm);
	bool Intersect(const OBB& a, const OBB& b);
	bool SweptSphere(const Sphere& P0, const Sphere& P1, const Sphere& Q0, const Sphere& Q1, float& outT);
	
	namespace Healper
	{
		bool TestSidePlane(float start, float end, float negd, Planes& out);
		bool TestSidePlane(float start, float end, float negd, const Vector3& norm, NormPlanes& out);
		float GetProjectionRadius(const Vector3& axis, const std::vector<Vector3> directions, const Vector3& axis);
	}
}