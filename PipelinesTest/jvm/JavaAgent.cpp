#include "JavaAgent.h"
#include "../JavaContext.h"

int const JavaAgent::VERSION = JNI_VERSION_1_8;

JNIEnv* JavaAgent::getEnv()
{
    if (current_enviroment != 0) {
        return current_enviroment;
    }
    else {
        return attach(local_enviroment);
    }
}

void JavaAgent::reattach()
{
    if (_jvm != 0) _jvm->DetachCurrentThread();
    current_enviroment = attach(local_enviroment);
    JavaContext::env = current_enviroment;
}

void JavaAgent::detach()
{
    if (_jvm != 0) _jvm->DetachCurrentThread();
    current_enviroment = 0;
    JavaContext::env = current_enviroment;
}

JavaClass JavaAgent::NativeFindClass(const char* name)
{
    jclass found = current_enviroment->FindClass(name);
    if (found == 0) throw exception("Class not found!");
    return JavaClass(found, name);
}

JavaAgent::JavaAgent(JNIEnv* from)
{
    local_enviroment = from;
    _jvm = 0;
    from->GetJavaVM(&_jvm);
    current_enviroment = attach(local_enviroment);
    JavaContext::env = current_enviroment;
}

JNIEnv* JavaAgent::attach(JNIEnv* from)
{
    JNIEnv* env = (JNIEnv*)from;
    env->GetJavaVM(&_jvm);
    JNIEnv* myNewEnv;
    JavaVMAttachArgs args;
    args.version = VERSION;
    args.name = NULL; 
    args.group = NULL; 
    _jvm->AttachCurrentThread((void**)&myNewEnv, &args);
}
