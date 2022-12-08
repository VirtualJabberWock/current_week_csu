#include <stdio.h>
#include "../../aquaUtils/pch.h"

#include "Prism.h"
#include "Sphere.h"
#include "Pyramid.h"

void testCylinder(double R, double height);
void testTrianglePrism();
void testSphere();

int main() {
	testCylinder(10, 10);
	testTrianglePrism();
	testSphere();
}

void testCylinder(double R, double h)
{
	Circle3* c3 = NewCircle3(&SPACE_CENTER, R); //base
	Cylinder* cyl = NewCylinder(c3, h); // 3d - object
	double vol = cyl->getVolume(cyl);
	printf("\nCylinder volume: %lf\n", vol);
	cyl->Dispose(cyl); // FREE
	free(cyl);
}

void testTrianglePrism()
{
	Vector3* A = NewVector3( 0, 1, 0);
	Vector3* B = NewVector3( 4, 1, 4);
	Vector3* C = NewVector3(-4, 1, 4);
	Triangle3* base = NewTriangle3(A,B,C);
	A->y += 8;
	B->y += 8;
	C->y += 8;
	Triangle3* top = NewTriangle3(A,B,C);
	free(A);
	free(B);
	free(C);
	TrianglePrism* prism = NewTrianglePrism(base, top);
	printf("\nPrism volume: %lf\n", prism->getVolume(prism)); // 8 * 4 * 4 = 128
	prism->Dispose(prism); //FREE
	free(prism);
}

void testSphere()
{
	Sphere* sphere = NewSphere();
	sphere->center.x = SPACE_CENTER.x;
	sphere->center.y = SPACE_CENTER.y;
	sphere->center.z = SPACE_CENTER.z;
	sphere->radius = 10;
	printf("\nPrism volume: %lf\n", sphere->getVolume(sphere)); // 10^3 * 4/3 * PI
	free(sphere);
}
