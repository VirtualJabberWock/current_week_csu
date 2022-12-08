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
    self->ptr = (Vector3*) initArray(4, sizeof(Vector3));
    self->__notnull__ = 1;
    return self;
}

void _Private_Vertex3V_Put(Vertex3V* self, Vector3* value)
{
    self->size++;
    Vector3* tmp = (Vector3*)realloc((Vector3*)self->ptr, (self->size) * sizeof(Vector3));
    if (tmp == NULL) panic("Array<Vector3>::Memory allocation error!");
    self->ptr[self->size - 1].x = value->x;
    self->ptr[self->size - 1].y = value->y;
    self->ptr[self->size - 1].z = value->z;
}

void _Private_Vertex3V_Clear(Vertex3V* self)
{
    if(self->ptr != 0) free(self->ptr);
    self->ptr = (Vector3*)initArray(0, sizeof(Vector3));
    self->size = 0;
}

Polygon3* NewPolygon3()
{
    Polygon3* self = (Polygon3*)malloc(sizeof(Polygon3));
    if (self == 0) return panic("Memory :: error!");
    self->cached_area.__notnull__ = 0;
    self->cached_area.v = (double)(0x7f'ff'ff'ff'ff'ff'ff'ffu); //NaN
    self->vetrices = NewVertex3V();
    self->__cache = NewVertex3V();
    self->getArea = Polygon_Private_getArea;
    self->getNormal = Polygon_Private_getNormal;
    self->Dispose = Polygon_Private_dispose;
}

Triangle3* NewTriangle3(Vertex3* p0, Vertex3* p1, Vertex3* p2)
{
    Polygon3* self = NewPolygon3();
    self->vetrices->put(self->vetrices, p0);
    self->vetrices->put(self->vetrices, p1);
    self->vetrices->put(self->vetrices, p2);
    return self;
}

Rectangle3* NewRectangle3(Vertex3* p0, Vertex3* p1, Vertex3* p2, Vertex3* p3)
{
    Polygon3* self = NewPolygon3();
    self->vetrices->put(self->vetrices, p0);
    self->vetrices->put(self->vetrices, p1);
    self->vetrices->put(self->vetrices, p2);
    self->vetrices->put(self->vetrices, p3);
    return self;
}

double Polygon_Private_getArea(Polygon3* self)
{
    double area = 0.0;
    if (self->vetrices->size == 0) return 0.0;
    if (self->cached_area.__notnull__) {
        if (self->vetrices->size == self->__cache->size) {
            int check = 1;
            for (int i = 0; i < self->__cache->size; i++) {
                check &= (self->vetrices->ptr[i].x == self->__cache->ptr[i].x);
                check &= (self->vetrices->ptr[i].y == self->__cache->ptr[i].y);
                check &= (self->vetrices->ptr[i].z == self->__cache->ptr[i].z);
                if (check == 0) break;
            }
            if (check) return self->cached_area.v; // cached
            else self->cached_area.__notnull__ = 0; // data changed -> don't validate cache in next call
        }
    }
    //TODO :: remade this to any plane rotated relative to main XZ plane
    int j = self->vetrices->size - 1;
    for (int i = 0; i < self->vetrices->size; i++) {
        double _x = (self->vetrices->ptr[i].x + self->vetrices->ptr[j].x);
        //double _y = (self->vetrices->ptr[i].y + self->vetrices->ptr[j].y); // this is don't work
        double _z = (self->vetrices->ptr[j].z - self->vetrices->ptr[i].z);
        area += _x  * _z;
        j = i;
    }
    area = area / 2.0;
    area = area * (-1.0 * (area < 0.0)); // abs
    self->cached_area.v = area;
    self->cached_area.__notnull__ = 1;
    if (self->__cache->size != 0) self->__cache->clear(self->__cache);
    for (int i = 0; i < self->vetrices->size; i++) {
        Vector3* shadow = NewVector3(
            self->vetrices->ptr[i].x,
            self->vetrices->ptr[i].y,
            self->vetrices->ptr[i].z
        );
        self->__cache->put(self->__cache,shadow);
    }
    return area;
}


void Polygon_Private_dispose(Polygon3* self)
{
    self->cached_area.__notnull__ = 0;
    self->vetrices->clear(self->vetrices);
    self->__cache->clear(self->__cache);
    free(self->vetrices);
    free(self->__cache);
}

Vector3* Polygon_Private_getNormal(Polygon3* self)
{
    if (self == 0)
        return panic_NPE(Polygon_Private_getNormal, "<Polygon> self");
    if (self->vetrices->size < 3)
        return panic_e("Polygon", "getNormal", "Invalid polygon!");

    Vector3* n = NewVector3(0, 0, 0);
    double a1 = self->vetrices->ptr[1].x - self->vetrices->ptr[0].x;
    double b1 = self->vetrices->ptr[1].y - self->vetrices->ptr[0].y;
    double c1 = self->vetrices->ptr[1].z - self->vetrices->ptr[0].z;
    double a2 = self->vetrices->ptr[2].x - self->vetrices->ptr[0].x;
    double b2 = self->vetrices->ptr[2].y - self->vetrices->ptr[0].y;
    double c2 = self->vetrices->ptr[2].z - self->vetrices->ptr[0].z;
    n->x = b1 * c2 - b2 * c1;
    n->y = a2 * c1 - a1 * c2;
    n->z = a1 * b2 - b1 * a2;
    return n;
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
    self->getNormal = __NotImplementedMethod; //TODO
    self->Dispose = __NotImplementedMethod; //TODO
}

double Circle3_Private_getArea(Circle3* self)
{
    return self->radius * self->radius * PI_NUMBER;
}

int isPolygonParallelToPlane(Polygon3* polygon)
{
    if (polygon == 0) 
        return panic_NPE(isPolygonParallelToPlane, "<Polygon3> polygon");
    int check = 1;
    for (int i = 0; i < polygon->vetrices->size - 1; i++) {
        /*i think this is not good way to ...*/
        check &= ((__int64)polygon->vetrices->ptr[i].y == (__int64)polygon->vetrices->ptr[i+1].y);
        if (check == 0) return 0;
    }
    return 1;
}

double Plane_scalar(Vector3* normal, Vector3* point)
{
    return
        - normal->x * point->x
        - normal->y * point->y
        - normal->z * point->z;
}

/*
TODO maybe cache this?
*/
double Plane_distanceToPoint(Vector3* plane_normal, Vector3* point_on, Vertex3* point_out) {

    double D = Plane_scalar(plane_normal, point_on);
    double d_u = AbsoluteDoubleValue(
        plane_normal->x * point_out->x +
        plane_normal->y * point_out->y +
        plane_normal->z * point_out->z +
        D
    );
    double d_w = sqrt(
        plane_normal->x * plane_normal->x +
        plane_normal->y * plane_normal->y +
        plane_normal->z * plane_normal->z
    );
    return d_u / d_w;
}