#pragma once

#include "Dimensions.h"
#include "../../aquaUtils/model/AquaTypes.h"

typedef struct tagVertex3V {
	Vector3* ptr;
	int size;
	void (*put) (struct tagVertex3V* self, Vector3* value);
	void (*clear)(struct tagVertex3V* self);
	Bool __notnull__; // (self == null) ? 0 : 1
} Vertex3V;

Vertex3V* NewVertex3V();

void _Private_Vertex3V_Put(Vertex3V* self, Vector3* value);
void _Private_Vertex3V_Clear(Vertex3V* self);

typedef struct tagPolygon3
{
	double (*getArea)(struct tagPolygon3* self);
	Vertex3V* vetrices;
	DimUnit cached_area;
} Polygon3, Triangle3;

Polygon3* NewPolygon3();
Triangle3* NewTriangle3(Vertex3* p0, Vertex3* p1, Vertex3* p2);
double Polygon_Private_getArea(Polygon3* self);

typedef struct tagCircle3
{
	double (*getArea)(struct tagCircle3* self);
	Vector3 center;
	double radius;
} Circle3;

Circle3* NewCircle3(Vector3* center, double radius);
double Circle3_Private_getArea(Circle3* self);

typedef void* UnderfinedFormP;
typedef struct tagUnderfinedForm {
	double (*getArea)(UnderfinedFormP self);
}UnderfinedForm;


