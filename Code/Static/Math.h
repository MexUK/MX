#pragma once

#include <vector>
#include <map>

#include "mx.h"
#include "Types.h"
#include "Components/Plane.h"
#include "Components/Frustum.h"

class mx::Math
{
public:
	// general
	static int32				limit(int32 iValue, int32 iMin, int32 iMax);
	static uint32				limit(uint32 uiValue, uint32 uiMin, uint32 uiMax);
	static float32				limit(float32 fValue, float32 fMin, float32 fMax);

	static float32				divide(int32 iInt1, int32 iInt2);
	static float32				divide(float32 fValue1, float32 fValue2);	// intended for sending non float values as parameters
	static float32				multiply(int32 iInt1, int32 iInt2);
	static float32				multiply(float32 fValue1, float32 fValue2);	// intended for sending non float values as parameters

	static void					reduceByRatioAndClamp(glm::vec3& vecVector, float32 fReduceByRatio, float32 fClamp);

	static float				mid(float f1, float f2);

	// iteration
	static uint32				getMaxEntryCount(int32 iAreaLengthPx, uint32 uiStepLengthPx);
	static uint32				getEntryStartIndex(uint32 uiTotalEntryCount, uint32 uiMaxEntryCount, float32 fProgress);
	static uint32				getEntryEndIndexExclusive(uint32 uiTotalEntryCount, uint32 uiEntryStartIndex, uint32 uiMaxEntryCount);

	static uint32				getRowIndex(glm::ivec2& vecPoint, glm::ivec2& vecMinPosition, float32 fRowHeight, uint32 uiRowCount);
	static uint32				getColumnIndex(glm::ivec2& vecPoint, glm::ivec2& vecMinPosition, float32 fRowWidth, uint32 uiColumnCount);
	static uint32				getRowIndexInRectangle(glm::ivec2& vecPoint, glm::ivec2& vecMinPosition, uint32 uiWidth, float32 fRowHeight, uint32 uiRowCount);

	// angles
	static float32				convertDegreesToRadians(float32 fDegrees);
	static float32				convertRadiansToDegrees(float32 fRadians);

	static bool					isAngleBetween(float f, float min, float max); // All 3 parameters are in degrees.
	static bool					isAngleNear(float fAngleRad, float fAngleToBeNearRad, float fRotEachSideRad);
	static float32				limitAngle(float32 fAngle, bool bIsDeg = true);
	static float32				constrainAngle(float32 fAngle); // The input parameter is in radians.

	// lines
	static float32				getDistanceBetweenPoints(glm::vec2& vecPosition1, glm::vec2& vecPosition2);
	static float32				getDistanceBetweenPoints(glm::vec3& vecPosition1, glm::vec3& vecPosition2);
	static float32				getDistanceBetweenPoints(glm::ivec2& vecPoint1, glm::ivec2& vecPoint2);
	static float32				getAngleBetweenPoints(glm::vec2& vecPosition1, glm::vec2& vecPosition2);		// return: radians
	static float32				getAngleBetweenPoints(glm::vec3& vecPosition1, glm::vec3& vecPosition2);		// return: radians
	static float32				getAngleBetweenPoints(glm::ivec2& vecPoint1, glm::ivec2& vecPoint2);			// return: radians
	static bool					isPointOnLine(glm::ivec2& vecPoint, glm::ivec2& vecLinePoint1, glm::ivec2& vecLinePoint2);
	static glm::ivec2			getBoundingRectanglePositionForLine(glm::ivec2& vecPoint1, glm::ivec2& vecPoint2);
	static glm::uvec2			getBoundingRectangleSizeForLine(glm::ivec2& vecPoint1, glm::ivec2& vecPoint2);

	// rectangles
	inline static bool			isPointInRectangle(glm::vec2& vecPoint, glm::vec2& vecPosition, glm::uvec2& vecSize);

	static uint32				getRectangleResizeEdges(glm::ivec2& vecPoint, glm::uvec2& vecSize, uint32 uiEdgeDistance); // [return] Edges: Left=1,Top=2,Right=4,Bottom=8
	static uint32				getRectangleResizeEdges(glm::ivec2& vecPoint, glm::ivec2& vecPosition, glm::uvec2& vecSize, uint32 uiEdgeDistance); // [return] Edges: Left=1,Top=2,Right=4,Bottom=8
	static void					getResizePositionAndSizeChange(glm::ivec2& vecCursorChange, uint32 uiResizeEdges, glm::ivec2& vecItemPositionChange, glm::ivec2& vecItemSizeChange);	// [parameter uiResizeEdges] Edges: Left=1,Top=2,Right=4,Bottom=8

	// circles
	static std::vector<glm::vec2>	getRectangleFromCircle(glm::ivec2& vecCenterPosition, float32 fRadius);
	static uint32					convertDiameterToRadius(uint32 uiDiameter);
	static glm::uvec2				getRectangleSizeFromCircle(glm::ivec2& vecPoint, float32 fRadius);

	// triangles
	static std::vector<glm::ivec2>	getEquilateralTrianglePoints(glm::ivec2& vecPoint, float32 fSideLength, uint32 uiPointingDirection);	// [parameter vecPoint] Represents top left point of triangle.
	static std::vector<glm::ivec2>	getEquilateralTrianglePoints(glm::ivec2& vecBottomLeftPoint, glm::ivec2& vecBottomRightPoint);
	static std::vector<glm::ivec2>	getEquilateralTrianglePoints(glm::ivec2& vecBottomLeftPoint, float32 fSideLength, float32 fBaseAngle);
	static std::vector<glm::ivec2>	getIsoscelesTrianglePoints(glm::ivec2& vecPoint, float32 fBaseLength, float32 f2SidesLength, uint32 uiPointingDirection);	// [parameter vecPoint] Represents top left point of triangle.
	static std::vector<glm::ivec2>	getIsoscelesTrianglePoints(glm::ivec2& vecBaseCenterPoint, glm::ivec2& vecTipPoint, uint32 uiBaseHalfWidth);
	static std::vector<glm::ivec2>	getIsoscelesTrianglePoints(glm::ivec2& vecBottomLeftPoint, float32 fBaseLength, float32 fTipAngle, float32 fBaseAngle);
	static float32				getEquilateralTriangleHeightFromSideLength(float32 fTriangleSideLength);
	static float32				getEquilateralTriangleSideLengthFromHeight(float32 fTriangleHeight);
	static float32				getIsoscelesTriangleHeightFrom2SidesLength(float32 fTriangle2SidesLength, float32 fBaseToTipAngle);
	
	// cuboids
	static bool						isPointInCuboid(glm::vec3& vecPointPosition, glm::vec3& vecCuboidPosition, glm::vec3& vecCuboidSize);
	static bool						doesLineIntersectCuboidAA(glm::vec3& vecCuboidPosition, glm::vec3& vecCuboidSize, glm::vec3& vecLinePoint1, glm::vec3& vecLinePoint2, float& t);
	static bool						doesLineIntersectCuboidOBB(
		glm::vec3& ray_origin,        // Ray origin, in world space
		glm::vec3& ray_direction,     // Ray direction (NOT target position!), in world space. Must be normalize()'d.
		glm::vec3& aabb_min,          // Minimum X,Y,Z coords of the mesh when not transformed at all.
		glm::vec3& aabb_max,          // Maximum X,Y,Z coords. Often aabb_min*-1 if your mesh is centered, but it's not always the case.
		glm::mat4& ModelMatrix,       // Transformation applied to the mesh (which will thus be also applied to its bounding box)
		float& intersection_distance // Output : distance between ray_origin and the intersection with the OBB
	);
	static bool						doCuboidsInteresectAA(glm::vec3& vecCuboid1Position, glm::vec3& vecCuboid1Size, glm::vec3& vecCuboid2Position, glm::vec3& vecCuboid2Size);
	static bool						doCuboidsInteresectOBB(glm::vec3& vecCuboid1Position, glm::vec3& vecCuboid1Size, glm::mat4& matCuboid1Model, glm::vec3& vecCuboid2Position, glm::vec3& vecCuboid2Size, glm::mat4& matCuboid2Model, float& fOutput);
	static bool						doesCuboidIntersectSphere(glm::vec3& vecCuboidPosition, glm::vec3& vecCuboidSize, glm::vec3& vecSpherePosition, float fSphereRadius);
	static glm::vec3				getBoundingCuboidMinFromSphere(glm::vec3& vecPosition, float32 fRadius);
	static glm::vec3				getBoundingCuboidMaxFromSphere(glm::vec3& vecPosition, float32 fRadius);
	static std::vector<glm::vec3>	getCuboidVertices(glm::vec3& vecMinPosition, glm::vec3& vecMaxPosition);
	static std::vector<glm::vec3>	getBoundingCuboidVertices(glm::vec3& vecMinPosition, glm::vec3& vecMaxPosition);
	static std::vector<glm::vec3>	getOBBLines(glm::vec3& vecMinAABB, glm::vec3& vecMaxAABB, glm::mat4& matCuboidModel);

private:
	static bool						doCuboidsOverlap(glm::vec3& vecCuboid1Position, glm::vec3& vecCuboid1Size, glm::mat4& matCuboid1Model, glm::vec3& vecCuboid2Position, glm::vec3& vecCuboid2Size, glm::mat4& matCuboid2Model);

public:
	// spheres
	static bool									isPointInSphere(glm::vec3& vecPointPosition, glm::vec3& vecSpherePosition, float32 fRadius);
	static bool									doesLineIntersectSphere(glm::vec3& vecSpherePosition, float32 fRadius, glm::vec3& vecLinePoint1, glm::vec3& vecLinePoint2);
	static bool									doSpheresIntersect(glm::vec3& vecSphere1Position, float fSphere1Radius, glm::vec3& vecSphere2Position, float32 fSphere2Radius);
	static std::vector<glm::vec3>				getSphereVertices(glm::vec3& vecCenter, float32 fRadius, uint32 uiRingCount = 100);
	static std::vector<std::vector<glm::vec3>>	getBoundingSphereVertices(glm::vec3& vecCenter, float32 fRadius, uint32 uiPointCount = 100);

	// ellipses
	static bool					isPointInEllipse(glm::ivec2& vecPoint, glm::ivec2& vecPosition, glm::uvec2& vecSize);
	static glm::ivec2			getEllipseFromRectangle(glm::ivec2& vecPoint, glm::uvec2& vecSize);

	// polygons
	static bool					isPointInPolygon(glm::ivec2& vecPoint, std::vector<glm::ivec2>& vecPolygonPoints);
	static bool					isPointInPolygon(glm::vec2& vecPoint, std::vector<glm::vec2>& vecPolygonPoints);

	static glm::vec3			getPolygonCenter(std::vector<glm::vec3>& vecPoints);
	static glm::ivec2			getBoundingRectanglePositionForPolygon(std::vector<glm::ivec2>& vecPoints);
	static glm::uvec2			getBoundingRectangleSizeForPolygon(std::vector<glm::ivec2>& vecPoints);
	static std::vector<glm::vec3>	getBoundingCuboidPointsForPolygon(std::vector<glm::vec3>& vecPoints);
	static std::vector<glm::vec3>	getBoundingCuboidPointsForPolygon(std::vector<glm::vec3>& vecMeshPoints, std::vector<GLuint>& vecFaceGroupIndices);

	// coordinate system conversion
	static glm::vec2			getPositionInFrontOfPosition(glm::vec2& vecPosition, float32 fAngleRad, float32 fRadius);		// parameter 2: radians
	static glm::vec3			getPositionInFrontOfPosition(glm::vec3& vecPosition, float32 fAngleRad, float32 fRadius);		// parameter 2: radians
	static glm::vec4			getPositionInFrontOfPosition(glm::vec4& vecPosition, float32 fAngleRad, float32 fRadius);		// parameter 2: radians
	static glm::ivec2			getPositionInFrontOfPosition(glm::ivec2& vecPosition, float32 fAngleRad, float32 fRadius);		// parameter 2: radians

	static glm::vec2			polarToCartesian(float32 fRadius, float32 fAngle);
	static void					cartesianToPolar(glm::vec2& vecCartesian, float& fRadius, float& fAngle);
	static glm::vec3			sphericalToCartesian(float32 fRadius, float32 fInclination, float32 fAzimuthal);
	static void					cartesianToSpherical(glm::vec3& vecCartesian, float& fRadius, float& fInclination, float& fAzimuthal);
	static float				cartesianToSphericalInclination(glm::vec3& vecCartesian);

	static glm::vec4			toQuaternion(double pitch, double roll, double yaw);
	static glm::vec3			toEulerAngle(const glm::vec4& q);
	static void					toPlucker(glm::vec3& point1, glm::vec3& point2, glm::vec3& pluckOut1, glm::vec3& pluckOut2);

	// dimension conversion
	static glm::vec2			convert3Dto2D(glm::vec3& pos, const glm::mat4& viewMatrix, const glm::mat4& projectionMatrix, glm::uvec2& vecScreenSize);

	// HDD
	inline static uint32		convertSectorsToBytes(uint32 uiSectorCount);
	inline static uint32		convertBytesToSectors(uint32 uiByteCount);

	// GPU
	static std::vector<glm::vec3>		getCuboidFaceVerticesAsQuads(glm::vec3& vecMinPosition, glm::vec3& vecMaxPosition); // quads are depreceated in OpenGL

	// integer creation
	static uint32				getUint32(uint8 a, uint8 b, uint8 c, uint8 d, bool bBigEndian);
	
	// vector components
	static glm::vec3			swapYZ(glm::vec3& vecIn);

	// intersections
	static bool					doRectanglesCollide(glm::ivec2& vecRectangle1Position, glm::uvec2& vecRectangle1Size, glm::ivec2& vecRectangle2Position, glm::uvec2& vecRectangle2Size);
	static bool					doesRayIntersectAABB2D(glm::vec3& ray_start, glm::vec3& ray_end, glm::vec3& cuboid_min, glm::vec3& cuboid_max, float& t);
	inline static int			doesRayIntersectTriangle(float *p, float *d, float *v0, float *v1, float *v2, float& t, float dirLen);
	static bool					doesFrustumIntersectAABB(const Frustum &f, const glm::vec3& vecCuboidMin, const glm::vec3& vecCuboidMax);

	// misc
	static int32_t				getFactorial(int32_t iValue);
	static int32_t				getBinomialCoefficient(int32_t n, int32_t k);
	static float32				getLength(glm::vec3& vecVector);
	static float32				getZDirection(glm::vec2& vecVector); // The return value is in radians.
	static float32				getZDirection(glm::vec3& vecVector); // The return value is in radians.
	static glm::vec2			getVelocityFromSpeedAndDirection(float32 fSpeed, float32 fDirectionRad);

	// glm
	static float32				getXAngleBetweenPoints(glm::vec3& vecPosition1, glm::vec3& vecPosition2);			// return: radians
	static float32				getYAngleBetweenPoints(glm::vec3& vecPosition1, glm::vec3& vecPosition2);			// return: radians

	static void					increaseAndLimit(glm::vec3& vecVector, glm::vec3& vecIncreaseBy, glm::vec3& vecMaxVector);
	static void					increaseAndLimit(glm::vec3& vecVector, glm::vec3& vecIncreaseBy, glm::vec3& vecMinVector, glm::vec3& vecMaxVector);
	static void					increaseAndLimit(float& fValue, float fIncreaseBy, float fMin, float fMax);

	static mx::Frustum			frustum(glm::mat4& matModel, float fMaxViewDistance);

	static float				getAngleDiff(float fAngle1Rad, float fAngle2Rad);
	static float				getAngleDiffDeg(float fAngle1Deg, float fAngle2Deg);
	static float				getAngleDiffWithSign(float fAngle1Rad, float fAngle2Rad);

	static bool					isFaceClockwise(glm::vec3& vecTrianglePoint1Position, glm::vec3& vecTrianglePoint2Position, glm::vec3& vecTrianglePoint3Position);
	static glm::vec3			getNormal(glm::vec3& x0, glm::vec3& x1, glm::vec3& x2);

	static float				rand(float fMin, float fMax);
	static glm::vec3			randRGBf(void);

	static glm::mat4			getRotationMatrixZYX(glm::vec3& vecRotationRad);

	static bool					isInteger(double dNumber);
	static unsigned long		reverseUlong(unsigned long uiNumber);
	static unsigned long		getRandomColour(void);

	static glm::vec3			interpolate(glm::vec3& vecStart, glm::vec3& vecEnd, float fProgress);
	static glm::vec3			sphericalInterpolate(glm::vec3& vecStart, glm::vec3& vecEnd, float fProgress);
	static glm::quat			sphericalInterpolate(glm::quat& vecStart, glm::quat& vecEnd, double fProgress);

	static uint32				getFirstRangeInRanges(std::map<uint32, uint32>& map, uint32 uiRangeSize, uint32 uiMaxRangesSize);
};

uint32				mx::Math::convertSectorsToBytes(uint32 uiSectorCount)
{
	return uiSectorCount * SECTOR_BYTE_COUNT;
}

uint32				mx::Math::convertBytesToSectors(uint32 uiByteCount)
{
	return (uint32) ceil(((float32)uiByteCount) / ((float32)SECTOR_BYTE_COUNT));
}

bool				mx::Math::isPointInRectangle(glm::vec2& vecPoint, glm::vec2& vecPosition, glm::uvec2& vecSize)
{
	return vecPoint.x >= vecPosition.x
		&& vecPoint.y >= vecPosition.y
		&& vecPoint.x <= (vecPosition.x + (int32)vecSize.x)
		&& vecPoint.y <= (vecPosition.y + (int32)vecSize.y);
}

#define MX_THIRD_PARTY_vector(a,b,c) \
	(a)[0] = (b)[0] - (c)[0];	\
	(a)[1] = (b)[1] - (c)[1];	\
	(a)[2] = (b)[2] - (c)[2];

#define MX_THIRD_PARTY_crossProduct(a,b,c) \
(a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; \
(a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; \
(a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];

#define MX_THIRD_PARTY_innerProduct(v,q) \
((v)[0] * (q)[0] + \
(v)[1] * (q)[1] + \
(v)[2] * (q)[2])

int					mx::Math::doesRayIntersectTriangle(float *p, float *d, float *v0, float *v1, float *v2, float& t, float dirLen)
{
	float e1[3], e2[3], h[3], s[3], q[3];
	float a, f, u, v;
	MX_THIRD_PARTY_vector(e1, v1, v0);
	MX_THIRD_PARTY_vector(e2, v2, v0);

	MX_THIRD_PARTY_crossProduct(h, d, e2);
	a = MX_THIRD_PARTY_innerProduct(e1, h);

	if (a > -0.00001 && a < 0.00001)
		return(false);

	f = 1 / a;
	MX_THIRD_PARTY_vector(s, p, v0);
	u = f * (MX_THIRD_PARTY_innerProduct(s, h));

	if (u < 0.0 || u > 1.0)
		return(false);

	MX_THIRD_PARTY_crossProduct(q, s, e1);
	v = f * MX_THIRD_PARTY_innerProduct(d, q);

	if (v < 0.0 || u + v > 1.0)
		return(false);

	// at this stage we can compute t to find out where
	// the intersection point is on the line
	t = f * MX_THIRD_PARTY_innerProduct(e2, q);

	if (t > 0.00001 && t <= dirLen) // ray intersection
		return(true);

	else // this means that there is a line intersection
		 // but not a ray intersection
		return (false);
}