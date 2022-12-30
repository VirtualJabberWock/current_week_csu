#pragma once
#include "jni.h"
#include "JavaClass.h"

class JavaAgent
{
public:

	static const int VERSION;

	JNIEnv* getEnv();
	void reattach();
	void detach();
	JavaClass NativeFindClass(const char* name);

	JavaAgent(JNIEnv* from);

private:

	JNIEnv* local_enviroment;

	JNIEnv* current_enviroment;
	JNIEnv* attach(JNIEnv* from);
	JavaVM* _jvm;
};

