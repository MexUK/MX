#include "Math.h"
#include <algorithm>
#define _USE_MATH_DEFINES
#include <math.h>
#include <ctime> // purely for time() seed to srand()
#include "Components/OBB.h"

using namespace std;
using namespace glm;
using namespace mx;

float32			Math::convertDegreesToRadians(float32 fDegrees)
{
	return (float32) (fDegrees / 180.0f * M_PI);
}
float32			Math::convertRadiansToDegrees(float32 fRadians)
{
	return (float32) (fRadians * 180.0f / M_PI);
}
float32			Math::getDistanceBetweenPoints(vec2& vecPosition1, vec2& vecPosition2)
{
	vec2 vecDistance;
	vecDistance.x = vecPosition2.x - vecPosition1.x;
	vecDistance.y = vecPosition2.y - vecPosition1.y;
	return sqrt(vecDistance.x*vecDistance.x + vecDistance.y*vecDistance.y);
}

float32			Math::getDistanceBetweenPoints(vec3& vecPosition1, vec3& vecPosition2)
{
	vec3 vecDistance = vecPosition2 - vecPosition1;
	return sqrt(vecDistance.x*vecDistance.x + vecDistance.y*vecDistance.y + vecDistance.z*vecDistance.z);
}

float32				Math::getDistanceBetweenPoints(ivec2& vecPoint1, ivec2& vecPoint2)
{
	vec2 vecDistance;
	vecDistance.x = (float32)((float32)(vecPoint2.x) - (float32)(vecPoint1.x));
	vecDistance.y = (float32)((float32)(vecPoint2.y) - (float32)(vecPoint1.y));
	return sqrt(vecDistance.x*vecDistance.x + vecDistance.y*vecDistance.y);
}

float32			Math::getAngleBetweenPoints(vec2& vecPosition1, vec2& vecPosition2)
{
	vec2 vecDistance;
	vecDistance.x = vecPosition2.x - vecPosition1.x;
	vecDistance.y = vecPosition2.y - vecPosition1.y;
	return atan2(vecDistance.y, vecDistance.x);
}

float32			Math::getAngleBetweenPoints(vec3& vecPosition1, vec3& vecPosition2)
{
	vec3 vecDistance;
	vecDistance.x = vecPosition2.x - vecPosition1.x;
	vecDistance.y = vecPosition2.y - vecPosition1.y;
	return atan2(vecDistance.y, vecDistance.x);
}

vec2		Math::getPositionInFrontOfPosition(vec2& vecPosition, float32 fAngleRad, float32 fRadius)
{
	vec2 vecOutPosition(vecPosition);
	vecOutPosition.x += cos(fAngleRad) * fRadius;
	vecOutPosition.y += sin(fAngleRad) * fRadius;
	return vecOutPosition;
}

vec3		Math::getPositionInFrontOfPosition(vec3& vecPosition, float32 fAngleRad, float32 fRadius)
{
	vec3 vecOutPosition;
	vecOutPosition.x = vecPosition.x;
	vecOutPosition.y = vecPosition.y;
	vecOutPosition.x += cos(fAngleRad) * fRadius;
	vecOutPosition.y += sin(fAngleRad) * fRadius;
	return vecOutPosition;
}

vec4		Math::getPositionInFrontOfPosition(vec4& vecPosition, float32 fAngleRad, float32 fRadius)
{
	vec4 vecOutPosition;
	vecOutPosition.x = vecPosition.x;
	vecOutPosition.y = vecPosition.y;
	vecOutPosition.x += cos(fAngleRad) * fRadius;
	vecOutPosition.y += sin(fAngleRad) * fRadius;
	vecOutPosition.z = vecPosition.z;
	vecOutPosition.w = vecPosition.w;
	return vecOutPosition;
}

ivec2		Math::getPositionInFrontOfPosition(ivec2& vecPosition, float32 fAngleRad, float32 fRadius)
{
	ivec2 vecOutPosition(vecPosition);
	vecOutPosition.x += (int32)(cos(fAngleRad) * fRadius);
	vecOutPosition.y += (int32)(sin(fAngleRad) * fRadius);
	return vecOutPosition;
}

bool			Math::isPointInPolygon(vec2& vecPoint, vector<vec2>& vecPolygonPoints)
{
	// http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
	uint32 i, j;
	bool c = false;
	for (i = 0, j = vecPolygonPoints.size() - 1; i < vecPolygonPoints.size(); j = i++) {
		if (((vecPolygonPoints[i].y>vecPoint.y) != (vecPolygonPoints[j].y>vecPoint.y)) &&
			(vecPoint.x < (vecPolygonPoints[j].x - vecPolygonPoints[i].x) * (vecPoint.y - vecPolygonPoints[i].y) / (vecPolygonPoints[j].y - vecPolygonPoints[i].y) + vecPolygonPoints[i].x))
			c = !c;
	}
	return c;
}

int32_t				Math::getFactorial(int32_t iValue)
{
	int iOutValue = 1;
	for (int i = 1; i <= iValue; i++)
	{
		iOutValue *= i;
	}
	return iOutValue;
}

int32_t				Math::getBinomialCoefficient(int32_t n, int32_t k)
{
	uint32 iValue = 1;
	for (int32 i = 1; i <= k; i++)
	{
		iValue *= ((n + 1) - i) / i;
	}
	return iValue;
}

float32			Math::getLength(vec3& vecVector)
{
	return sqrt(vecVector.x*vecVector.x + vecVector.y*vecVector.y + vecVector.z*vecVector.z);
}

// velocity
vec2			Math::getVelocityFromSpeedAndDirection(float32 fSpeed, float32 fDirectionRad)
{
	return polarToCartesian(fSpeed, fDirectionRad);
}

// z rotation
float32			Math::getZDirection(vec2& vecVector)
{
	return atan2(vecVector.y, vecVector.x);
}

float32			Math::getZDirection(vec3& vecVector)
{
	vec2 vecVector2d(vecVector);
	return getZDirection(vecVector2d);
}

// polar/cartesian coordinates
vec2			Math::polarToCartesian(float32 fRadius, float32 fAngle)
{
	vec2 vecCartesian;
	vecCartesian.x = fRadius * cos(fAngle);
	vecCartesian.y = fRadius * sin(fAngle);
	return vecCartesian;
}

void			Math::cartesianToPolar(vec2& vecCartesian, float& fRadius, float& fAngle)
{
	fRadius = sqrt(vecCartesian.x*vecCartesian.x + vecCartesian.y*vecCartesian.y);
	fAngle = atan2(vecCartesian.y, vecCartesian.x);
}

// spherical/cartesian coordinates
vec3			Math::sphericalToCartesian(float32 fRadius, float32 fInclination, float32 fAzimuthal)
{
	vec3 vecCartesian;
	vecCartesian.x = fRadius * sin(fInclination) * cos(fAzimuthal);
	vecCartesian.y = fRadius * sin(fInclination) * sin(fAzimuthal);
	vecCartesian.z = fRadius * cos(fInclination);
	return vecCartesian;
}

void			Math::cartesianToSpherical(vec3& vecCartesian, float& fRadius, float& fInclination, float& fAzimuthal)
{
	fRadius = sqrt(vecCartesian.x*vecCartesian.x + vecCartesian.y*vecCartesian.y + vecCartesian.z*vecCartesian.z);
	fInclination = acos(vecCartesian.z / fRadius);
	fAzimuthal = atan2(vecCartesian.y, vecCartesian.x);
}

float			Math::cartesianToSphericalInclination(vec3& vecCartesian)
{
	float fRadius = sqrt(vecCartesian.x*vecCartesian.x + vecCartesian.y*vecCartesian.y + vecCartesian.z*vecCartesian.z);
	return acos(vecCartesian.z / fRadius);
}

// polygon
vec3		Math::getPolygonCenter(vector<vec3>& vecPoints)
{
	// http://stackoverflow.com/questions/22465832/center-point-calculation-for-3d-polygon
	vec3 *s = new vec3;
	float32 areaTotal = 0.0;

	vec3 p1 = vecPoints[0];
	vec3 p2 = vecPoints[1];

	for (int i = 2, j = vecPoints.size(); i < j; i++)
	{
		vec3 p3 = vecPoints[i];
		vec3 edge1 = p3 - p1;
		vec3 edge2 = p3 - p2;

		vec3 crossProduct = cross(edge1, edge2);
		float32 area = length(crossProduct) / 2.0f;

		s->x += area * (p1.x + p2.x + p3.x) / 3;
		s->y += area * (p1.y + p2.y + p3.y) / 3;
		s->z += area * (p1.z + p2.z + p3.z) / 3;

		areaTotal += area;
		p2 = p3;
	}

	vec3 vecPoint;
	vecPoint.x = s->x / areaTotal;
	vecPoint.y = s->y / areaTotal;
	vecPoint.z = s->z / areaTotal;
	return vecPoint;
}

// cuboids
bool			Math::isPointInCuboid(vec3& vecPointPosition, vec3& vecCuboidPosition, vec3& vecCuboidSize)
{
	vec3 vecCuboidMin = vecCuboidPosition;
	vec3 vecCuboidMax = vecCuboidPosition + vecCuboidSize;
	return vecCuboidMin.x <= vecPointPosition.x && vecPointPosition.x <= vecCuboidMax.x
		&& vecCuboidMin.y <= vecPointPosition.y && vecPointPosition.y <= vecCuboidMax.y
		&& vecCuboidMin.z <= vecPointPosition.z && vecPointPosition.z <= vecCuboidMax.z;
}

bool			Math::doesLineIntersectCuboidAA(vec3& vecCuboidPosition, vec3& vecCuboidSize, vec3& vecLinePoint1, vec3& vecLinePoint2, float& t)
{
	vec3& vecCuboidMin = vecCuboidPosition;
	vec3 vecCuboidMax = vecCuboidPosition + vecCuboidSize;

	vec3 ray_direction(0, 0, 0);
	//ray_direction.x = getAngleBetweenPoints(vec3(ray_start.x, ray_start.z, 0), vec3(ray_end.x, ray_end.z, 0));
	//ray_direction.y = getAngleBetweenPoints(vec3(ray_start.y, ray_start.z, 0), vec3(ray_end.y, ray_end.z, 0));
	//ray_direction.z = Math::getAngleBetweenPoints(vecLinePoint1, vecLinePoint2);
	vec3 vecDiff = vecLinePoint2 - vecLinePoint1;
	ray_direction.z = Math::cartesianToSphericalInclination(vecDiff);

	vec3 dirfrac;
	// r.dir is unit direction vector of ray
	dirfrac.x = 1.0f / ray_direction.x;
	dirfrac.y = 1.0f / ray_direction.y;
	dirfrac.z = 1.0f / ray_direction.z;
	// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	// r.org is origin of ray
	float t1 = (vecCuboidMin.x - vecLinePoint1.x)*dirfrac.x;
	float t2 = (vecCuboidMax.x - vecLinePoint1.x)*dirfrac.x;
	float t3 = (vecCuboidMin.y - vecLinePoint1.y)*dirfrac.y;
	float t4 = (vecCuboidMax.y - vecLinePoint1.y)*dirfrac.y;
	float t5 = (vecCuboidMin.z - vecLinePoint1.z)*dirfrac.z;
	float t6 = (vecCuboidMax.z - vecLinePoint1.z)*dirfrac.z;

	float tmin = glm::max(glm::max(glm::min(t1, t2), glm::min(t3, t4)), glm::min(t5, t6));
	float tmax = glm::min(glm::min(glm::max(t1, t2), glm::max(t3, t4)), glm::max(t5, t6));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	t = tmin;
	return true;
}

bool			Math::doesLineIntersectCuboidOBB(
	vec3& ray_origin,				// Ray origin, in world space
	vec3& ray_direction,			// Ray direction (NOT target position!), in world space. Must be normalize()'d.
	vec3& aabb_min,					// Minimum X,Y,Z coords of the mesh when not transformed at all.
	vec3& aabb_max,					// Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
	mat4& ModelMatrix,				// Transformation applied to the mesh (which will thus be also applied to its bounding box)
	float& intersection_distance	// Output : distance between ray_origin and the intersection with the OBB
)
{
	// Intersection method from Real-Time Rendering and Essential Mathematics for Games

	float tMin = 0.0f;
	float tMax = 100000.0f;

	vec3 OBBposition_worldspace(ModelMatrix[3].x, ModelMatrix[3].y, ModelMatrix[3].z);

	vec3 delta = OBBposition_worldspace - ray_origin;

	// Test intersection with the 2 planes perpendicular to the OBB's X axis
	{
		vec3 xaxis(ModelMatrix[0].x, ModelMatrix[0].y, ModelMatrix[0].z);
		float e = dot(xaxis, delta);
		float f = dot(ray_direction, xaxis);

		if (fabs(f) > 0.001f) { // Standard case

			float t1 = (e + aabb_min.x) / f; // Intersection with the "left" plane
			float t2 = (e + aabb_max.x) / f; // Intersection with the "right" plane
			// t1 and t2 now contain distances betwen ray origin and ray-plane intersections

			// We want t1 to represent the nearest intersection, 
			// so if it's not the case, invert t1 and t2
			if (t1 > t2) {
				float w = t1; t1 = t2; t2 = w; // swap t1 and t2
			}

			// tMax is the nearest "far" intersection (amongst the X,Y and Z planes pairs)
			if (t2 < tMax)
				tMax = t2;
			// tMin is the farthest "near" intersection (amongst the X,Y and Z planes pairs)
			if (t1 > tMin)
				tMin = t1;

			// And here's the trick :
			// If "far" is closer than "near", then there is NO intersection.
			// See the images in the tutorials for the visual explanation.
			if (tMax < tMin)
				return false;

		}
		else { // Rare case : the ray is almost parallel to the planes, so they don't have any "intersection"
			if (-e + aabb_min.x > 0.0f || -e + aabb_max.x < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Y axis
	// Exactly the same thing than above.
	{
		vec3 yaxis(ModelMatrix[1].x, ModelMatrix[1].y, ModelMatrix[1].z);
		float e = dot(yaxis, delta);
		float f = dot(ray_direction, yaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabb_min.y) / f;
			float t2 = (e + aabb_max.y) / f;

			if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + aabb_min.y > 0.0f || -e + aabb_max.y < 0.0f)
				return false;
		}
	}


	// Test intersection with the 2 planes perpendicular to the OBB's Z axis
	// Exactly the same thing than above.
	{
		vec3 zaxis(ModelMatrix[2].x, ModelMatrix[2].y, ModelMatrix[2].z);
		float e = dot(zaxis, delta);
		float f = dot(ray_direction, zaxis);

		if (fabs(f) > 0.001f) {

			float t1 = (e + aabb_min.z) / f;
			float t2 = (e + aabb_max.z) / f;

			if (t1 > t2) { float w = t1; t1 = t2; t2 = w; }

			if (t2 < tMax)
				tMax = t2;
			if (t1 > tMin)
				tMin = t1;
			if (tMin > tMax)
				return false;

		}
		else {
			if (-e + aabb_min.z > 0.0f || -e + aabb_max.z < 0.0f)
				return false;
		}
	}

	intersection_distance = tMin;
	return true;

}

bool			Math::doCuboidsInteresectAA(vec3& vecCuboid1Position, vec3& vecCuboid1Size, vec3& vecCuboid2Position, vec3& vecCuboid2Size)
{
	vec3 vecCuboid1Max = vecCuboid1Position + vecCuboid1Size;
	vec3 vecCuboid2Max = vecCuboid2Position + vecCuboid2Size;
	return vecCuboid2Position.x <= vecCuboid1Position.x && vecCuboid1Position.x <= vecCuboid2Max.x
		&& vecCuboid2Position.y <= vecCuboid1Position.y && vecCuboid1Position.y <= vecCuboid2Max.y
		&& vecCuboid2Position.z <= vecCuboid1Position.z && vecCuboid1Position.z <= vecCuboid2Max.z;
}

/*
// https://github.com/juj/MathGeoLib/blob/master/src/Geometry/OBB.cpp

/// Computes the dot product of two 3D vectors, the elements are accessed using array notation.
/// @param v1 A vector of type float3, or a C array of three elements.
/// @param v2 A vector of type float3, or a C array of three elements.
/// @see DOT2(), DOT4(), ABSDOT3(), DOT3_xyz(), DOT3STRIDED().
#define DOT3(v1, v2) ((v1)[0] * (v2)[0] + (v1)[1] * (v2)[1] + (v1)[2] * (v2)[2])

#define DIR_VEC(...) float3(__VA_ARGS__)
*/

// https://stackoverflow.com/questions/47866571/simple-oriented-bounding-box-obb-collision-detection-explaining?rq=1

// check if there's a separating plane in between the selected axes
bool getSeparatingPlane(const vec3& RPos, const vec3& Plane, const OBB& box1, const OBB&box2)
{
	return (fabs(dot(RPos, Plane)) >
		(fabs(dot((box1.AxisX*box1.Half_size.x), Plane)) +
			fabs(dot((box1.AxisY*box1.Half_size.y), Plane)) +
			fabs(dot((box1.AxisZ*box1.Half_size.z), Plane)) +
			fabs(dot((box2.AxisX*box2.Half_size.x), Plane)) +
			fabs(dot((box2.AxisY*box2.Half_size.y), Plane)) +
			fabs(dot((box2.AxisZ*box2.Half_size.z), Plane))));
}

// test for separating planes in all 15 axes
bool getCollision(const OBB& box1, const OBB&box2)
{
	static vec3 RPos;
	RPos = box2.Pos - box1.Pos;

	return !(getSeparatingPlane(RPos, box1.AxisX, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisY, box1, box2) ||
		getSeparatingPlane(RPos, box1.AxisZ, box1, box2) ||
		getSeparatingPlane(RPos, box2.AxisX, box1, box2) ||
		getSeparatingPlane(RPos, box2.AxisY, box1, box2) ||
		getSeparatingPlane(RPos, box2.AxisZ, box1, box2) ||
		getSeparatingPlane(RPos, cross(box1.AxisX, box2.AxisX), box1, box2) ||
		getSeparatingPlane(RPos, cross(box1.AxisX, box2.AxisY), box1, box2) ||
		getSeparatingPlane(RPos, cross(box1.AxisX, box2.AxisZ), box1, box2) ||
		getSeparatingPlane(RPos, cross(box1.AxisY, box2.AxisX), box1, box2) ||
		getSeparatingPlane(RPos, cross(box1.AxisY, box2.AxisY), box1, box2) ||
		getSeparatingPlane(RPos, cross(box1.AxisY, box2.AxisZ), box1, box2) ||
		getSeparatingPlane(RPos, cross(box1.AxisZ, box2.AxisX), box1, box2) ||
		getSeparatingPlane(RPos, cross(box1.AxisZ, box2.AxisY), box1, box2) ||
		getSeparatingPlane(RPos, cross(box1.AxisZ, box2.AxisZ), box1, box2));
}

/*
// a quick test to see the code working
int _tmain(int argc, _TCHAR* argv[])
{
	// create two obbs
	OBB A, B;

	// set the first obb's properties
	A.Pos = { 0.f, 0.f, 0.f }; // set its center position

	// set the half size
	A.Half_size.x = 10.f;
	A.Half_size.y = 1.f;
	A.Half_size.z = 1.f;

	// set the axes orientation
	A.AxisX = { 1.f, 0.f, 0.f };
	A.AxisY = { 0.f, 1.f, 0.f };
	A.AxisZ = { 0.f, 0.f, 1.f };

	// set the second obb's properties
	B.Pos = { 20.f, 0.f, 0.f }; // set its center position

	// set the half size
	B.Half_size.x = 10.f;
	B.Half_size.y = 1.f;
	B.Half_size.z = 1.f;

	// set the axes orientation
	B.AxisX = { 1.f, 0.f, 0.f };
	B.AxisY = { 0.f, 1.f, 0.f };
	B.AxisZ = { 0.f, 0.f, 1.f };

	// run the code and get the result as a message
	if (getCollision(A, B)) cout << "Collision!!!" << endl;
	else cout << "No collision." << endl;

	// pause and quit
	cout << endl;
	system("pause");
	return 0;
}
*/

vec3		getQuatAxisAngle(quat q1) {
	if (q1.w > 1.0f) q1 = normalize(q1); // if w>1 acos and sqrt will produce errors, this cant happen if quaternion is normalised
	float angle = 2.0f * acosf(q1.w);
	float s = sqrtf(1.0f - q1.w*q1.w); // assuming quaternion normalised then w is less than 1, so term always positive.
	vec3 v;
	if (s < 0.001f) { // test to avoid divide by zero, s is always positive due to sqrt
	  // if s close to zero then direction of axis not important
		v.x = q1.x; // if it is important that axis is normalised then replace with x=1; y=z=0;
		v.y = q1.y;
		v.z = q1.z;
	}
	else {
		v.x = q1.x / s; // normalise axis
		v.y = q1.y / s;
		v.z = q1.z / s;
	}
	return v;
}

bool			Math::doCuboidsOverlap(vec3& vecCuboid1Position, vec3& vecCuboid1Size, mat4& matCuboid1Model, vec3& vecCuboid2Position, vec3& vecCuboid2Size, mat4& matCuboid2Model)
{
	OBB box1, box2;

	vec3 scale1;
	quat rotation1;
	vec3 translation1;
	vec3 skew1;
	vec4 perspective1;
	decompose(matCuboid1Model, scale1, rotation1, translation1, skew1, perspective1);
	rotation1 = conjugate(rotation1);

	vec3 scale2;
	quat rotation2;
	vec3 translation2;
	vec3 skew2;
	vec4 perspective2;
	decompose(matCuboid2Model, scale2, rotation2, translation2, skew2, perspective2);
	rotation2 = conjugate(rotation2);

	box1.Pos = vecCuboid1Position;
	box1.Half_size = vecCuboid1Size / 2.0f;

	box2.Pos = vecCuboid2Position;
	box2.Half_size = vecCuboid2Size / 2.0f;

	///*
	box1.AxisX = vec3(1.0f, 0.0f, 0.0f);
	box1.AxisY = vec3(0.0f, 1.0f, 0.0f);
	box1.AxisZ = vec3(0.0f, 0.0f, 1.0f);

	box2.AxisX = vec3(1.0f, 0.0f, 0.0f);
	box2.AxisY = vec3(0.0f, 1.0f, 0.0f);
	box2.AxisZ = vec3(0.0f, 0.0f, 1.0f);
	//*/

	/*
	vec3 v = getQuatAxisAngle(rotation1);
	box1.AxisX = vec3(v.x, 0.0f, 0.0f);// * scale1;
	box1.AxisY = vec3(0.0f, v.y, 0.0f);// * scale1;
	box1.AxisZ = vec3(0.0f, 0.0f, v.z);// * scale1;
	*/
	
	/*
	mat3 matRot2 = mat3(matCuboid2Model);
	box2.AxisX = matRot2[0];
	box2.AxisY = matRot2[1];
	box2.AxisZ = matRot2[2];
	*/

	/*
	quat quatRot2 = quat_cast(matCuboid2Model);
	box2.AxisX = vec3(eulerAngles(quatRot2).x, 0.0f, 0.0f);
	box2.AxisY = vec3(0.0f, eulerAngles(quatRot2).y, 0.0f);
	box2.AxisZ = vec3(0.0f, 0.0f, eulerAngles(quatRot2).z);
	*/

	/*
	v = getQuatAxisAngle(rotation2);
	box2.AxisX = vec3(v.x, 0.0f, 0.0f);// * scale2;
	box2.AxisY = vec3(0.0f, v.y, 0.0f);// * scale2;
	box2.AxisZ = vec3(0.0f, 0.0f, v.z);// * scale2;
	*/

	return getCollision(box1, box2);
}

bool			Math::doCuboidsInteresectOBB(vec3& vecCuboid1Position, vec3& vecCuboid1Size, mat4& matCuboid1Model, vec3& vecCuboid2Position, vec3& vecCuboid2Size, mat4& matCuboid2Model, float& fOutput)
{
	return doCuboidsOverlap(vecCuboid1Position, vecCuboid1Size, matCuboid1Model, vecCuboid2Position, vecCuboid2Size, matCuboid2Model);

	/*
	bool c1 = doCuboidsOverlap(vecCuboid1Position, vecCuboid1Size, matCuboid1Model, vecCuboid2Position, vecCuboid2Size, matCuboid2Model), c2 = false;
	if (c1)
	{
		c2 = doCuboidsOverlap(vecCuboid2Position, vecCuboid2Size, matCuboid2Model, vecCuboid1Position, vecCuboid1Size, matCuboid1Model);
	}
	return c1 && c2;
	*/
}

bool			Math::doesCuboidIntersectSphere(vec3& vecCuboidPosition, vec3& vecCuboidSize, vec3& vecSpherePosition, float fSphereRadius)
{
	vec3& vecCuboidMin = vecCuboidPosition;
	vec3 vecCuboidMax = vecCuboidPosition + vecCuboidSize;
	float r2 = fSphereRadius * fSphereRadius;
	float dmin = 0;
	for(int i = 0; i < 3; i++)
	{
		if(vecSpherePosition[i] < vecCuboidMin[i])
			dmin += sqrtf(vecSpherePosition[i] - vecCuboidMin[i]);
		else if(vecSpherePosition[i] > vecCuboidMax[i])
			dmin += sqrtf(vecSpherePosition[i] - vecCuboidMax[i]);
	}
	return dmin <= r2;
}

vec3			Math::getBoundingCuboidMinFromSphere(vec3& vecPosition, float32 fRadius)
{
	return vecPosition - fRadius;
}

vec3			Math::getBoundingCuboidMaxFromSphere(vec3& vecPosition, float32 fRadius)
{
	return vecPosition + fRadius;
}

// spheres
bool				Math::isPointInSphere(vec3& vecPointPosition, vec3& vecSpherePosition, float32 fRadius)
{
	vec3 vecDiff = vecPointPosition - vecSpherePosition;
	return (vecDiff.x*vecDiff.x) + (vecDiff.y*vecDiff.y) + (vecDiff.z*vecDiff.z) < fRadius*fRadius;
}

bool				Math::doesLineIntersectSphere(vec3& vecSpherePosition, float32 fRadius, vec3& vecLinePoint1, vec3& vecLinePoint2)
{
	return true; // todo
}

bool				Math::doSpheresIntersect(vec3& vecSphere1Position, float fSphere1Radius, vec3& vecSphere2Position, float32 fSphere2Radius)
{
	return Math::getDistanceBetweenPoints(vecSphere1Position, vecSphere2Position) < (fSphere1Radius + fSphere2Radius);
}

vector<vec3>	Math::getSphereVertices(vec3& vecCenter, float32 fRadius, uint32 uiRingCount)
{
	vector<vec3> vecVertexPositions;

	float32 UResolution = (float32)(uiRingCount / 2);
	float32 VResolution = (float32)(uiRingCount / 2);

	float32 startU = 0.0f;
	float32 startV = 0.0f;
	float32 endU = (float32)M_PI * 2.0f;
	float32 endV = (float32)M_PI;
	float32 stepU = (endU - startU) / UResolution; // step size between U-points on the grid
	float32 stepV = (endV - startV) / VResolution; // step size between V-points on the grid

	auto F = [fRadius](float32 u, float32 v) -> vec3
	{
		return vec3(cos(u)*sin(v)*fRadius, cos(v)*fRadius, sin(u)*sin(v)*fRadius);
	};

	for (int i = 0; i < (int)UResolution; i++)
	{
		// U-points
		for (int j = 0; j < (int)VResolution; j++)
		{
			// V-points
			float32 u = i * stepU + startU;
			float32 v = j * stepV + startV;
			float32 un = (i + 1 == UResolution) ? endU : (i + 1)*stepU + startU;
			float32 vn = (j + 1 == VResolution) ? endV : (j + 1)*stepV + startV;
			// Find the four points of the grid
			// square by evaluating the parametric
			// surface function
			vec3 p0 = F(u, v);
			vec3 p1 = F(u, vn);
			vec3 p2 = F(un, v);
			vec3 p3 = F(un, vn);
			// NOTE: For spheres, the normal is just the normalized
			// version of each vertex point; this generally won't be the case for
			// other parametric surfaces.
			// Output the first triangle of this grid square
			//triangle(p0, p2, p1);
			// Output the other triangle of this grid square
			//triangle(p3, p1, p2);

			vecVertexPositions.push_back(vecCenter + p0);
			vecVertexPositions.push_back(vecCenter + p2);
			vecVertexPositions.push_back(vecCenter + p1);

			vecVertexPositions.push_back(vecCenter + p3);
			vecVertexPositions.push_back(vecCenter + p1);
			vecVertexPositions.push_back(vecCenter + p2);
		}
	}

	return vecVertexPositions;
}

vector<vector<vec3>>	Math::getBoundingSphereVertices(vec3& vecCenter, float32 fRadius, uint32 uiPointCount) // todo: uiPointCount
{
	vector<vector<vec3>> vecVertices;
	vecVertices.resize(3); // 3 rings
	for (uint32 i = 0, j = 3, step = 1; i < j; i += step)
	{
		for (uint32 i2 = 0, j2 = 360, step2 = 1; i2 < j2; i2 += step2)
		{
			vec3 vecPosition;
			switch (i)
			{
			case 0:
				vecPosition = Math::sphericalToCartesian(fRadius, Math::convertDegreesToRadians((float32)i2), Math::convertDegreesToRadians(0.0f));
				break;
			case 1:
				vecPosition = Math::sphericalToCartesian(fRadius, Math::convertDegreesToRadians(90.0f), Math::convertDegreesToRadians((float32)i2));
				break;
			case 2:
				vecPosition = Math::sphericalToCartesian(fRadius, Math::convertDegreesToRadians((float32)i2), Math::convertDegreesToRadians(90.0f));
				break;
			}

			vec3 vecPosition2 = vec3(vecPosition.x, vecPosition.y, vecPosition.z);
			vecPosition2 += vecCenter;

			vecVertices[i].push_back(vecPosition2);
		}
	}
	return vecVertices;
}

vector<vec3>	Math::getCuboidVertices(vec3& vecMinPosition, vec3& vecMaxPosition)
{
	vec3 p0, p1, p2, p3;
	vector<vec3> vecVertices;
	vecVertices.resize(36);

	// top (y = max)
	p0 = vec3(vecMaxPosition.x, vecMaxPosition.y, vecMinPosition.z);
	p1 = vec3(vecMaxPosition.x, vecMaxPosition.y, vecMaxPosition.z);
	p2 = vec3(vecMinPosition.x, vecMaxPosition.y, vecMaxPosition.z);
	p3 = vec3(vecMinPosition.x, vecMaxPosition.y, vecMinPosition.z);

	vecVertices[0] = p0;
	vecVertices[1] = p1;
	vecVertices[2] = p2;
	vecVertices[3] = p2;
	vecVertices[4] = p3;
	vecVertices[5] = p0;

	// bottom (y = min)
	p0 = vec3(vecMaxPosition.x, vecMinPosition.y, vecMaxPosition.z);
	p1 = vec3(vecMinPosition.x, vecMinPosition.y, vecMaxPosition.z);
	p2 = vec3(vecMinPosition.x, vecMinPosition.y, vecMinPosition.z);
	p3 = vec3(vecMaxPosition.x, vecMinPosition.y, vecMinPosition.z);

	vecVertices[6] = p0;
	vecVertices[7] = p1;
	vecVertices[8] = p2;
	vecVertices[9] = p2;
	vecVertices[10] = p3;
	vecVertices[11] = p0;

	// front (z = max)
	p0 = vec3(vecMaxPosition.x, vecMaxPosition.y, vecMaxPosition.z);
	p1 = vec3(vecMinPosition.x, vecMaxPosition.y, vecMaxPosition.z);
	p2 = vec3(vecMinPosition.x, vecMinPosition.y, vecMaxPosition.z);
	p3 = vec3(vecMaxPosition.x, vecMinPosition.y, vecMaxPosition.z);

	vecVertices[12] = p0;
	vecVertices[13] = p1;
	vecVertices[14] = p2;
	vecVertices[15] = p2;
	vecVertices[16] = p3;
	vecVertices[17] = p0;

	// back (z = min)
	p0 = vec3(vecMaxPosition.x, vecMinPosition.y, vecMinPosition.z);
	p1 = vec3(vecMinPosition.x, vecMinPosition.y, vecMinPosition.z);
	p2 = vec3(vecMinPosition.x, vecMaxPosition.y, vecMinPosition.z);
	p3 = vec3(vecMaxPosition.x, vecMaxPosition.y, vecMinPosition.z);

	vecVertices[18] = p0;
	vecVertices[19] = p1;
	vecVertices[20] = p2;
	vecVertices[21] = p2;
	vecVertices[22] = p3;
	vecVertices[23] = p0;

	// left (x = min)
	p0 = vec3(vecMinPosition.x, vecMaxPosition.y, vecMaxPosition.z);
	p1 = vec3(vecMinPosition.x, vecMaxPosition.y, vecMinPosition.z);
	p2 = vec3(vecMinPosition.x, vecMinPosition.y, vecMinPosition.z);
	p3 = vec3(vecMinPosition.x, vecMinPosition.y, vecMaxPosition.z);

	vecVertices[24] = p0;
	vecVertices[25] = p1;
	vecVertices[26] = p2;
	vecVertices[27] = p2;
	vecVertices[28] = p3;
	vecVertices[29] = p0;

	// right (x = max)
	p0 = vec3(vecMaxPosition.x, vecMaxPosition.y, vecMinPosition.z);
	p1 = vec3(vecMaxPosition.x, vecMaxPosition.y, vecMaxPosition.z);
	p2 = vec3(vecMaxPosition.x, vecMinPosition.y, vecMaxPosition.z);
	p3 = vec3(vecMaxPosition.x, vecMinPosition.y, vecMinPosition.z);

	vecVertices[30] = p0;
	vecVertices[31] = p1;
	vecVertices[32] = p2;
	vecVertices[33] = p2;
	vecVertices[34] = p3;
	vecVertices[35] = p0;

	return vecVertices;
}

vector<vec3>	Math::getBoundingCuboidVertices(vec3& vecMinPosition, vec3& vecMaxPosition)
{
	vec3 p0, p1, p2, p3;
	vector<vec3> vecVertices;
	vecVertices.resize(24);

	// top (y = max)
	p0 = vec3(vecMaxPosition.x, vecMaxPosition.y, vecMinPosition.z);
	p1 = vec3(vecMaxPosition.x, vecMaxPosition.y, vecMaxPosition.z);
	p2 = vec3(vecMinPosition.x, vecMaxPosition.y, vecMaxPosition.z);
	p3 = vec3(vecMinPosition.x, vecMaxPosition.y, vecMinPosition.z);

	vecVertices[0] = p0;
	vecVertices[1] = p1;

	vecVertices[2] = p1;
	vecVertices[3] = p2;

	vecVertices[4] = p2;
	vecVertices[5] = p3;

	vecVertices[6] = p3;
	vecVertices[7] = p0;

	// bottom (y = min)
	p0 = vec3(vecMaxPosition.x, vecMinPosition.y, vecMaxPosition.z);
	p1 = vec3(vecMinPosition.x, vecMinPosition.y, vecMaxPosition.z);
	p2 = vec3(vecMinPosition.x, vecMinPosition.y, vecMinPosition.z);
	p3 = vec3(vecMaxPosition.x, vecMinPosition.y, vecMinPosition.z);

	vecVertices[8] = p0;
	vecVertices[9] = p1;

	vecVertices[10] = p1;
	vecVertices[11] = p2;

	vecVertices[12] = p2;
	vecVertices[13] = p3;

	vecVertices[14] = p3;
	vecVertices[15] = p0;

	// vertical sides
	vecVertices[16] = vecVertices[0];
	vecVertices[17] = vecVertices[14];

	vecVertices[18] = vecVertices[2];
	vecVertices[19] = vecVertices[8];

	vecVertices[20] = vecVertices[4];
	vecVertices[21] = vecVertices[10];

	vecVertices[22] = vecVertices[6];
	vecVertices[23] = vecVertices[12];

	return vecVertices;
}

vector<vec3>	Math::getOBBLines(vec3& vecMinAABB, vec3& vecMaxAABB, mat4& matCuboidModel)
{
	vector<vec3> vecLinePoints;
	vecLinePoints.resize(4 * 2 * 3);

	// left 4 lines
	vector<vec3> vecLeftEdgePointsAABB =
	{
		vecMinAABB,
		vec3(vecMinAABB.x, vecMinAABB.y, vecMaxAABB.z),
		vec3(vecMinAABB.x, vecMaxAABB.y, vecMaxAABB.z),
		vec3(vecMinAABB.x, vecMaxAABB.y, vecMinAABB.z),
	};

	// right 4 lines
	vector<vec3> vecRightEdgePointsAABB =
	{
		vecMaxAABB,
		vec3(vecMaxAABB.x, vecMaxAABB.y, vecMinAABB.z),
		vec3(vecMaxAABB.x, vecMinAABB.y, vecMinAABB.z),
		vec3(vecMaxAABB.x, vecMinAABB.y, vecMaxAABB.z)
	};

	// side 4 lines
	vector<vec3> vecSideLinePointsAABB =
	{
		vecLeftEdgePointsAABB[0],
		vecRightEdgePointsAABB[2],
		vecLeftEdgePointsAABB[1],
		vecRightEdgePointsAABB[3],
		vecLeftEdgePointsAABB[2],
		vecRightEdgePointsAABB[0],
		vecLeftEdgePointsAABB[3],
		vecRightEdgePointsAABB[1]
	};

	uint32 i2 = 0;
	for (uint32 i = 0, j = vecLeftEdgePointsAABB.size(); i < j; i++)
	{
		vecLinePoints[i2++] = vec3(translate(matCuboidModel, vecLeftEdgePointsAABB[i])[3]);
		vecLinePoints[i2++] = vec3(translate(matCuboidModel, vecLeftEdgePointsAABB[(i == (j - 1)) ? 0 : (i + 1)])[3]);
	}
	for (uint32 i = 0, j = vecRightEdgePointsAABB.size(); i < j; i++)
	{
		vecLinePoints[i2++] = vec3(translate(matCuboidModel, vecRightEdgePointsAABB[i])[3]);
		vecLinePoints[i2++] = vec3(translate(matCuboidModel, vecRightEdgePointsAABB[(i == (j - 1)) ? 0 : (i + 1)])[3]);
	}
	for (uint32 i = 0, j = vecSideLinePointsAABB.size(); i < j; i++)
	{
		vecLinePoints[i2++] = vec3(translate(matCuboidModel, vecSideLinePointsAABB[i])[3]);
	}

	return vecLinePoints;
}

int32			Math::limit(int32 iValue, int32 iMin, int32 iMax)
{
	if (iValue < iMin)
	{
		iValue = iMin;
	}
	else if (iValue > iMax)
	{
		iValue = iMax;
	}
	return iValue;
}

uint32			Math::limit(uint32 uiValue, uint32 uiMin, uint32 uiMax)
{
	if (uiValue < uiMin)
	{
		uiValue = uiMin;
	}
	else if (uiValue > uiMax)
	{
		uiValue = uiMax;
	}
	return uiValue;
}
float32				Math::limit(float32 fValue, float32 fMin, float32 fMax)
{
	if (fValue < fMin)
	{
		fValue = fMin;
	}
	else if (fValue > fMax)
	{
		fValue = fMax;
	}
	return fValue;
}

float32				Math::limitAngle(float32 fAngle, bool bIsDeg)
{
	const float32 fOffset = bIsDeg ? 360.0f : ((float32)M_PI * 2.0f);
	while (fAngle < -180.0f) fAngle += fOffset;
	while (fAngle > 180.0f) fAngle -= fOffset;
	return fAngle;
}

float32				Math::constrainAngle(float32 fAngle) // The input parameter is in radians.
{
	fAngle = degrees(fAngle);
	fAngle = fmod(fAngle, 360.0f);
	if (fAngle < 0.0f)
		fAngle += 360.0f;
	return radians(fAngle);
}

vector<vec3>		Math::getCuboidFaceVerticesAsQuads(vec3& vecMinPosition, vec3& vecMaxPosition)
{
	vector<vec3> vecVertices;

	// top (y = max)
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMaxPosition.y, vecMinPosition.z));
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMaxPosition.y, vecMaxPosition.z));
	vecVertices.push_back(vec3(vecMinPosition.x, vecMaxPosition.y, vecMaxPosition.z));
	vecVertices.push_back(vec3(vecMinPosition.x, vecMaxPosition.y, vecMinPosition.z));

	// bottom (y = min)
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMinPosition.y, vecMaxPosition.z));
	vecVertices.push_back(vec3(vecMinPosition.x, vecMinPosition.y, vecMaxPosition.z));
	vecVertices.push_back(vec3(vecMinPosition.x, vecMinPosition.y, vecMinPosition.z));
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMinPosition.y, vecMinPosition.z)); 

	// front (z = max)
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMaxPosition.y, vecMaxPosition.z)); 
	vecVertices.push_back(vec3(vecMinPosition.x, vecMaxPosition.y, vecMaxPosition.z));
	vecVertices.push_back(vec3(vecMinPosition.x, vecMinPosition.y, vecMaxPosition.z));
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMinPosition.y, vecMaxPosition.z)); 

	// back (z = min)
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMinPosition.y, vecMinPosition.z));
	vecVertices.push_back(vec3(vecMinPosition.x, vecMinPosition.y, vecMinPosition.z));
	vecVertices.push_back(vec3(vecMinPosition.x, vecMaxPosition.y, vecMinPosition.z));
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMaxPosition.y, vecMinPosition.z));

	// left (x = min)
	vecVertices.push_back(vec3(vecMinPosition.x, vecMaxPosition.y, vecMaxPosition.z));
	vecVertices.push_back(vec3(vecMinPosition.x, vecMaxPosition.y, vecMinPosition.z));
	vecVertices.push_back(vec3(vecMinPosition.x, vecMinPosition.y, vecMinPosition.z));
	vecVertices.push_back(vec3(vecMinPosition.x, vecMinPosition.y, vecMaxPosition.z));

	// right (x = max)
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMaxPosition.y, vecMinPosition.z));
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMaxPosition.y, vecMaxPosition.z));
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMinPosition.y, vecMaxPosition.z));
	vecVertices.push_back(vec3(vecMaxPosition.x, vecMinPosition.y, vecMinPosition.z));

	return vecVertices;
}

uint32				Math::getUint32(uint8 a, uint8 b, uint8 c, uint8 d, bool bBigEndian)
{
	if(bBigEndian)
	{
		return d +
			(c * 256) +
			(b * 65536) +
			(a * 16777216);
	}
	else
	{
		return a +
			(b * 256) +
			(c * 65536) +
			(d * 16777216);
	}
}

bool				Math::isPointInEllipse(ivec2& vecPoint, ivec2& vecPosition, uvec2& vecSize)
{
	int32 iNumberX1 = (int32)(vecPoint.x - vecPosition.x);
	int32 iNumberY1 = (int32)(vecPoint.y - vecPosition.y);
	int32 iNumberX2 = (int32)((iNumberX1 * iNumberX1) / (vecSize.x * vecSize.x));
	int32 iNumberY2 = (int32)((iNumberY1 * iNumberY1) / (vecSize.y * vecSize.y));
	return (iNumberX2 + iNumberY2) <= 1;
}

bool				Math::isPointOnLine(ivec2& vecPoint, ivec2& vecLinePoint1, ivec2& vecLinePoint2)
{
	int32
		iDiffX1 = vecPoint.x - vecLinePoint1.x,
		iDiffX2 = vecLinePoint2.x - vecLinePoint1.x,
		iDiffY1 = vecPoint.y - vecLinePoint1.y,
		iDiffY2 = vecLinePoint2.y - vecLinePoint1.y;
	return ((iDiffX1 == 0 || iDiffX2 == 0) ? 0 : (iDiffX2 / iDiffX1)) == ((iDiffY1 == 0 || iDiffY2 == 0) ? 0 : (iDiffY2 / iDiffY1));
}

bool				Math::isPointInPolygon(ivec2& vecPoint, vector<ivec2>& vecPolygonPoints)
{
	// http://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
	uint32 i, j;
	bool c = false;
	for (i = 0, j = vecPolygonPoints.size() - 1; i < vecPolygonPoints.size(); j = i++) {
		if (((vecPolygonPoints[i].y>vecPoint.y) != (vecPolygonPoints[j].y>vecPoint.y)) &&
			(vecPoint.x < (vecPolygonPoints[j].x - vecPolygonPoints[i].x) * (vecPoint.y - vecPolygonPoints[i].y) / (vecPolygonPoints[j].y - vecPolygonPoints[i].y) + vecPolygonPoints[i].x))
			c = !c;
	}
	return c;
}

vector<vec2>				Math::getRectangleFromCircle(ivec2& vecCenterPosition, float32 fRadius)
{
	uvec2 pos, size;
	pos.x = (uint32)(vecCenterPosition.x - fRadius);
	pos.y = (uint32)(vecCenterPosition.y - fRadius);
	size.x = (uint32)(vecCenterPosition.x + fRadius);
	size.y = (uint32)(vecCenterPosition.y + fRadius);
	return { pos, size };
}

uvec2				Math::getRectangleSizeFromCircle(ivec2& vecPoint, float32 fRadius)
{
	float32 fDiameter = fRadius * 2;
	return uvec2((uint32)fDiameter, (uint32)fDiameter);
}

uint32				Math::getRowIndex(ivec2& vecPoint, ivec2& vecMinPosition, float32 fRowHeight, uint32 uiRowCount)
{
	float32 fPointYRelative = (float32)(vecPoint.y - vecMinPosition.y);
	int32 iRowIndex = (int32) floor(fPointYRelative / fRowHeight);
	if (iRowIndex >= 0 && iRowIndex < (int32)uiRowCount)
	{
		return iRowIndex;
	}
	else
	{
		return -1;
	}
}

uint32				Math::getColumnIndex(ivec2& vecPoint, ivec2& vecMinPosition, float32 fRowWidth, uint32 uiColumnCount)
{
	float32 fPointXRelative = (float32)(vecPoint.x - vecMinPosition.x);
	int32 iColumnIndex = (int32)floor(fPointXRelative / fRowWidth);
	if (iColumnIndex >= 0 && iColumnIndex < (int32)uiColumnCount)
	{
		return iColumnIndex;
	}
	else
	{
		return -1;
	}
}

uint32				Math::getRowIndexInRectangle(ivec2& vecPoint, ivec2& vecMinPosition, uint32 uiWidth, float32 fRowHeight, uint32 uiRowCount)
{
	if (vecPoint.x < vecMinPosition.x || vecPoint.x > (vecMinPosition.x + (int32)uiWidth))
	{
		return -1;
	}
	return getRowIndex(vecPoint, vecMinPosition, fRowHeight, uiRowCount);
}

uint32				Math::getRectangleResizeEdges(ivec2& vecPoint, uvec2& vecSize, uint32 uiEdgeDistance)
{
	uint32
		uiResult = 0;
	bool
		bIsNearLeftEdge = vecPoint.x < (int32)uiEdgeDistance,
		bIsNearTopEdge = vecPoint.y < (int32)uiEdgeDistance,
		bIsNearRightEdge = vecPoint.x >= (int32)(vecSize.x - uiEdgeDistance),
		bIsNearBottomEdge = vecPoint.y >= (int32)(vecSize.y - uiEdgeDistance);

	if		(bIsNearLeftEdge)	uiResult |= 1;
	else if (bIsNearRightEdge)	uiResult |= 4;
	if		(bIsNearTopEdge)	uiResult |= 2;
	else if (bIsNearBottomEdge)	uiResult |= 8;

	return uiResult;
}

uint32				Math::getRectangleResizeEdges(ivec2& vecPoint, ivec2& vecPosition, uvec2& vecSize, uint32 uiEdgeDistance)
{
	uint32
		uiResult = 0;
	bool
		bIsNearLeftEdge = vecPoint.x < (int32)(vecPosition.x + uiEdgeDistance),
		bIsNearTopEdge = vecPoint.y < (int32)(vecPosition.y + uiEdgeDistance),
		bIsNearRightEdge = vecPoint.x >= (int32)((vecPosition.x + vecSize.x) - uiEdgeDistance),
		bIsNearBottomEdge = vecPoint.y >= (int32)((vecPosition.y + vecSize.y) - uiEdgeDistance);

	if		(bIsNearLeftEdge)	uiResult |= 1;
	else if (bIsNearRightEdge)	uiResult |= 4;
	if		(bIsNearTopEdge)	uiResult |= 2;
	else if (bIsNearBottomEdge)	uiResult |= 8;

	return uiResult;
}

ivec2				Math::getEllipseFromRectangle(ivec2& vecPoint, uvec2& vecSize)
{
	uvec2 vecPoint2 = ivec2(vecSize / 2u) + vecPoint;
	return ivec2(vecPoint2.x, vecPoint2.y);
}

uint32				Math::convertDiameterToRadius(uint32 uiDiameter)
{
	return uiDiameter / 2;
}

float32				Math::getAngleBetweenPoints(ivec2& vecPoint1, ivec2& vecPoint2)
{
	vec2 vecDistance;
	vecDistance.x = (float32)((float32)(vecPoint2.x) - (float32)(vecPoint1.x));
	vecDistance.y = (float32)((float32)(vecPoint2.y) - (float32)(vecPoint1.y));
	return atan2(vecDistance.y, vecDistance.x);
}

// http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToEuler/index.htm
void toEuler(double x, double y, double z, double angle, vec3& vecOut) {
	double s = sin(angle);
	double c = cos(angle);
	double t = 1 - c;
	//  if axis is not already normalised then uncomment this
	double magnitude = sqrt(x*x + y*y + z*z);
	if (magnitude == 0) return;
	x /= magnitude;
	y /= magnitude;
	z /= magnitude;
	if ((x*y*t + z * s) > 0.998) { // north pole singularity detected
		vecOut.z = (float32)(2.0f * atan2(x*sin(angle / 2), cos(angle / 2)));
		vecOut.y = (float32)M_PI / 2.0f;
		vecOut.x = 0.0f;
		return;
	}
	if ((x*y*t + z * s) < -0.998) { // south pole singularity detected
		vecOut.z = (float32)(-2.0f * atan2(x*sin(angle / 2), cos(angle / 2)));
		vecOut.y = -(float32)M_PI / 2.0f;
		vecOut.x = 0.0f;
		return;
	}
	vecOut.z = (float32)atan2(y * s - x * z * t, 1 - (y*y + z * z) * t);
	vecOut.y = (float32)asin(x * y * t + z * s);
	vecOut.x = (float32)atan2(x * s - y * z * t, 1 - (x*x + z * z) * t);
}

float32				Math::getXAngleBetweenPoints(vec3& vecPoint1, vec3& vecPoint2)
{
	//return acos(dot(vec2(vecPoint1.y, vecPoint1.z), vec2(vecPoint2.y, vecPoint2.z)));
	
	//vec2 vecDistance;
	//vecDistance.x = (float32)((float32)(vecPoint2.y) - (float32)(vecPoint1.y));
	//vecDistance.y = (float32)((float32)(vecPoint2.z) - (float32)(vecPoint1.z));
	//return atan2(vecDistance.y, vecDistance.x);

	//return 0;

	//vec3 vecDiff = vecPoint2 - vecPoint1;
	//return atan2(vecDiff.y, vecDiff.z);

	vec3 vecDiff = vecPoint2 - vecPoint1;
	vecDiff = normalize(vecDiff);
	float angle = dot(vec3(1, 0, 0), vecDiff);
	return angle;

	//vec3 vecCross = cross(vecPoint1, vecPoint2);
	//float fDot = dot(vecPoint1, vecPoint2);

	//float fValue = acos(fDot);// (length(vecPoint1) * length(vecPoint2)));
	//return fValue;

	//vec3 vecCross = cross(vecPoint1, vecPoint2);
	/*
	vec3 vecDiff = vecPoint2 - vecPoint1;
	float fDot = dot(vecPoint1, vecPoint2);
	float fAngle = acos(fDot / (length(vecPoint1) * length(vecPoint2)));
	vec3 vecEuler;
	toEuler(vecDiff.x, vecDiff.y, vecDiff.z, fAngle, vecEuler);
	return vecEuler.x;
	*/
}

float32				Math::getYAngleBetweenPoints(vec3& vecPoint1, vec3& vecPoint2)
{
	//return acos(dot(vec2(vecPoint1.x, vecPoint1.z), vec2(vecPoint2.x, vecPoint2.z)));
	
	//vec2 vecDistance;
	//vecDistance.x = (float32)((float32)(vecPoint2.x) - (float32)(vecPoint1.x));
	//vecDistance.y = (float32)((float32)(vecPoint2.z) - (float32)(vecPoint1.z));
	//return atan2(vecDistance.y, vecDistance.x);

	//vec3 vecDiff = vecPoint2 - vecPoint1;
	//return atan2(vecDiff.x * cos();

	//vec3 vecDiff = vecPoint2 - vecPoint1;
	//vecDiff = normalize(vecDiff);
	//float angle = dot(vec3(0, 1, 0), vecDiff);
	//return angle;

	vec3 vecDiff = vecPoint2 - vecPoint1;
	return atan2(vecDiff.z, vecDiff.x);

	/*
	vec3 vecDiff = vecPoint2 - vecPoint1;
	float fRotX = atan2(vecDiff.y, vecDiff.z);
	float fRotY;

	if (vecDiff.z >= 0) {
		fRotY = -atan2(vecDiff.x * cos(fRotX), vecDiff.z);
	}
	else {
		fRotY = atan2(vecDiff.x * cos(fRotX), -vecDiff.z);
	}
	return fRotY;
	*/
	//return atan2(vecDiff.x * cos(rotx), vecDiff.z);
	//return atan2(vecDiff.x, sqrt(vecDiff.y*vecDiff.y + vecDiff.z*vecDiff.z));

	//vec3 vecCross = cross(vecPoint1, vecPoint2);
	/*
	vec3 vecDiff = vecPoint2 - vecPoint1;
	float fDot = dot(vecPoint1, vecPoint2);
	float fAngle = acos(fDot / (length(vecPoint1) * length(vecPoint2)));
	vec3 vecEuler;
	toEuler(vecDiff.x, vecDiff.y, vecDiff.z, fAngle, vecEuler);
	return vecEuler.y;
	*/
}

float32				Math::divide(int32 iInt1, int32 iInt2)
{
	return (float32) (((float32) iInt1) / ((float32) iInt2));
}

float32				Math::divide(float32 fValue1, float32 fValue2)
{
	return fValue1 / fValue2;
}

float32				Math::multiply(int32 iInt1, int32 iInt2)
{
	return (float32) (((float32) iInt1) * ((float32) iInt2));
}

float32				Math::multiply(float32 fValue1, float32 fValue2)
{
	return fValue1 * fValue2;
}

void				Math::reduceByRatioAndClamp(vec3& vecVector, float32 fReduceByRatio, float32 fClamp)
{
	vecVector.x -= vecVector.x * fReduceByRatio; // wind resistance
	vecVector.y -= vecVector.y * fReduceByRatio; // wind resistance
	vecVector.z -= vecVector.z * fReduceByRatio; // gravity

	if (fabs(vecVector.x) < fClamp)
		vecVector.x = 0.0f;
	if (fabs(vecVector.y) < fClamp)
		vecVector.y = 0.0f;
	if (fabs(vecVector.z) < fClamp)
		vecVector.z = 0.0f;
}

float				Math::mid(float f1, float f2)
{
	if (f1 > f2)
	{
		float f3 = f1;
		f1 = f2;
		f2 = f3;
	}
	return f1 + ((f2 - f1) / 2.0f);
}

ivec2				Math::getBoundingRectanglePositionForLine(ivec2& vecPoint1, ivec2& vecPoint2)
{
	return ivec2(
		glm::min(vecPoint1.x, vecPoint2.x),
		glm::min(vecPoint1.y, vecPoint2.y)
	);
}

uvec2				Math::getBoundingRectangleSizeForLine(ivec2& vecPoint1, ivec2& vecPoint2)
{
	ivec2 vecMinPoint(
		glm::min(vecPoint1.x, vecPoint2.x),
		glm::min(vecPoint1.y, vecPoint2.y)
	);
	ivec2 vecMaxPoint(
		glm::max(vecPoint1.x, vecPoint2.x),
		glm::max(vecPoint1.y, vecPoint2.y)
	);
	return uvec2(
		(vecMaxPoint.x - vecMinPoint.x) + 1,
		(vecMaxPoint.y - vecMinPoint.y) + 1
	);
}

ivec2				Math::getBoundingRectanglePositionForPolygon(vector<ivec2>& vecPoints)
{
	ivec2 vecMinPoint = ivec2(vecPoints[0]);
	for (ivec2& vecPoint : vecPoints)
	{
		if (vecPoint.x < vecMinPoint.x)
		{
			vecMinPoint.x = vecPoint.x;
		}
		if (vecPoint.y < vecMinPoint.y)
		{
			vecMinPoint.y = vecPoint.y;
		}
	}
	return vecMinPoint;
}

uvec2				Math::getBoundingRectangleSizeForPolygon(vector<ivec2>& vecPoints)
{
	ivec2 vecMinPoint = ivec2(vecPoints[0]);
	ivec2 vecMaxPoint = ivec2(vecPoints[0]);
	for (ivec2& vecPoint : vecPoints)
	{
		if (vecPoint.x < vecMinPoint.x)
		{
			vecMinPoint.x = vecPoint.x;
		}
		else if (vecPoint.x > vecMaxPoint.x)
		{
			vecMaxPoint.x = vecPoint.x;
		}

		if (vecPoint.y < vecMinPoint.y)
		{
			vecMinPoint.y = vecPoint.y;
		}
		else if (vecPoint.y > vecMaxPoint.y)
		{
			vecMaxPoint.y = vecPoint.y;
		}
	}
	return uvec2(vecMaxPoint.x - vecMinPoint.x, vecMaxPoint.y - vecMinPoint.y);
}

vector<vec3>	Math::getBoundingCuboidPointsForPolygon(vector<vec3>& vecPoints)
{
	if (vecPoints.size() == 0)
	{
		vector<vec3> vecResult;
		vecResult.push_back(vec3(0.0f, 0.0f, 0.0f));
		vecResult.push_back(vec3(0.0f, 0.0f, 0.0f));
		//vecResult.push_back(vec3(99999.99f, 99999.99f, 99999.99f));
		//vecResult.push_back(vec3(-99999.99f, -99999.99f, -99999.99f));
		return vecResult;
	}

	vec3 vecMinPoint = vec3(vecPoints[0]);
	vec3 vecMaxPoint = vec3(vecPoints[0]);
	for (vec3& vecPoint : vecPoints)
	{
		if (vecPoint.x < vecMinPoint.x)
		{
			vecMinPoint.x = vecPoint.x;
		}
		else if (vecPoint.x > vecMaxPoint.x)
		{
			vecMaxPoint.x = vecPoint.x;
		}

		if (vecPoint.y < vecMinPoint.y)
		{
			vecMinPoint.y = vecPoint.y;
		}
		else if (vecPoint.y > vecMaxPoint.y)
		{
			vecMaxPoint.y = vecPoint.y;
		}

		if (vecPoint.z < vecMinPoint.z)
		{
			vecMinPoint.z = vecPoint.z;
		}
		else if (vecPoint.z > vecMaxPoint.z)
		{
			vecMaxPoint.z = vecPoint.z;
		}
	}

	vector<vec3> vecResult;
	vecResult.push_back(vecMinPoint);
	vecResult.push_back(vecMaxPoint);
	return vecResult;
}

vector<vec3>	Math::getBoundingCuboidPointsForPolygon(vector<vec3>& vecMeshPoints, vector<GLuint>& vecFaceGroupIndices)
{
	if (vecMeshPoints.size() == 0 || vecFaceGroupIndices.size() == 0)
	{
		vector<vec3> vecResult;
		vecResult.push_back(vec3(0.0f, 0.0f, 0.0f));
		vecResult.push_back(vec3(0.0f, 0.0f, 0.0f));
		return vecResult;
	}

	vec3 vecMinPoint = vec3(vecMeshPoints[vecFaceGroupIndices[0]]);
	vec3 vecMaxPoint = vec3(vecMeshPoints[vecFaceGroupIndices[0]]);
	for (GLuint uiIndex : vecFaceGroupIndices)
	{
		vec3& vecPoint = vecMeshPoints[uiIndex];

		if (vecPoint.x < vecMinPoint.x)
		{
			vecMinPoint.x = vecPoint.x;
		}
		else if (vecPoint.x > vecMaxPoint.x)
		{
			vecMaxPoint.x = vecPoint.x;
		}

		if (vecPoint.y < vecMinPoint.y)
		{
			vecMinPoint.y = vecPoint.y;
		}
		else if (vecPoint.y > vecMaxPoint.y)
		{
			vecMaxPoint.y = vecPoint.y;
		}

		if (vecPoint.z < vecMinPoint.z)
		{
			vecMinPoint.z = vecPoint.z;
		}
		else if (vecPoint.z > vecMaxPoint.z)
		{
			vecMaxPoint.z = vecPoint.z;
		}
	}

	vector<vec3> vecResult;
	vecResult.push_back(vecMinPoint);
	vecResult.push_back(vecMaxPoint);
	return vecResult;
}

void						Math::getResizePositionAndSizeChange(ivec2& vecCursorChange, uint32 uiResizeEdges, ivec2& vecItemPositionChange, ivec2& vecItemSizeChange)
{
	vecItemPositionChange.x = 0;
	vecItemPositionChange.y = 0;
	vecItemSizeChange.x = 0;
	vecItemSizeChange.y = 0;

	if (uiResizeEdges & 1) // left edge
	{
		vecItemPositionChange.x += vecCursorChange.x;
	}
	else if (uiResizeEdges & 4) // right edge
	{
		vecItemSizeChange.x += vecCursorChange.x;
	}

	if (uiResizeEdges & 2) // top edge
	{
		vecItemPositionChange.y += vecCursorChange.y;
	}
	else if (uiResizeEdges & 8) // bottom edge
	{
		vecItemSizeChange.y += vecCursorChange.y;
	}
}

vector<ivec2>	Math::getEquilateralTrianglePoints(ivec2& vecPoint, float32 fSideLength, uint32 uiPointingDirection)
{
	int32
		iTriangleHeight = (int32) getEquilateralTriangleHeightFromSideLength(fSideLength),
		iHalfSideLength = (int32) (fSideLength / 2);
	vector<ivec2>
		vecPoints;
	vecPoints.resize(3);
	switch (uiPointingDirection)
	{
	case 1: // left arrow
		vecPoints[0] = vecPoint; // top right point
		vecPoints[1] = vecPoint + ivec2(0, (uint32)fSideLength);
		vecPoints[2] = vecPoint + ivec2(-iTriangleHeight, iHalfSideLength);
		break;
	case 2: // up arrow
		vecPoints[0] = vecPoint; // bottom left point
		vecPoints[1] = vecPoint + ivec2((uint32)fSideLength, 0);
		vecPoints[2] = vecPoint + ivec2(iHalfSideLength, -iTriangleHeight);
		break;
	case 3: // right arrow
		vecPoints[0] = vecPoint; // top left point
		vecPoints[1] = vecPoint + ivec2(0, (uint32)fSideLength);
		vecPoints[2] = vecPoint + ivec2(iTriangleHeight, iHalfSideLength);
		break;
	case 4: // down arrow
		vecPoints[0] = vecPoint; // top left point
		vecPoints[1] = vecPoint + ivec2((uint32)fSideLength, 0);
		vecPoints[2] = vecPoint + ivec2(iHalfSideLength, iTriangleHeight);
		break;
	case 5: // top left arrow
		vecPoints[0] = vecPoint + ivec2(0, iTriangleHeight); // bottom left point
		vecPoints[1] = vecPoint + ivec2((uint32)fSideLength, 0); // top right point
		vecPoints[2] = vecPoint; // top left point
		break;
	case 6: // top right arrow
		vecPoints[0] = vecPoint + ivec2(0, iTriangleHeight); // top left point
		vecPoints[1] = vecPoint + ivec2((uint32)fSideLength, iTriangleHeight); // bottom right point
		vecPoints[2] = vecPoint + ivec2((uint32)fSideLength, 0); // top right point
		break;
	case 7: // bottom right arrow
		vecPoints[0] = vecPoint + ivec2(0, iTriangleHeight); // bottom left point
		vecPoints[1] = vecPoint + ivec2((uint32)fSideLength, 0); // top right point
		vecPoints[2] = vecPoint + ivec2((uint32)fSideLength, iTriangleHeight); // bottom right point
		break;
	case 8: // bottom left arrow
		vecPoints[0] = vecPoint; // top left point
		vecPoints[1] = vecPoint + ivec2((uint32)fSideLength, iTriangleHeight); // bottom right point
		vecPoints[2] = vecPoint + ivec2(0, iTriangleHeight); // bottom left point
		break;
	}
	return vecPoints;
}

vector<ivec2>	Math::getEquilateralTrianglePoints(ivec2& vecBottomLeftPoint, ivec2& vecBottomRightPoint)
{
	const float32
		fSideLength = Math::getDistanceBetweenPoints(vecBottomLeftPoint, vecBottomRightPoint),
		fBaseAngle = Math::getAngleBetweenPoints(vecBottomLeftPoint, vecBottomRightPoint),
		fAngle = Math::convertDegreesToRadians(60.0f);
	ivec2
		vecTipPoint = Math::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle - fAngle, fSideLength);
	vector<ivec2>
		vecShapePoints;
	vecShapePoints.resize(3);
	vecShapePoints[0] = vecBottomLeftPoint;
	vecShapePoints[1] = vecBottomRightPoint;
	vecShapePoints[2] = vecTipPoint;
	return vecShapePoints;
}

vector<ivec2>	Math::getEquilateralTrianglePoints(ivec2& vecBottomLeftPoint, float32 fSideLength, float32 fBaseAngle)
{
	const float32
		fAngle = Math::convertDegreesToRadians(60.0f);
	fBaseAngle = Math::convertDegreesToRadians(fBaseAngle);
	ivec2
		vecBottomRightPoint = Math::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle, fSideLength),
		vecTopPoint = Math::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle - fAngle, fSideLength);
	vector<ivec2>
		vecShapePoints;
	vecShapePoints.resize(3);
	vecShapePoints[0] = vecBottomLeftPoint;
	vecShapePoints[1] = vecBottomRightPoint;
	vecShapePoints[2] = vecTopPoint;
	return vecShapePoints;
}

vector<ivec2>	Math::getIsoscelesTrianglePoints(ivec2& vecPoint, float32 fBaseLength, float32 f2SidesLength, uint32 uiPointingDirection)
{
	int32
		iTriangleHeight = (int32) getIsoscelesTriangleHeightFrom2SidesLength(f2SidesLength, cos(f2SidesLength / fBaseLength)),
		iHalfSideLength = (int32) (f2SidesLength / 2);
	vector<ivec2>
		vecPoints;
	vecPoints.resize(3);
	switch (uiPointingDirection)
	{
	case 1: // left arrow
		vecPoints[0] = vecPoint; // top right point
		vecPoints[1] = vecPoint + ivec2(0, (uint32)fBaseLength);
		vecPoints[2] = vecPoint + ivec2(-iTriangleHeight, iHalfSideLength);
		break;
	case 2: // up arrow
		vecPoints[0] = vecPoint; // bottom left point
		vecPoints[1] = vecPoint + ivec2((uint32)fBaseLength, 0);
		vecPoints[2] = vecPoint + ivec2(iHalfSideLength, -iTriangleHeight);
		break;
	case 3: // right arrow
		vecPoints[0] = vecPoint; // top left point
		vecPoints[1] = vecPoint + ivec2(0, (uint32)fBaseLength);
		vecPoints[2] = vecPoint + ivec2(iTriangleHeight, iHalfSideLength);
		break;
	case 4: // down arrow
		vecPoints[0] = vecPoint; // top left point
		vecPoints[1] = vecPoint + ivec2((uint32)fBaseLength, 0);
		vecPoints[2] = vecPoint + ivec2(iHalfSideLength, iTriangleHeight);
		break;
	case 5: // top left arrow
		vecPoints[0] = vecPoint + ivec2(0, iTriangleHeight); // bottom left point
		vecPoints[1] = vecPoint + ivec2((uint32)fBaseLength, 0); // top right point
		vecPoints[2] = vecPoint; // top left point
		break;
	case 6: // top right arrow
		vecPoints[0] = vecPoint + ivec2(0, iTriangleHeight); // top left point
		vecPoints[1] = vecPoint + ivec2((uint32)fBaseLength, iTriangleHeight); // bottom right point
		vecPoints[2] = vecPoint + ivec2((uint32)fBaseLength, 0); // top right point
		break;
	case 7: // bottom right arrow
		vecPoints[0] = vecPoint + ivec2(0, iTriangleHeight); // bottom left point
		vecPoints[1] = vecPoint + ivec2((uint32)fBaseLength, 0); // top right point
		vecPoints[2] = vecPoint + ivec2((uint32)fBaseLength, iTriangleHeight); // bottom right point
		break;
	case 8: // bottom left arrow
		vecPoints[0] = vecPoint; // top left point
		vecPoints[1] = vecPoint + ivec2((uint32)fBaseLength, iTriangleHeight); // bottom right point
		vecPoints[2] = vecPoint + ivec2(0, iTriangleHeight); // bottom left point
		break;
	}
	return vecPoints;
}

vector<ivec2>	Math::getIsoscelesTrianglePoints(ivec2& vecBaseCenterPoint, ivec2& vecTipPoint, uint32 uiBaseHalfWidth)
{
	const float32
		fBaseToTipDistance = Math::getDistanceBetweenPoints(vecBaseCenterPoint, vecTipPoint),
		fBaseToTipAngle = Math::getAngleBetweenPoints(vecBaseCenterPoint, vecTipPoint),
		f90DegreeAngle = Math::convertDegreesToRadians(90.0f);
	ivec2
		vecBasePoint1 = Math::getPositionInFrontOfPosition(vecBaseCenterPoint, fBaseToTipAngle - f90DegreeAngle, (float32)uiBaseHalfWidth),
		vecBasePoint2 = Math::getPositionInFrontOfPosition(vecBaseCenterPoint, fBaseToTipAngle + f90DegreeAngle, (float32)uiBaseHalfWidth);
	vector<ivec2>
		vecShapePoints;
	vecShapePoints.resize(3);
	vecShapePoints[0] = vecBasePoint1;
	vecShapePoints[1] = vecBasePoint2;
	vecShapePoints[2] = vecTipPoint;
	return vecShapePoints;
}

vector<ivec2>	Math::getIsoscelesTrianglePoints(ivec2& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle, float32 fBaseAngle)
{
	float32
		fLinesAngle = (180.0f - fTipAngle) / 2.0f,
		fHeight = (fBaseLength / 2.0f) * tan(fLinesAngle),
		fSideLength = sqrt(((fBaseLength * fBaseLength) / 4.0f) + (fHeight * fHeight));
	fTipAngle = Math::convertDegreesToRadians(fTipAngle);
	fBaseAngle = Math::convertDegreesToRadians(fBaseAngle);
	fLinesAngle = Math::convertDegreesToRadians(fLinesAngle);
	ivec2
		vecBottomRightPoint = Math::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle, fBaseLength),
		vecTipPoint = Math::getPositionInFrontOfPosition(vecBottomLeftPoint, fBaseAngle - fLinesAngle, fSideLength);
	vector<ivec2>
		vecShapePoints;
	vecShapePoints.resize(3);
	vecShapePoints[0] = vecBottomLeftPoint;
	vecShapePoints[1] = vecBottomRightPoint;
	vecShapePoints[2] = vecTipPoint;
	return vecShapePoints;
}

float32				Math::getEquilateralTriangleHeightFromSideLength(float32 fTriangleSideLength)
{
	return (fTriangleSideLength / 2.0f) * sqrt(3.0f);
}

float32				Math::getEquilateralTriangleSideLengthFromHeight(float32 fTriangleHeight)
{
	return (fTriangleHeight * 2.0f) / sqrt(3.0f);
}

float32				Math::getIsoscelesTriangleHeightFrom2SidesLength(float32 fTriangle2SidesLength, float32 fBaseToTipAngle)
{
	return fTriangle2SidesLength * sin(fBaseToTipAngle);
}

uint32				Math::getMaxEntryCount(int32 iAreaLengthPx, uint32 uiStepLengthPx)
{
	if (iAreaLengthPx < 0)
	{
		return 0;
	}
	return (uint32) floor(((float32)iAreaLengthPx) / (float32)uiStepLengthPx);
}

uint32				Math::getEntryStartIndex(uint32 uiTotalEntryCount, uint32 uiMaxEntryCount, float32 fProgress)
{
	if (uiTotalEntryCount < uiMaxEntryCount)
	{
		return 0;
	}
	else
	{
		return (uint32)floor(fProgress * (float32)((uiTotalEntryCount + 1) - uiMaxEntryCount));
	}
}

uint32				Math::getEntryEndIndexExclusive(uint32 uiTotalEntryCount, uint32 uiEntryStartIndex, uint32 uiMaxEntryCount)
{
	uint32 uiEntryEndIndexExclusive = uiEntryStartIndex + uiMaxEntryCount;
	if (uiEntryEndIndexExclusive >= (uiTotalEntryCount + 1))
	{
		uiEntryEndIndexExclusive -= uiEntryEndIndexExclusive - uiTotalEntryCount;
	}
	return uiEntryEndIndexExclusive;
}

void				Math::increaseAndLimit(vec3& vecVector, vec3& vecIncreaseBy, vec3& vecMaxVector)
{
	vecVector.x += (vecVector.x + vecIncreaseBy.x) <= vecMaxVector.x ? vecIncreaseBy.x : (vecMaxVector.x - vecVector.x);
	vecVector.y += (vecVector.y + vecIncreaseBy.y) <= vecMaxVector.y ? vecIncreaseBy.y : (vecMaxVector.y - vecVector.y);
	vecVector.z += (vecVector.z + vecIncreaseBy.z) <= vecMaxVector.z ? vecIncreaseBy.z : (vecMaxVector.z - vecVector.z);
}

void				Math::increaseAndLimit(vec3& vecVector, vec3& vecIncreaseBy, vec3& vecMinVector, vec3& vecMaxVector)
{
	vecVector.x += (vecVector.x + vecIncreaseBy.x) <= vecMaxVector.x ? ((vecVector.x + vecIncreaseBy.x) >= vecMinVector.x ? vecIncreaseBy.x : (vecMinVector.x - vecVector.x)) : (vecMaxVector.x - vecVector.x);
	vecVector.y += (vecVector.y + vecIncreaseBy.y) <= vecMaxVector.y ? ((vecVector.y + vecIncreaseBy.y) >= vecMinVector.y ? vecIncreaseBy.y : (vecMinVector.y - vecVector.y)) : (vecMaxVector.y - vecVector.y);
	vecVector.z += (vecVector.z + vecIncreaseBy.z) <= vecMaxVector.z ? ((vecVector.z + vecIncreaseBy.z) >= vecMinVector.z ? vecIncreaseBy.z : (vecMinVector.z - vecVector.z)) : (vecMaxVector.z - vecVector.z);
}

void				Math::increaseAndLimit(float& fValue, float fIncreaseBy, float fMin, float fMax)
{
	fValue += fIncreaseBy;
	fValue = Math::limit(fValue, fMin, fMax);
}

bool				Math::doRectanglesCollide(ivec2& vecRectangle1Position, uvec2& vecRectangle1Size, ivec2& vecRectangle2Position, uvec2& vecRectangle2Size)
{
	// https://stackoverflow.com/questions/306316/determine-if-two-rectangles-overlap-each-other
	return vecRectangle1Position.x < (vecRectangle2Position.x + (int32)vecRectangle2Size.x)
		&& (vecRectangle1Position.x + (int32)vecRectangle1Size.x) > vecRectangle2Position.x
		&& vecRectangle1Position.y > (vecRectangle2Position.y + (int32)vecRectangle2Size.y)
		&& (vecRectangle1Position.y + (int32)vecRectangle1Size.y) < vecRectangle2Position.y;
}

vec4			Math::toQuaternion(double pitch, double roll, double yaw)
{
	vec4 q;

	// Abbreviations for the various angular functions
	double cy = cos(yaw * 0.5);
	double sy = sin(yaw * 0.5);
	double cr = cos(roll * 0.5);
	double sr = sin(roll * 0.5);
	double cp = cos(pitch * 0.5);
	double sp = sin(pitch * 0.5);

	q.w = (float32)(cy * cr * cp + sy * sr * sp);
	q.x = (float32)(cy * sr * cp - sy * cr * sp);
	q.y = (float32)(cy * cr * sp + sy * sr * cp);
	q.z = (float32)(sy * cr * cp - cy * sr * sp);
	return q;
}

vec3			Math::toEulerAngle(const vec4& q)
{
	vec3 a;

	// roll (x-axis rotation)
	double sinr = +2.0 * (q.w * q.x + q.y * q.z);
	double cosr = +1.0 - 2.0 * (q.x * q.x + q.y * q.y);
	a.x = (float32)atan2(sinr, cosr);

	// pitch (y-axis rotation)
	double sinp = +2.0 * (q.w * q.y - q.z * q.x);
	if (fabs(sinp) >= 1)
		a.y = (float32)copysign(M_PI / 2, sinp); // use 90 degrees if out of range
	else
		a.y = (float32)asin(sinp);

	// yaw (z-axis rotation)
	double siny = +2.0 * (q.w * q.z + q.x * q.y);
	double cosy = +1.0 - 2.0 * (q.y * q.y + q.z * q.z);
	a.z = (float32)atan2(siny, cosy);

	return a;
}

void				Math::toPlucker(vec3& point1, vec3& point2, vec3& pluckOut1, vec3& pluckOut2)
{
	pluckOut1 = point1 - point2;
	pluckOut2 = {
		point2.y*point1.z - point1.y*point2.z,
		point2.z*point1.x - point1.z*point2.x,
		point2.x*point1.y - point1.x*point2.y
	};
}

// https://stackoverflow.com/questions/7748357/converting-3d-position-to-2d-screen-position
vec2			Math::convert3Dto2D(vec3& pos, const mat4& viewMatrix, const mat4& projectionMatrix, uvec2& vecScreenSize)
{
	vec4 pos2(pos, 1.0);
	pos2 = viewMatrix * pos2;
	pos2 = projectionMatrix * pos2;

	pos2.x /= pos2.z;
	pos2.y /= pos2.z;
	pos2.x = (pos2.x + 1.0f) * (float32)(vecScreenSize.x / 2);
	pos2.y = (pos2.y + 1.0f) * (float32)(vecScreenSize.y / 2);

	return vec2(pos2.x, pos2.y);
}

bool				Math::doesRayIntersectAABB2D(vec3& ray_start, vec3& ray_end, vec3& cuboid_min, vec3& cuboid_max, float& t)
{
	vec3 ray_direction(0, 0, 0);
	ray_direction.z = Math::getAngleBetweenPoints(ray_start, ray_end);

	vec3 dirfrac;
	// r.dir is unit direction vector of ray
	dirfrac.x = 1.0f / ray_direction.x;
	dirfrac.y = 1.0f / ray_direction.y;
	//dirfrac.z = 1.0f / ray_direction.z;
	// lb is the corner of AABB with minimal coordinates - left bottom, rt is maximal corner
	// r.org is origin of ray
	float t1 = (cuboid_min.x - ray_start.x)*dirfrac.x;
	float t2 = (cuboid_max.x - ray_start.x)*dirfrac.x;
	float t3 = (cuboid_min.y - ray_start.y)*dirfrac.y;
	float t4 = (cuboid_max.y - ray_start.y)*dirfrac.y;
	//float t5 = (cuboid_min.z - ray_start.z)*dirfrac.z;
	//float t6 = (cuboid_max.z - ray_start.z)*dirfrac.z;

	float tmin = glm::max(glm::min(t1, t2), glm::min(t3, t4));
	float tmax = glm::min(glm::max(t1, t2), glm::max(t3, t4));

	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0.0f)
	{
		t = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		t = tmax;
		return false;
	}

	t = tmin;
	return true;
}

bool				Math::isAngleBetween(float f, float min, float max) // All 3 parameters are in degrees.
{
	f = fmod(fmod(f, 360.0f) + 360.0f, 360.0f);

	// make the angle from angle1 to angle2 to be <= 180 degrees
	float rAngle = fmod(fmod(max - min, 360.0f) + 360.0f, 360.0f);
	if (rAngle >= 180.0f)
		swap(min, max);

	// check if it passes through zero
	if (min <= max)
		return f >= min && f <= max;
	else
		return f >= min || f <= max;
}

bool				Math::isAngleNear(float fAngleRad, float fAngleToBeNearRad, float fRotEachSideRad)
{
	return isAngleBetween(degrees(fAngleRad), degrees(fAngleToBeNearRad - fRotEachSideRad), degrees(fAngleToBeNearRad + fRotEachSideRad));
}

vec3				Math::swapYZ(vec3& vecIn)
{
	return vec3(vecIn.x, vecIn.z, vecIn.y);
}

// http://www.txutxi.com/?p=584
/*
**
* Tells whether or not b is intersecting f.
* @param f Viewing frustum.
* @param b An axis aligned bounding box.
* @return True if b intersects f, false otherwise.
*
*/
bool				Math::doesFrustumIntersectAABB(const Frustum &f, const vec3& vecCuboidMin, const vec3& vecCuboidMax)
{
	// Indexed for the 'index trick' later
	vec3 box[] = { vecCuboidMin, vecCuboidMax };

	// We have 6 planes defining the frustum
	static const int NUM_PLANES = 6;
	const Plane *planes[NUM_PLANES] =
	{ &f._near, &f.left, &f.right, &f.bottom, &f.top, &f._far };

	// We only need to do 6 point-plane tests
	for (int i = 0; i < NUM_PLANES; ++i)
	{
		// This is the current plane
		const Plane &p = *planes[i];

		// p-vertex selection (with the index trick)
		// According to the plane normal we can know the
		// indices of the positive vertex
		const int px = static_cast<int>(p.a > 0.0f);
		const int py = static_cast<int>(p.b > 0.0f);
		const int pz = static_cast<int>(p.c > 0.0f);

		// Dot product
		// project p-vertex on plane normal
		// (How far is p-vertex from the origin)
		const float dp =
			(p.a*box[px].x) +
			(p.b*box[py].y) +
			(p.c*box[pz].z);

		// Doesn't intersect if it is behind the plane
		if (dp < -p.distance) { return false; }
	}
	return true;
}

Frustum					Math::frustum(mat4& matModel, float fMaxViewDistance)
{
	mat4& mvp = matModel;
	Plane _near, _far, top, bottom, left, right;
	const float mat[16] =
	{
		mvp[0][0], mvp[0][1], mvp[0][2], mvp[0][3],
		mvp[1][0], mvp[1][1], mvp[1][2], mvp[1][3],
		mvp[2][0], mvp[2][1], mvp[2][2], mvp[2][3],
		mvp[3][0], mvp[3][1], mvp[3][2], mvp[3][3]
	};

	// Left Plane
	// col4 + col1
	left.a = mat[3] + mat[0];
	left.b = mat[7] + mat[4];
	left.c = mat[11] + mat[8];
	left.d = mat[15] + mat[12];
	left.distance = fMaxViewDistance;

	// Right Plane
	// col4 - col1
	right.a = mat[3] - mat[0];
	right.b = mat[7] - mat[4];
	right.c = mat[11] - mat[8];
	right.d = mat[15] - mat[12];
	right.distance = fMaxViewDistance;

	// Bottom Plane
	// col4 + col2
	bottom.a = mat[3] + mat[1];
	bottom.b = mat[7] + mat[5];
	bottom.c = mat[11] + mat[9];
	bottom.d = mat[15] + mat[13];
	bottom.distance = fMaxViewDistance;

	// Top Plane
	// col4 - col2
	top.a = mat[3] - mat[1];
	top.b = mat[7] - mat[5];
	top.c = mat[11] - mat[9];
	top.d = mat[15] - mat[13];
	top.distance = fMaxViewDistance;

	// Near Plane
	// col4 + col3
	_near.a = mat[3] + mat[2];
	_near.b = mat[7] + mat[6];
	_near.c = mat[11] + mat[10];
	_near.d = mat[15] + mat[14];
	_near.distance = fMaxViewDistance;

	// Far Plane
	// col4 - col3
	_far.a = mat[3] - mat[2];
	_far.b = mat[7] - mat[6];
	_far.c = mat[11] - mat[10];
	_far.d = mat[15] - mat[14];
	_far.distance = fMaxViewDistance;

	Frustum frustum;
	frustum._near = _near;
	frustum._far = _far;
	frustum.top = top;
	frustum.bottom = bottom;
	frustum.left = left;
	frustum.right = right;
	return frustum;
}

float						Math::getAngleDiff(float fAngle1Rad, float fAngle2Rad)
{
	fAngle1Rad = degrees(constrainAngle(fAngle1Rad));
	fAngle2Rad = degrees(constrainAngle(fAngle2Rad));
	return radians(180.0f - fabs(fabs(fAngle1Rad - fAngle2Rad) - 180.0f));
}

float						Math::getAngleDiffDeg(float fAngle1Deg, float fAngle2Deg)
{
	return degrees(getAngleDiff(fAngle1Deg, fAngle2Deg));
}

float						Math::getAngleDiffWithSign(float fAngle1Rad, float fAngle2Rad)
{
	float fAngle1Deg = degrees(fAngle1Rad);
	float fAngle2Deg = degrees(fAngle2Rad);
	float fAngleDeg = fAngle2Deg - fAngle1Deg;
	if (fAngleDeg > 180.0f)
		fAngleDeg -= 180.0f;
	if (fAngleDeg < -180.0f)
		fAngleDeg += 180.0f;
	return radians(fAngleDeg);
}

bool						Math::isFaceClockwise(vec3& vecTrianglePoint1Position, vec3& vecTrianglePoint2Position, vec3& vecTrianglePoint3Position)
{
	float fTotalSideLength = 0;
	fTotalSideLength += (vecTrianglePoint2Position.x - vecTrianglePoint1Position.x) * (vecTrianglePoint2Position.y + vecTrianglePoint1Position.y);
	fTotalSideLength += (vecTrianglePoint3Position.x - vecTrianglePoint2Position.x) * (vecTrianglePoint3Position.y + vecTrianglePoint2Position.y);
	fTotalSideLength += (vecTrianglePoint1Position.x - vecTrianglePoint3Position.x) * (vecTrianglePoint1Position.y + vecTrianglePoint3Position.y);
	return fTotalSideLength > 0.0f;
}

vec3					Math::getNormal(vec3& x0, vec3& x1, vec3& x2)
{
	return normalize(cross(x0 - x2, x1 - x2));
}

float						Math::rand(float fMin, float fMax)
{
	return 0.0f;
}

vec3					Math::randRGBf(void)
{
	//srand((unsigned int)GetTickCount());
	return vec3(
		::rand() % 256,
		::rand() % 256,
		::rand() % 256
	) / 255.0f;
}

mat4					Math::getRotationMatrixZYX(vec3& vecRotationRad)
{
	mat4 mat1(1.0f);
	mat4 matRotationX = rotate(mat1, vecRotationRad.x, vec3(1.0f, 0.0f, 0.0f));
	mat4 matRotationY = rotate(mat1, vecRotationRad.y, vec3(0.0f, 1.0f, 0.0f));
	mat4 matRotationZ = rotate(mat1, vecRotationRad.z, vec3(0.0f, 0.0f, 1.0f));
	return matRotationX * matRotationY * matRotationZ;
}

bool					Math::isInteger(double dNumber)
{
	char szBuffer[50];
	sprintf_s(szBuffer, "%g", dNumber);
	return strstr(szBuffer, ".") == nullptr;
}

unsigned long			Math::reverseUlong(unsigned long uiNumber)
{
	return (uiNumber % 256) * 16777216,
		+((unsigned long)floor(uiNumber / 256)) * 65536,
		+((unsigned long)floor(uiNumber / 65536)) * 256,
		+((unsigned long)floor(uiNumber / 16777216));
}

unsigned long			Math::getRandomColour(void)
{
	return (::rand() % 256) | ((::rand() % 256) << 8) | ((::rand() % 256) << 16) | (255 << 24);
}

vec3					Math::interpolate(vec3& vecStart, vec3& vecEnd, float fProgress)
{
	return vecStart + ((vecEnd - vecStart) * fProgress);
}

vec3					Math::sphericalInterpolate(vec3& vecStart, vec3& vecEnd, float fProgress)
{
	return vecStart + ((vecEnd - vecStart) * fProgress);
}

/*
quat					Math::sphericalInterpolate(quat v0, quat v1, double t)
{
	// Only unit quaternions are valid rotations.
	// Normalize to avoid undefined behavior.
	v0 = normalize(v0);
	v1 = normalize(v1);

	// Compute the cosine of the angle between the two vectors.
	double dot = glm::dot(v0, v1);

	// If the dot product is negative, slerp won't take
	// the shorter path. Note that v1 and -v1 are equivalent when
	// the negation is applied to all four components. Fix by 
	// reversing one quaternion.
	if (dot < 0.0f) {
		v1 = -v1;
		dot = -dot;
	}

	const double DOT_THRESHOLD = 0.9995;
	if (dot > DOT_THRESHOLD) {
		// If the inputs are too close for comfort, linearly interpolate
		// and normalize the result.

		quat result = v0 + t * (v1 - v0);
		result = normalize(result);
		return result;
	}

	// Since dot is in range [0, DOT_THRESHOLD], acos is safe
	double theta_0 = acos(dot);        // theta_0 = angle between input vectors
	double theta = theta_0 * t;          // theta = angle between v0 and result
	double sin_theta = sin(theta);     // compute this value only once
	double sin_theta_0 = sin(theta_0); // compute this value only once

	double s0 = cos(theta) - dot * sin_theta / sin_theta_0;  // == sin(theta_0 - theta) / sin(theta_0)
	double s1 = sin_theta / sin_theta_0;

	return (s0 * v0) + (s1 * v1);
}
*/

uint32					Math::getFirstRangeInRanges(map<uint32, uint32>& map, uint32 uiRangeSize, uint32 uiMaxRangesSize)
{
	uint32 uiSeek = 0;

	for (auto it : map)
	{
		uint32 uiOffset = it.first;
		uint32 uiSize = it.second;

		if (uiRangeSize <= (uiOffset - uiSeek))
		{
			return (uiOffset + uiRangeSize) <= uiMaxRangesSize ? (uiOffset + uiRangeSize) : -1;
		}

		uiSeek = uiOffset + uiSize;
	}

	return (uiSeek + uiRangeSize) <= uiMaxRangesSize ? uiSeek : -1;
}