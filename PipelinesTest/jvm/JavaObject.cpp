#include "JavaObject.h"
#include "../JavaContext.h"

JavaObject::JavaObject(jobject ptr) {
	
	JNIEnv* env = JavaContext::env;
	if (env == 0) throw exception("Null env [obj]");
	class_ptr = env->GetObjectClass(ptr);
	if (class_ptr == 0) throw exception("Null class [obj]");
	path = JavaClass::getClassName(env, class_ptr);
	obj_ptr = ptr;
}

JavaObject::JavaObject(jobject ptr, JavaClass* clazz)
{
    class_ptr = clazz->class_ptr;
    path = clazz->path;
    obj_ptr = ptr;
}

void JavaObject::invokeVoid(string name, int args_c, ...)
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
    if (obj_ptr == 0) throw exception("Null object");
    jmethodID method = env->GetMethodID(class_ptr, name.c_str(), __sig.c_str());
    string tmp = "Bad method [" + name + "] sig: " + __sig;
    if (method == 0) throw exception(tmp.c_str());
    env->CallVoidMethodV(obj_ptr, method, ptr);
    va_end(ptr);
}

JavaObject JavaObject::invoke(IJavaClass ret, string name, int args_c, ...)
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
    string tmp = "Null object: " + getFullName();
    if (obj_ptr == 0) throw exception(tmp.c_str());
    jmethodID method = env->GetMethodID(class_ptr, name.c_str(), __sig.c_str());
    tmp = "Bad method [" + name + "] sig: " + __sig;
    if (method == 0) throw exception(tmp.c_str());
    jobject obj = env->CallObjectMethodV(obj_ptr, method, ptr);
    va_end(ptr);
    return JavaObject(obj);
}

JavaObject JavaObject::getField(IJavaClass type, string name)
{
    JNIEnv* env = JavaContext::env;
    if (env == 0) throw exception("Null Env");
    if (class_ptr == 0) throw exception("Null class");
    jfieldID mid = env->GetFieldID(class_ptr, name.c_str(), type.getFullName().c_str());
    string tmp = "Bad field [" + name + "] type: " + type.getFullName();
    if (mid == 0) throw exception(tmp.c_str());
    jobject obj = env->GetObjectField(class_ptr, mid);
    return JavaObject(obj);
}
