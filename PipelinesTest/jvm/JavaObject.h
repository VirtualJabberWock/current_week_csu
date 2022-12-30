#pragma once
#include "JavaClass.h"
class JavaObject : JavaClass
{
public:
	jobject obj_ptr;
	JavaObject(jobject ptr);
	JavaObject(jobject ptr, JavaClass* clazz);
	void invokeVoid(string name, int args_c, ...);
	JavaObject invoke(IJavaClass ret, string name, int args_c, ...);
	JavaObject getField(IJavaClass type, string name);
};

