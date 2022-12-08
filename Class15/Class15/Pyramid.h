#pragma once

#include "../../aquaUtils/model/AquaTypes.h"
#include "Dimensions.h"
#include "PlaneInSpace.h"
#include "../../aquaUtils/utils/DebugUtils.h"


typedef struct tagPyramid {
	Vector3 vertex;
	UnderfinedFormP base;
	DimUnit height;
	/*@return Volume of pyramid; Warning: NaN possible*/
	double (*getVolume)(struct tagSphere* self);
} Pyramid, SquarePyramid;

Pyramid* NewPyramid(UnderfinedFormP base, Vertex3* vertex);
/*
Note:
	only with base parallel to main XZ plane pyramid, NOT rotated!!!
	it will be fixed later
*/
SquarePyramid* NewSquarePyramid(Rectangle3* base, Vertex3* vertex);
double Pyramid_Private_getVolume(Pyramid* self);
