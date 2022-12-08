#include "Pyramid.h"

Pyramid* NewPyramid(UnderfinedFormP base, Vertex3* vertex)
{
    Pyramid* self = (Pyramid*)malloc(sizeof(Pyramid));
    if (self == 0) return panic("Memory :: error!");
    self->getVolume = Pyramid_Private_getVolume;
    if (base == 0) return panic_NPE(NewPyramid, "<UnderfinedFormP> base");
    self->base = base;
    self->vertex.x = vertex->x;
    self->vertex.y = vertex->y;
    self->vertex.z = vertex->z;
    self->height.__notnull__ = 0;
    self->height.v = (double) NOT_A_NUMBER;
    return self;
}

SquarePyramid* NewSquarePyramid(Rectangle3* base, Vertex3* vertex)
{
    if(base->vetrices->size != 4)
        return panic_e("Pyramid", "NewSquarePiramyd", "Bad Rectangle");
    if (!isPolygonParallelToPlane(base)) // temp
        panic_e("Pyramid", "NewSquarePyramid", "Base not parallel to XZ plane");
    Pyramid* p = NewPyramid(base, vertex);
    Vector3* normal = base->getNormal(base);
    p->height.v = Plane_distanceToPoint(normal, &base->vetrices->ptr[0], vertex);
    p->height.__notnull__ = 1;
    free(normal);
    return p;
}

double Pyramid_Private_getVolume(Pyramid* self)
{
    if (self->height.__notnull__ == 0) return NOT_A_NUMBER;
    if (self->base == 0) return (double)NOT_A_NUMBER;
    UnderfinedForm* b_form = self->base;
    if (self->height.v == 0) return 0.0;
    double base_area = b_form->getArea(b_form);
    return self->height.v * base_area * 1 / 3;
}
