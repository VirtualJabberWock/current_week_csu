#pragma once

#define PI_NUMBER 3.14159265359
#define NOT_A_NUMBER 0x7ff8000000000000L

struct tagVector2 {
	double x;
	double y;
};

typedef struct tagVector2 Vector2, Point;

typedef struct tagVector3 {
	double x;
	double y;
	double z;
} Vector3, Vertex3;

static const Vector3 SPACE_CENTER = { .x = 0, .y = 0, .z = 0 };

typedef struct tagDimUnit {
	double v;
	int __notnull__;
} DimUnit;