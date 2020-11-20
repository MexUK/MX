#pragma once

#include "mx.h"

struct mx::OBB
{
	glm::vec3			Pos;
	
	glm::vec3			AxisX;
	glm::vec3			AxisY;
	glm::vec3			AxisZ;
	
	glm::vec3			Half_size;
};