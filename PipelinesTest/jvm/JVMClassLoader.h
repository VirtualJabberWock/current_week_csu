#pragma once
#include "jni.h"
#include <string>
#include <set>
#include <Windows.h>

using namespace std;

class JVMClassLoader
{
public:
	static bool loadClass(JNIEnv *env, string classpath, string filepath, int size);
private:
	static set<string> loadedClasses;
};

