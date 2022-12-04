#include "../../aquaUtils/model/AquaTypes.h"
#include "Dimensions.h"
#include "PlaneInSpace.h"

/*
1. parallelepiped
2. cylinder
3. triangle prism
*/

typedef struct tagPrismForm {
	double height;
	UnderfinedFormP baseForm;
	UnderfinedFormP topForm;
	double (*getVolume)(struct tagPrismForm* form);
} PrismForm, Cylinder, TrianglePrism, Parallelepiped;

PrismForm* NewPrismForm();
double PrismForm_Private_getVolume(PrismForm* self);

Cylinder* NewCylinder(Circle3* base, double height);
TrianglePrism* NewTrianglePrism(Triangle3* base, Triangle3* top);