#pragma once

#include "../../aquaUtils/model/AquaTypes.h"
#include "Dimensions.h"
#include "PlaneInSpace.h"
#include "../../aquaUtils/utils/DebugUtils.h"

typedef struct tagSphere {
	double radius;
	Vector3 center;
	double (*getVolume)(struct tagSphere* self);
} Sphere;

Sphere* NewSphere();
double Sphere_Private_getVolume(Sphere* self);
