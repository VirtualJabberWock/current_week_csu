#include <stdio.h>
#include "../../aquaUtils/pch.h"
#include "Prism.h"

int main() {
	PrismForm* cyl = NewPrismForm();
	cyl->height = 10;
	Circle3* c3 = NewCircle3(&SPACE_CENTER, 10);
	cyl->baseForm = c3;
	double vol = cyl->getVolume(cyl);
	printf("%lf", vol);
}