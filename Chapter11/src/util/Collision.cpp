#include "Collision.h"
#include <array>
#include <algorithm>

LineSegment::LineSegment(const Vector3& start, const Vector3& end)
{
	mStart = start;
	mEnd = end;
}

Vector3 LineSegment::PointOnSegment(float t) const
{
	return mStart + (mEnd - mStart) * t;
}

float LineSegment::MinDistSq(const Vector3& point) const
{
	Vector3 ab = mEnd - mStart;
	Vector3 ba = -1.0 * ab;
	Vector3 ac = point - mStart;
	Vector3 bc = point - mEnd;

	if (Vector3::Dot(ab, ac) < 0.0f)
	{
		return ac.LengthSq();
	}
	else if (Vector3::Dot(ba, bc) < 0.0f)
	{
		return bc.LengthSq();
	}
	else
	{
		float scalar = Vector3::Dot(ac, ab) / Vector3::Dot(ab, ab);
		Vector3 p = scalar * ab;
		return (ac - p).LengthSq();
	}
}

Plane::Plane(const Vector3& a, const Vector3& b, const Vector3& c)
{
	Vector3 ab = b - a;
	Vector3 ac = c - a;

	mNormal = Vector3::Cross(ab, ac);
	mNormal.Normalize();
	mD = -Vector3::Dot(a, mNormal);
}

float Plane::SignedDist(const Vector3& point) const
{
	return Vector3::Dot(point, mNormal) - mD;
}

Sphere::Sphere(const Vector3& center, float radius)
{
	mCenter = center;
	mRadius = radius;
}

bool Sphere::Contains(const Vector3& point) const
{
	float distSq = (mCenter - point).LengthSq();
	return distSq <= (mRadius * mRadius);
}

AABB::AABB()
{
	mMin = Vector3::Zero;
	mMax = Vector3::Zero;
}

AABB::AABB(const Vector3& min, const Vector3& max)
{
	mMin = min;
	mMax = max;
}

//Must be Updated MinMax before to use.
AABB::AABB(const Vector3& point)
{
	mMin = point;
	mMax = point;
}
//Use this as much as possible
AABB AABB::GetAABB(Points points)
{
	auto box = AABB(points[0]);
	for (size_t i = 1; i < points.size(); ++i)
	{
		box.UpdateMinMax(points[i]);
	}
	return box;
}

void AABB::UpdateMinMax(const Vector3& point)
{
	mMin.x = Math::Min(mMin.x, point.x);
	mMin.y = Math::Min(mMin.y, point.y);
	mMin.z = Math::Min(mMin.z, point.z);
	mMax.x = Math::Max(mMax.x, point.x);
	mMax.y = Math::Max(mMax.y, point.y);
	mMax.z = Math::Max(mMax.z, point.z);
}

void AABB::Rotate(const Quaternion& q)
{
	std::array<Vector3, 8> points;
	points[0] = mMin;
	points[1] = Vector3(mMax.x, mMin.y, mMin.z);
	points[2] = Vector3(mMin.x, mMax.y, mMin.z);
	points[3] = Vector3(mMin.x, mMin.y, mMax.z);
	points[4] = Vector3(mMax.x, mMax.y, mMin.z);
	points[5] = Vector3(mMax.x, mMin.y, mMax.z);
	points[6] = Vector3(mMin.x, mMax.y, mMax.z);
	points[7] = mMax;

	Vector3 p = Vector3::Transform(points[0], q);
	mMin = p;
	mMax = p;

	for (size_t i = 0; i < points.size(); ++i)
	{
		p = Vector3::Transform(points[i], q);
		UpdateMinMax(p);
	}
}

float AABB::MinDistSq(const Vector3& point) const
{
	float dx = Math::Max(mMin.x - point.x, 0.0f);
	dx = Math::Max(dx, point.x - mMax.x);
	float dy = Math::Max(mMin.y - point.y, 0.0f);
	dy = Math::Max(dy, point.y - mMax.y);
	float dz = Math::Max(mMin.z - point.z, 0.0f);
	dz = Math::Max(dz, point.z - mMax.z);
	return dx * dx + dy * dy + dz * dz;
}


bool AABB::Contains(const Vector3& point) const
{
	bool outSide = point.x < mMin.x ||
		point.y < mMin.y ||
		point.z < mMin.z ||
		point.x > mMax.x ||
		point.y > mMax.y ||
		point.z > mMax.z;

	return !outSide;
}

OBB::OBB()
{
	mCenter = Vector3::Zero;
	mRotation = Quaternion::Identity;
	mExtents = Vector3::Zero;
}

OBB OBB::CreateWorldOBB(const Transform& objTransform, const AABB& localAABB)
{
	OBB outOBB;

	outOBB.mRotation = objTransform.rotation;

	Vector3 extent = (localAABB.mMax - localAABB.mMin) * 0.5f;
	outOBB.mExtents = extent * objTransform.scale;

	Vector3	localCenter = (localAABB.mMax + localAABB.mMin) * 0.5f;

	Vector3 rotatedOffset = Vector3::Transform(localCenter * objTransform.scale, objTransform.rotation);
	outOBB.mCenter = objTransform.position + rotatedOffset;

	return outOBB;
}

bool OBB::Contains(const Vector3& point)
{
	Vector3 dir = point - mCenter;
	Quaternion invRot = Quaternion::Conjugate(mRotation);
	Vector3 localPoint = Vector3::Transform(dir, invRot);

	bool outSide = Math::Abs(localPoint.x) <= mExtents.x &&
		Math::Abs(localPoint.y) <= mExtents.y &&
		Math::Abs(localPoint.z) <= mExtents.z;

	return !outSide;
}



bool Capsule::Contains(const Vector3& point) const
{
	float distSq = mSegment.MinDistSq(point);
	return distSq <= (mRadius * mRadius);
}

bool ConvexPolygon::Contains(const Vector2& point) const
{
	float sum = 0.0f;
	Vector2 a, b;
	for (size_t i = 0; i < mVertices.size() - 1; ++i)
	{
		a = mVertices[i] - point;
		a.Normalize();
		b = mVertices[i + 1] - point;
		b.Normalize();
		sum += Math::Acos(Vector2::Dot(a, b));
	}
	
	a = mVertices.back() - point;
	a.Normalize();
	b = mVertices.front() - point;
	b.Normalize();
	sum += Math::Acos(Vector2::Dot(a, b));
	return Math::NearZero(sum - Math::TwoPi);
}

bool Collision::Intersect(const Sphere& a, const Sphere& b)
{
	float distSq = (a.mCenter - b.mCenter).LengthSq();
	float sumRadii = a.mRadius + b.mRadius;
	return distSq <= (sumRadii * sumRadii);
}

bool Collision::Intersect(const AABB& a, const AABB& b)
{
	bool outSide = a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		a.mMin.x > b.mMax.x ||
		a.mMin.y > b.mMax.y ||
		a.mMin.z > b.mMax.z;

	return !outSide;
}

bool Collision::Intersect(const Sphere& a, const AABB& b)
{
	float distSq = b.MinDistSq(a.mCenter);
	return distSq <= (a.mRadius * a.mRadius);
}

bool Collision::Intersect(const LineSegment& l, const Plane& p, float& outT)
{
	//denominator
	float denom = Vector3::Dot(l.mEnd - l.mStart, p.mNormal);

	if (Math::NearZero(denom))
	{
		if (Math::NearZero(Vector3::Dot(l.mStart, p.mNormal) - p.mD))
		{
			outT = 0.0f;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		float number = -Vector3::Dot(l.mStart, p.mNormal) - p.mD;
		outT = number / denom;
		if (outT >= 0.0f && outT <= 1.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Collision::Intersect(const LineSegment& l, const Sphere& s, float& outT)
{
	Vector3 X = l.mStart - s.mCenter;
	Vector3 Y = l.mEnd - l.mStart;
	float a = Vector3::Dot(Y, Y);
	float b = Vector3::Dot(X, Y) * 2;
	float c = Vector3::Dot(X, X) - s.mRadius * s.mRadius;
	//discriminant
	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);
		float tMin = (-b - disc) / (2.0f * a);
		float tMax = (-b + disc) / (2.0f * a);
		if (tMin >= 0.0f && tMax <= 1.0f)
		{
			outT = tMin;
			return true;
		}
		else if (tMax >= 0.0f && tMin <= 1.0f)
		{
			outT = tMax;
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Collision::Intersect(const LineSegment& l, const AABB& b, float& outT)
{
	Planes tValues;
	
	Healper::TestSidePlane(l.mStart.x, l.mEnd.x, b.mMin.x, tValues);
	Healper::TestSidePlane(l.mStart.x, l.mEnd.x, b.mMax.x, tValues);
	Healper::TestSidePlane(l.mStart.y, l.mEnd.y, b.mMin.y, tValues);
	Healper::TestSidePlane(l.mStart.y, l.mEnd.y, b.mMax.y, tValues);
	Healper::TestSidePlane(l.mStart.z, l.mEnd.z, b.mMin.z, tValues);
	Healper::TestSidePlane(l.mStart.z, l.mEnd.z, b.mMax.z, tValues);
	std::ranges::sort(tValues);

	Vector3 point;
	for (float t : tValues)
	{
		point = l.PointOnSegment(t);
		if (b.Contains(point))
		{
			outT = t;
			return true;
		}
	}
	return false;
}

bool Collision::Intersect(const LineSegment& l, const AABB& b, float& outT, Vector3& outNorm)
{
	std::vector<std::pair<float, Vector3>> tValues;

	Healper::TestSidePlane(l.mStart.x, l.mEnd.x, b.mMin.x, Vector3::NegUnitX, tValues);
	Healper::TestSidePlane(l.mStart.x, l.mEnd.x, b.mMax.x, Vector3::UnitX, tValues);
	Healper::TestSidePlane(l.mStart.y, l.mEnd.y, b.mMin.y, Vector3::NegUnitY, tValues);
	Healper::TestSidePlane(l.mStart.y, l.mEnd.y, b.mMax.y, Vector3::UnitY, tValues);
	Healper::TestSidePlane(l.mStart.z, l.mEnd.z, b.mMin.z, Vector3::NegUnitZ, tValues);
	Healper::TestSidePlane(l.mStart.z, l.mEnd.z, b.mMax.z, Vector3::UnitZ, tValues);

	std::ranges::sort(tValues, [](
		const std::pair<float, Vector3>& a,
		const std::pair<float, Vector3>& b)
		{
			return a.first < b.first;
		});
	Vector3 point;
	for (auto& t : tValues)
	{
		point = l.PointOnSegment(t.first);
		if (b.Contains(point))
		{
			outT = t.first;
			outNorm = t.second;
			return true;
		}
	}
	return false;
}



bool Collision::Intersect(const OBB& a, const OBB& b)
{
	std::vector<Vector3> axisA(3), axisB(3);

	axisA[0] = Vector3::Transform(Vector3::UnitX, a.mRotation);
	axisA[1] = Vector3::Transform(Vector3::UnitY, a.mRotation);
	axisA[2] = Vector3::Transform(Vector3::UnitZ, a.mRotation);
	axisB[0] = Vector3::Transform(Vector3::UnitX, a.mRotation);
	axisB[1] = Vector3::Transform(Vector3::UnitY, a.mRotation);
	axisB[2] = Vector3::Transform(Vector3::UnitZ, a.mRotation);
	Vector3 interval = a.mCenter - b.mCenter;
	
	return false;
}

bool Collision::SweptSphere(const Sphere& P0, const Sphere& Q0, const Sphere& P1, const Sphere& Q1, float& outT)
{
	Vector3 X = P0.mCenter - Q0.mCenter;
	Vector3 Y = (P1.mCenter - P0.mCenter) - (Q1.mCenter - Q0.mCenter);

	float a = Vector3::Dot(Y, Y);
	float b = Vector3::Dot(X, Y) * 2.0f;
	float sumRadii = P0.mRadius + Q0.mRadius;
	float c = Vector3::Dot(X, X) - sumRadii * sumRadii;

	float disc = b * b - 4.0f * a * c;
	if (disc < 0.0f)
	{
		return false;
	}
	else
	{
		disc = Math::Sqrt(disc);
		outT = (-b - disc) / (2.0f * a);
		if (outT >= 0.0f && outT <= 1.0f)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Collision::Healper::TestSidePlane(float start, float end, float negd, Planes& out)
{
	float denom = end - start;
	if (Math::NearZero(denom))
	{
		return false;
	}
	else
	{
		float number = -start + negd;
		float t = number / denom;
		if (t >= 0.0f && t <= 1.0f)
		{
			out.emplace_back(t);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Collision::Healper::TestSidePlane(float start, float end, float negd, const Vector3& norm, NormPlanes& out)
{
	float denom = end - start;
	if (Math::NearZero(denom))
	{
		return false;
	}
	else
	{
		float number = -start + negd;
		float t = number / denom;
		if (t >= 0.0f && t <= 1.0f)
		{
			out.emplace_back(t, norm);
			return true;
		}
		else
		{
			return false;
		}
	}
}

float Collision::Healper::GetProjectionRadius(const Vector3& axis, const std::vector<Vector3> directions, const Vector3& extents)
{
	return extents.x * Math::Abs(Vector3::Dot(axis, directions[0])) +
		extents.y * Math::Abs(Vector3::Dot(axis, directions[1])) +
		extents.z * Math::Abs(Vector3::Dot(axis, directions[2]));
}