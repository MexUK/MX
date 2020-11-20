#pragma once

#include "mx.h"
#include "Plane.h"

struct mx::Frustum
{
	Plane _near;
	Plane _far;
	Plane left;
	Plane right;
	Plane top;
	Plane bottom;
};