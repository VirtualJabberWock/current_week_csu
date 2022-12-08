#include "Sphere.h"

#define PRECALACULATED_VOLUME_COFFICIENT 4.1887902047863905 // 4/3 * PI

Sphere* NewSphere()
{
    Sphere* self = (Sphere*)malloc(sizeof(Sphere));
    if (self == 0) return panic("Memory :: error!");
    self->getVolume = Sphere_Private_getVolume;
    return self;
}

double Sphere_Private_getVolume(Sphere* self)
{
    return
        self->radius * self->radius * self->radius * PRECALACULATED_VOLUME_COFFICIENT;
}
