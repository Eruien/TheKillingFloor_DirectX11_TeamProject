#pragma once
#include "LStd.h"
#include "TMath.h"
#include "LInput.h"
#include "LGlobal.h"
#include "LNode.h"


struct Plane;
struct Cube;
struct Sphere;
struct Rect;
struct Ray;
struct Circle;

using Color = TVector4;


class Collision
{
public:
	// --------------------------------- Ray ---------------------------------//
	static bool RayToFace(const Ray& ray, const TVector3& v0, const TVector3& v1, const TVector3& v2, TVector3* pickPoint = nullptr);

	// --------------------------------- Cube ---------------------------------//

	static bool CubeToRay(const Cube& cube, const Ray& ray);
	static bool CubeToPlane(const Cube& cube, const Plane& plane);

	// --------------------------------- Circle ---------------------------------//
	static bool CircleToPoint(const Circle& circle, const TVector3& point);
};



enum class CollisionPos
{
	Overlap,
	Behind,
	Front
};


// --------------------------------- Ray ---------------------------------//
struct Ray
{
	TVector3		origin = { 0,0,0 };
	TVector3		direction = { 0,0,0 };

	void UpdateRay(const TMatrix& matView, const TMatrix& matProj);
};

// --------------------------------- Cube ---------------------------------//

struct Cube
{
	union
	{
		TVector3 position = { 0,0,0 };
		TVector3 center;
	};

	TVector3 min = { 0,0,0 };
	TVector3 max = { 0,0,0 };
	TVector3 axisVector[3] = { {1,0,0}, {0,1,0}, {0,0,1} };
	TVector3 size = { 0,0,0 };

	Cube() {};
	Cube(TVector3& center, float x, float y, float z);
	Cube(TVector3& min, TVector3& max);

	void SetCube(TVector3& center, float x, float y, float z);
	void SetCube(TVector3& min, TVector3& max);
	void SetHeight(float minY, float maxY);
};

// --------------------------------- Plane ---------------------------------//

struct Plane
{
	TVector4 normal = { 0,0,0,0 };

	Plane() {};
	Plane(TVector3& p0, TVector3& p1, TVector3& p2);
	void SetPlane(TVector3& p0, TVector3& p1, TVector3& p2);
};


// --------------------------------- Circle ---------------------------------//

struct Circle
{
	union
	{
		TVector2 center;
		TVector2 position;
	};

	float radius = 0.0f;

	Circle(TVector2 center, float radius);
	void SetCircle(TVector2 center, float radius);
};