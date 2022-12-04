#include "PlaneInSpace.h"
#include "../../aquaUtils/utils/ArrayUtils.h"
#include <math.h>

Vertex3V* NewVertex3V()
{
    Vertex3V* self = (Vertex3V*)malloc(sizeof(Vertex3V));
    if (self == 0) return panic("Memory :: error!");
    self->size = 0;
    self->put = _Private_Vertex3V_Put;
    self->clear = _Private_Vertex3V_Clear;
    self->ptr = (Vector3*) initBucket(0);
    self->__notnull__ = 1;
    return self;
}

void _Private_Vertex3V_Put(Vertex3V* self, Vector3* value)
{
    pushToBucket(&(self->ptr), &(self->size), value);
}

void _Private_Vertex3V_Clear(Vertex3V* self)
{
    freeBucket(self->ptr, self->size);
    self->ptr = initBucket(0);
    self->size = 0;
}

Polygon3* NewPolygon3()
{
    Polygon3* self = (Polygon3*)malloc(sizeof(Polygon3));
    if (self == 0) return panic("Memory :: error!");
    self->cached_area.__notnull__ = 0;
    self->cached_area.v = (double)(0x7f'ff'ff'ff'ff'ff'ff'ffu); //NaN
    self->vetrices = NewVertex3V();
    self->getArea;
}

Triangle3* NewTriangle3(Vertex3* p0, Vertex3* p1, Vertex3* p2)
{
    Polygon3* self = NewPolygon3();
    self->vetrices->put(self->vetrices, p0);
    self->vetrices->put(self->vetrices, p1);
    self->vetrices->put(self->vetrices, p2);
    return self;
}

double Polygon_Private_getArea(Polygon3* self)
{
    double area = 0.0;
    if (self->vetrices->size == 0) return 0.0;

    if (self->cached_area.__notnull__)
        return self->cached_area.v; // cached

    int j = self->vetrices->size - 1;
    for (int i = 0; i < self->vetrices->size; i++) {
        double _x = (self->vetrices->ptr[i].x + self->vetrices->ptr[j].x);
        double _y = (self->vetrices->ptr[i].y + self->vetrices->ptr[j].y);
        double _z = (self->vetrices->ptr[i].z + self->vetrices->ptr[j].z);
        area += _x * _y * _z;
        j = i;
    }
    area = area / 2.0;
    area = area * (-1.0 * (area < 0.0)); // abs
    self->cached_area.v = area;
    self->cached_area.__notnull__ = 1;
    return area;
}

Circle3* NewCircle3(Vector3* center, double radius)
{
    Circle3* self = (Circle3*)malloc(sizeof(Circle3));
    if (self == 0) return panic("Memory :: error!");
    self->getArea = Circle3_Private_getArea;
    self->center.x = center->x;
    self->center.y = center->y;
    self->center.z = center->z;
    self->radius = radius;
}

double Circle3_Private_getArea(Circle3* self)
{
    return self->radius * self->radius * PI_NUMBER;
}
