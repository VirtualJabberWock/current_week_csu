#include "jni.h"
#include <string>
#include "JavaObject.h"

using namespace std;

class IJavaClass
{
public:
	virtual __int64 getValue();
	virtual string getFullName();
};

class JavaClass : public IJavaClass{

public:
	jclass class_ptr;
	string path;
	JavaClass(jclass class_ptr, string path);
	JavaClass();
	void invokeStaticVoid(string name, int args_c, ...);
	JavaObject invokeStaticObject(IJavaClass ret, string name, int args_c, ...);
	JavaObject getStaticField(IJavaClass type, string name);
	string getFullName();
	__int64 getValue();
	static string getClassName(JNIEnv* env, jclass myCls);
private:
	static string getMethodSignature(IJavaClass retObj, int args_c, ...);
	
};

class JavaInteger : public IJavaClass {
public:
	int v;
	string getFullName();
	__int64 getValue();
	JavaInteger(int v);
};

class JavaLong : public IJavaClass {
public:
	__int64 v;
	string getFullName();
	__int64 getValue();
	JavaLong(__int64 v);
};

class JavaByte : public IJavaClass {
public:
	char v;
	string getFullName();
	__int64 getValue();
	JavaByte(char v);
};

class JavaChar : public IJavaClass {
public:
	char v;
	string getFullName();
	__int64 getValue();
	JavaChar(char v);
};

class JavaVoid : public IJavaClass {
public:
	string getFullName();
	__int64 getValue();
	JavaVoid();
};

class JavaBoolean : public IJavaClass {
public:
	bool v;
	string getFullName();
	__int64 getValue();
	JavaBoolean(bool v);
};

class JavaDouble : public IJavaClass {
public:
	double v;
	string getFullName();
	__int64 getValue();
	JavaDouble(double v);
};