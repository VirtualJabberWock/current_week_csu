#include "Prism.h"
#include "../../aquaUtils/utils/DebugUtils.h"

PrismForm* NewPrismForm()
{
    PrismForm* self = (PrismForm*)malloc(sizeof(PrismForm));
    if (self == 0) return panic("Memory :: error!");
    self->getVolume = PrismForm_Private_getVolume;
    return self;
}

double PrismForm_Private_getVolume(PrismForm* self)
{
    if (self->baseForm == 0) return NOT_A_NUMBER;
    if (self->height == 0) return 0.0;
    UnderfinedForm* uf = self->baseForm;
    if (uf->getArea == 0) 
        return panic_e("PrismForm", "getVolume", "bad form");
    return uf->getArea(uf) * self->height;
}

Cylinder* NewCylinder(Circle3* base, double height)
{
    PrismForm* self = NewPrismForm();
    self->baseForm = base;
    Vector3 top_point = {
        .x = base->center.x,
        .y = base->center.y + height,
        .z = base->center.z
    };
    Circle3* top = NewCircle3(&top_point, base->radius);
    self->topForm = top;
    self->height = height;
}

TrianglePrism* NewTrianglePrism(Triangle3* base, Triangle3* top)
{
    PrismForm* self = NewPrismForm();
    self->baseForm = base;
    self->topForm = top;
    //self->height = ;
}
