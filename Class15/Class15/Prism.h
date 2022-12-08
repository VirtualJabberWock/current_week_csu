#pragma once

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
	/*After Disposing, object becomes inaccessible*/
	void (*Dispose)(struct tagPrismForm* form);
} PrismForm, Cylinder, TrianglePrism, Parallelepiped;

PrismForm* NewPrismForm();
double PrismForm_Private_getVolume(PrismForm* self);
void PrismForm_Private_Dispose(PrismForm* self);

/*
Note:
	only parallel to main XZ plane NOT rotated!!!
	it will be fixed later (maybe)
*/

Cylinder* NewCylinder(Circle3* base, double height);
TrianglePrism* NewTrianglePrism(Triangle3* base, Triangle3* top);
Parallelepiped* NewParallelepiped(Rectangle3* base, Rectangle3* top);