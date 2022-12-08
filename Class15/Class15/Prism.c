#include "Prism.h"
#include "../../aquaUtils/utils/DebugUtils.h"

PrismForm* NewPrismForm()
{
    PrismForm* self = (PrismForm*)malloc(sizeof(PrismForm));
    if (self == 0) return panic("Memory :: error!");
    self->getVolume = PrismForm_Private_getVolume;
    self->Dispose = PrismForm_Private_Dispose;
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

void PrismForm_Private_Dispose(PrismForm* self)
{
    UnderfinedForm* ubf;
    if (self->baseForm != 0) {
        ubf = self->baseForm;
        if ((ubf->Dispose != 0) && (ubf->Dispose != __NotImplementedMethod)) {
            ubf->Dispose(ubf);
        }
        free(self->baseForm);
        self->baseForm = 0;
    }
    if (self->topForm != 0) {
        ubf = self->topForm;
        if ((ubf->Dispose != 0) && (ubf->Dispose != __NotImplementedMethod)) {
            ubf->Dispose(ubf);
        }
        free(self->topForm);
        self->topForm = 0;
    }
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
    return self;
}

TrianglePrism* NewTrianglePrism(Triangle3* base, Triangle3* top)
{
    PrismForm* self = NewPrismForm();
    self->baseForm = base;
    self->topForm = top;
    if (base == 0)
        return panic_NPE(NewTrianglePrism, "<Triangle3> base");
    if (top == 0)
        return panic_NPE(NewTrianglePrism, "<Triangle3> top");
    if (base->vetrices->size != 3 || top->vetrices->size != 3)
        panic_e("TrianglePrism", "NewTrianglePrism", "Bad Triangle");
    if(!(isPolygonParallelToPlane(base) && isPolygonParallelToPlane(top))) //temp
        panic_e("TrianglePrism", "NewTrianglePrism", "Base not parallel to XZ plane");
    double tmp = top->vetrices->ptr[0].y - base->vetrices->ptr[0].y;
    self->height = AbsoluteDoubleValue(tmp);
}

Parallelepiped* NewParallelepiped(Rectangle3* base, Rectangle3* top)
{
    PrismForm* self = NewPrismForm();
    self->baseForm = base;
    self->topForm = top;
    if (base->vetrices->size != 4 || top->vetrices->size != 4)
        panic_e("Parallelepiped", "NewParallelepiped", "Bad Rectangle");
    if (!(isPolygonParallelToPlane(base) && isPolygonParallelToPlane(top))) //temp
        panic_e("Parallelepiped", "NewParallelepiped", "Base not parallel to XZ plane");
    self->height =
        AbsoluteDoubleValue(top->vetrices->ptr[0].y - base->vetrices->ptr[0].y);
}
