#include "JavaClass.h"
#include "../JavaContext.h"
#include "JavaObject.h"

#define initNative {v = _v;}
#define nativeName(s) {return s;}
#define nativeValue {return v;}

JavaClass::JavaClass(jclass ptr, string _path)
{
	class_ptr = ptr;
    path = _path;
}

JavaClass::JavaClass()
{}

void JavaClass::invokeStaticVoid(string name, int args_c, ...)
{
    string __sig = "";
    va_list ptr;
    va_start(ptr, args_c);
    string result = "(";
    for (int i = 0; i < args_c; i++) {
        IJavaClass iclass = va_arg(ptr, IJavaClass);
        result = result + iclass.getFullName();
    }
    result = result + ")V";
    JNIEnv* env = JavaContext::env;
    if (env == 0) throw exception("Null Env");
    if (class_ptr == 0) throw exception("Null class");
    jmethodID method = env->GetStaticMethodID(class_ptr, name.c_str(), __sig.c_str());
    string tmp = "Bad method [" + name + "] sig: " + __sig;
    if (method == 0) throw exception(tmp.c_str());
    env->CallStaticVoidMethodV(class_ptr, method, ptr);
    va_end(ptr);
}

JavaObject JavaClass::invokeStaticObject(IJavaClass ret, string name, int args_c, ...)
{
    string __sig = "";
    va_list ptr;
    va_start(ptr, args_c);
    string result = "(";
    for (int i = 0; i < args_c; i++) {
        IJavaClass iclass = va_arg(ptr, IJavaClass);
        result = result + iclass.getFullName();
    }
    result = result + ")" + ret.getFullName();
    JNIEnv* env = JavaContext::env;
    if (env == 0) throw exception("Null Env");
    if (class_ptr == 0) throw exception("Null class");
    jmethodID method = env->GetStaticMethodID(class_ptr, name.c_str(), __sig.c_str());
    string tmp = "Bad method [" + name + "] sig: " + __sig;
    if (method == 0) throw exception(tmp.c_str());
    jobject obj = env->CallStaticObjectMethodV(class_ptr, method, ptr);
    va_end(ptr);
    return JavaObject(obj);
}

JavaObject JavaClass::getStaticField(IJavaClass type, string name)
{
    JNIEnv* env = JavaContext::env;
    if (env == 0) throw exception("Null Env");
    if(class_ptr == 0) throw exception("Null class");
    jfieldID mid = env->GetStaticFieldID(class_ptr, name.c_str(), type.getFullName().c_str());
    string tmp = "Bad field [" + name + "] type: " + type.getFullName();
    if (mid == 0) throw exception(tmp.c_str());
    jobject obj = env->GetStaticObjectField(class_ptr, mid);
    return JavaObject(obj);
}


string JavaClass::getFullName()
{
    string result = "L" + path + ";";
}

__int64 JavaClass::getValue()
{
    return (__int64)class_ptr;
}

string JavaClass::getMethodSignature(IJavaClass ret, int args_c, ...)
{
    va_list ptr;
    va_start(ptr, args_c);
    string result = "(";
    for (int i = 0; i < args_c; i++) {
        IJavaClass iclass = va_arg(ptr, IJavaClass);
        result = result + iclass.getFullName();
    }
    result = result + ")" + ret.getFullName();
    va_end(ptr);
}

string JavaClass::getClassName(JNIEnv* env, jclass myCls) {
    jclass ccls = env->FindClass("java/lang/Class");
    jmethodID mid_getName = env->GetMethodID(ccls, "getName", "()Ljava/lang/String;");
    jstring strObj = (jstring)env->CallObjectMethod(myCls, mid_getName);
    const char* localName = env->GetStringUTFChars(strObj, 0);
    std::string res = localName;
    env->ReleaseStringUTFChars(strObj, localName);
    res.replace(res.begin(), res.end(), '.', '/');
    return res;
}

//bruh



__int64 IJavaClass::getValue()
{
    return 0;
}

string IJavaClass::getFullName()
{
    return "java/lang/Class";
}

string JavaInteger::getFullName() nativeName("I");
string JavaLong::getFullName() nativeName("J");
string JavaChar::getFullName() nativeName("C");
string JavaByte::getFullName() nativeName("B");
string JavaVoid::getFullName() nativeName("V");
string JavaBoolean::getFullName() nativeName("Z");
string JavaDouble::getFullName() nativeName("D");

__int64 JavaInteger::getValue() nativeValue;
JavaInteger::JavaInteger(int _v) initNative;

__int64 JavaLong::getValue() nativeValue;
JavaLong::JavaLong(__int64 _v) initNative;

__int64 JavaByte::getValue() nativeValue;
JavaByte::JavaByte(char _v) initNative;

__int64 JavaChar::getValue() nativeValue;
JavaChar::JavaChar(char _v) initNative;

__int64 JavaBoolean::getValue() nativeValue;
JavaBoolean::JavaBoolean(bool _v) initNative;

__int64 JavaVoid::getValue(){ return 0; }
JavaVoid::JavaVoid(){}

JavaDouble::JavaDouble(double _v) initNative;

union double_convert {
    __int64 b;
    double d;
};

__int64 JavaDouble::getValue()
{
    union double_convert c;
    c.d = v;
    return c.b;
}

