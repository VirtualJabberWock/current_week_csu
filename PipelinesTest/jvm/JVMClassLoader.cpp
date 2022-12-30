#include "JVMClassLoader.h"
#include "../FormatUtils.h"

bool JVMClassLoader::loadClass(JNIEnv* env, string classpath, string filepath, int size)
{
    if (loadedClasses.count(classpath) > 0) {
        return false;
    }
    string globalpath = "C:\\root\\dev\\AquaNH\\class\\" + filepath;
    LPWSTR fpath = StringToLPWSTR(globalpath.c_str());
    HANDLE hFile = CreateFileW(fpath,
        GENERIC_READ,          // open for reading
        FILE_SHARE_READ,    // share for reading
        NULL,                  // default security
        OPEN_EXISTING,         // existing file only
        FILE_ATTRIBUTE_NORMAL, // normal file
        NULL);
    if (hFile == 0) {
        return false;
    }
    DWORD dwBytesRead;
    char* bytecode_raw = (char*)malloc(sizeof(char) * size);
    if (ReadFile(hFile, bytecode_raw, size, &dwBytesRead, NULL) == FALSE)
    {
        free(bytecode_raw);
        return false;
    }
    CloseHandle(hFile);
    auto class_loader = env->FindClass("java/lang/ClassLoader");
    auto get_system_loader =
        env->GetStaticMethodID(class_loader, "getSystemClassLoader",
            "()Ljava/lang/ClassLoader;");
    auto system_loader =
        env->CallStaticObjectMethod(class_loader, get_system_loader);
    jclass lol0 = env->DefineClass(classpath.c_str(), system_loader, (const jbyte*)bytecode_raw, size);
    if (lol0 != 0) {
        loadedClasses.insert(classpath);
        return true;
    }
    return false;
}

set<string> JVMClassLoader::loadedClasses = set<string>();
