#include "Collision.h"


bool Collision::RayToFace(const Ray& ray, const TVector3& v0, const TVector3& v1, const TVector3& v2, TVector3* pickPoint)
{
	TVector3 edge1 = v1 - v0;
	TVector3 edge2 = v2 - v0;

	TVector3 pvec = ray.direction.Cross(edge2);
	float det = edge1.Dot(pvec);

	TVector3 tvec = TVector3::Zero;
	if (det > 0.0f) // shot front
	{
		tvec = ray.origin - v0;
	}
	else            // shot back
	{
		tvec = v0 - ray.origin;
		det = -det;
	}

	if (det < 0.0001f)	// parallel
		return false;

	// get u (v0 -> v1)
	float u = tvec.Dot(pvec);
	if (u < 0.0f || u > det)
		return false;

	// get v (v0 -> v2)
	TVector3 qvec = tvec.Cross(edge1);
	float v = ray.direction.Dot(qvec);
	if (v < 0.0f || u + v > det)
		return false;

	// get t (origin -> v0)
	// currentlly, t is not nercessary
	float t = edge2.Dot(qvec);
	float fInvDet = 1.0f / det;
	t *= fInvDet;
	u *= fInvDet;
	v *= fInvDet;

	if (pickPoint != nullptr)
		*pickPoint = v0 + edge1 * u + edge2 * v;

	return true;
}

bool Collision::CubeToRay(const Cube& cube, const Ray& ray)
{
	float t_min = -999999.0f;
	float t_max = 999999.0f;
	float  f[3], fa[3], s[3], sa[3];

	TVector3 vR = ray.origin - cube.center;

	TVector3 axisVec[3];
	axisVec[0] = cube.axisVector[0]; axisVec[0].Normalize();
	axisVec[1] = cube.axisVector[1]; axisVec[1].Normalize();
	axisVec[2] = cube.axisVector[2]; axisVec[2].Normalize();

	float extent[3] = { cube.size.x * 0.5f, cube.size.y * 0.5f, cube.size.z * 0.5f };

	for (int v = 0; v < 3; v++)
	{
		f[v] = axisVec[v].Dot(ray.direction);
		s[v] = axisVec[v].Dot(vR);
		fa[v] = fabs(f[v]);
		sa[v] = fabs(s[v]);

		if (sa[v] > extent[v] && s[v] * f[v] >= 0.0f)
			return false;

		float t1 = (-s[v] - extent[v]) / f[v];
		float t2 = (-s[v] + extent[v]) / f[v];
		if (t1 > t2)
			std::swap(t1, t2);

		t_min = t_min > t1 ? t_min : t1;
		t_max = t_max < t2 ? t_max : t2;
		if (t_min > t_max)
			return false;
	}

	float  fCross[3], fRhs;
	TVector3 vDxR;

	ray.direction.Cross(vR, vDxR);
	// D X pBox->vAxis[0]  분리축
	fCross[0] = fabs(vDxR.Dot(axisVec[0]));
	float fAxis2 = extent[1] * fa[2];
	float fAxis1 = extent[2] * fa[1];
	fRhs = fAxis2 + fAxis1;
	if (fCross[0] > fRhs)
		return false;

	// D x pBox->vAxis[1]  분리축
	fCross[1] = fabs(vDxR.Dot(axisVec[1]));
	fRhs = extent[0] * fa[2] + extent[2] * fa[0];
	if (fCross[1] > fRhs)
		return false;

	// D x pBox->vAxis[2]  분리축
	fCross[2] = fabs(vDxR.Dot(axisVec[2]));
	fRhs = extent[0] * fa[1] + extent[1] * fa[0];
	if (fCross[2] > fRhs)
		return false;

	return true;
}

bool Collision::CubeToPlane(const Cube& cube, const Plane& plane)
{
	float fDist = 0.0f;
	float fPlaneToCenter = 0.0f;
	TVector3 vDir;

	TVector3 axisVec[3];
	axisVec[0] = cube.axisVector[0]; axisVec[0].Normalize();
	axisVec[1] = cube.axisVector[1]; axisVec[1].Normalize();
	axisVec[2] = cube.axisVector[2]; axisVec[2].Normalize();

	vDir = axisVec[0] * (cube.size.x * 0.5f);
	fDist += fabs(plane.normal.x * vDir.x + plane.normal.y * vDir.y + plane.normal.z * vDir.z);
	vDir = axisVec[1] * (cube.size.y * 0.5f);
	fDist += fabs(plane.normal.x * vDir.x + plane.normal.y * vDir.y + plane.normal.z * vDir.z);
	vDir = axisVec[2] * (cube.size.z * 0.5f);
	fDist += fabs(plane.normal.x * vDir.x + plane.normal.y * vDir.y + plane.normal.z * vDir.z);

	fPlaneToCenter = plane.normal.x * cube.center.x + plane.normal.y * cube.center.y
		+ plane.normal.z * cube.center.z + plane.normal.w;

	if (fPlaneToCenter < -fDist)
		return false;

	return true;
}

bool Collision::CircleToPoint(const Circle& circle, const TVector3& point)
{
	TVector2 p = { point.x, point.z };

	if (fabs((circle.center - p).Length()) < circle.radius)
		return true;

	return false;
}


// --------------------------------- Ray ---------------------------------//

void Ray::UpdateRay(const TMatrix& viewMat, const TMatrix& projectionMat)
{
	POINT mousePos = LInput::GetInstance().GetMousePos();

	int nX = (int)mousePos.x;
	int nY = (int)mousePos.y;

	origin =TVector3::Zero;
	direction.x = (((2.0f * nX) / (float)LGlobal::g_WindowWidth) - 1) / projectionMat._11;
	direction.y = (((-2.0f * nY) / (float)LGlobal::g_WindowHeight) + 1) / projectionMat._22;
	direction.z = 1.0f;

	TMatrix invertViewMat = viewMat.Invert();

	origin = origin.Transform(origin, invertViewMat);
	direction = direction.TransformNormal(direction, invertViewMat);
	direction.Normalize();
}

// --------------------------------- Cube ---------------------------------//

Cube::Cube(TVector3& center, float x, float y, float z)
{
	this->center = center;
	size = TVector3(x, y, z);

	min = center - size * 0.5f;
	max = center + size * 0.5f;

	TVector3 c0, c2, c3, c6;
	c2 = min;
	c3 = min; c3.x += size.x;
	c0 = c2; c0.y += size.y;
	c6 = c2; c6.z += size.z;
	axisVector[0] = (c3 - c2) * 0.5f;
	axisVector[1] = (c0 - c2) * 0.5f;
	axisVector[2] = (c6 - c2) * 0.5f;
}

Cube::Cube(TVector3& min, TVector3& max)
{
	this->min = min;
	this->max = max;

	center = (max + min) * 0.5f;
	size = max - min;

	TVector3 c0, c2, c3, c6;
	c2 = min;
	c3 = min; c3.x += size.x;
	c0 = c2; c0.y += size.y;
	c6 = c2; c6.z += size.z;
	axisVector[0] = (c3 - c2) * 0.5f;
	axisVector[1] = (c0 - c2) * 0.5f;
	axisVector[2] = (c6 - c2) * 0.5f;
}

void Cube::SetCube(TVector3& center, float x, float y, float z)
{
	this->center = center;
	size = TVector3(x, y, z);

	min = center - size * 0.5f;
	max = center + size * 0.5f;

	TVector3 c0, c2, c3, c6;
	c2 = min;
	c3 = min; c3.x += size.x;
	c0 = c2; c0.y += size.y;
	c6 = c2; c6.z += size.z;
	axisVector[0] = (c3 - c2) * 0.5f;
	axisVector[1] = (c0 - c2) * 0.5f;
	axisVector[2] = (c6 - c2) * 0.5f;
}

void Cube::SetCube(TVector3& min, TVector3& max)
{
	this->min = min;
	this->max = max;

	center = (max + min) * 0.5f;
	size = max - min;

	TVector3 c0, c2, c3, c6;
	c2 = min;
	c3 = min; c3.x += size.x;
	c0 = c2; c0.y += size.y;
	c6 = c2; c6.z += size.z;
	axisVector[0] = (c3 - c2) * 0.5f;
	axisVector[1] = (c0 - c2) * 0.5f;
	axisVector[2] = (c6 - c2) * 0.5f;
}

void Cube::SetHeight(float minY, float maxY)
{
	min.y = minY;
	max.y = maxY;
	center.y = (minY + maxY) * 0.5f;
	size.y = maxY - minY;

	TVector3 c0, c2;
	c2 = min;
	c0 = c2; c0.y += size.y;
	axisVector[1] = (c0 - c2) * 0.5f;
}

// --------------------------------- Plane ---------------------------------//


Plane::Plane(TVector3& p0, TVector3& p1, TVector3& p2)
{
	TVector3 v1 = p1 - p0;
	TVector3 v2 = p2 - p0;

	TVector3 tempNormal;
	v1.Cross(v2, tempNormal);

	tempNormal.Normalize();
	normal = TVector4(tempNormal.x, tempNormal.y, tempNormal.z, 0.0f);

	normal.w = -(normal.x * p0.x + normal.y * p0.y + normal.z * p0.z);
}

void Plane::SetPlane(TVector3& p0, TVector3& p1, TVector3& p2)
{
	TVector3 v1 = p1 - p0;
	TVector3 v2 = p2 - p0;

	TVector3 tempNormal;
	v1.Cross(v2, tempNormal);

	tempNormal.Normalize();
	normal = TVector4(tempNormal.x, tempNormal.y, tempNormal.z, 0.0f);

	normal.w = -(normal.x * p0.x + normal.y * p0.y + normal.z * p0.z);
}

// --------------------------------- Circle ---------------------------------//

Circle::Circle(TVector2 center, float radius)
{
	this->center = center;
	this->radius = radius;
}

void Circle::SetCircle(TVector2 center, float radius)
{
	this->center = center;
	this->radius = radius;
}