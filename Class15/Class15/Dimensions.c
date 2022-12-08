#include "Dimensions.h"
#include "../../aquaUtils/utils/DebugUtils.h"

Vector3* NewVector3(double x, double y, double z)
{
	Vector3* self = (Vector3*)malloc(sizeof(Vector3));
	if (self == 0) return panic("Memory error!");
	self->x = x;
	self->y = y;
	self->z = z;
	return self;
}

void* __NotImplementedMethod(void* ptr, ...)
{
	printf("\nWarning: NIM\n");
	return;
}

double AbsoluteDoubleValue(double d)
{
	if (d < 0) return d * -1;
	return d;
}
