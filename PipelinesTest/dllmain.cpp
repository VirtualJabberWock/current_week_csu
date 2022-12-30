//WRITER
#include "pch.h"
#include <filesystem>
#include <fstream>
#include <string>
#include "DataBlock.h"
#include "dllmain.h"
#include "NetCore.h"
#include <winsock.h>
#include <wincrypt.h>
#include <sstream>
#include "SocketHandler.h"
#include "Pipeline.h"
#include <map>

using namespace std;
#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "jvm.lib")

#include <jni.h>

__int64 z = 0;

//CHANNELS
string channel0 = "null channel[0]"; // main info
string channel1 = "null channel[1]"; // socket tests
string channel2 = "null channel[2]";
string channel3 = "null channel[3]"; // something to test
string channel4 = "null channel[4]"; // special case

#include "StaticHooks.h"
#include "HookListner.h"
#include <iostream>
#include "jvm/JVMClassLoader.h"

int hook_cookie_case_0 = 1;
int hook_cookie_case_1 = 2;

extern "C" __declspec(dllexport) __declspec(noinline) __int64 hook_64r64(__int64 a, __int64 b, __int64 c, __int64 d, __int64 x, __int64 y, __int64 z)
{
    int uuid = 0x13371;
    uuid += hook_cookie_case_0;
    uuid = hook_cookie_case_1 ^ uuid;
    return __hook_64r64(uuid, a, b, c, d, x, y, z);
}

extern "C" __declspec(dllexport) __declspec(noinline) int hook_64r32(__int64 a, __int64 b, __int64 c, __int64 d, __int64 x, __int64 y, __int64 z)
{
    int uuid = 0x13372;
    uuid += hook_cookie_case_0;
    uuid = hook_cookie_case_1 ^ uuid;
    return __hook_64r32(uuid, a, b, c, d, x, y, z);
}

extern "C" __declspec(dllexport) __declspec(noinline) void hook_64rVoid(__int64 a, __int64 b, __int64 c, __int64 d, __int64 x, __int64 y, __int64 z)
{
    int uuid = 0x13373;
    uuid += hook_cookie_case_0;
    uuid = hook_cookie_case_1 ^ uuid;
    __hook_64rVoid(uuid, a, b, c, d, x, y, z);
}

extern "C" __declspec(dllexport) __declspec(noinline) void hook_LLFr32(double a, double b, double c, double d, double x, double y, double z)
{
    int uuid = 0x13374;
    uuid += hook_cookie_case_0;
    uuid = hook_cookie_case_1 ^ uuid;
    __hook_LLFr32(uuid, a, b, c, d, x, y, z);
}
      
vector<int> info_channels_update;

//-=-=--

string p_pipe = "C:\\root\\dev\\pipe0\\";

int direction = 0;
HMODULE currentModule;
SocketHandler sockHandler = SocketHandler();
static Pipeline pipeline;
SOCKET last_socket;

typedef std::basic_ofstream<unsigned char, std::char_traits<unsigned char> > uofstream;

void throw_error(string error);

string getChannel(int channel) {
    string result = "[";
    switch (channel)
    {
        case 0: result += channel0; break;
        case 1: result += channel1; break;
        case 2: result += channel2; break;
        case 3: result += channel3; break;
        case 4: result += channel4; break;
    default:
        return "unknown channel: "+to_string(channel);
    }
    if (channel > info_channels_update.size()) return result + "]";
    result += "](last_update=" + to_string(info_channels_update.at(channel)) + ")";
    return result;
}

HMODULE getDLLContext() {
    return currentModule;
}

int setObject(string obj_name, map<string, long long> json) {
    try {
        if (obj_name == "sendMask") {
            sockHandler.setSendMask(
                (int)json["pos"],
                (int)json["len"],
                (int)json["mask_mode"],
                (char)json["arg"]);
            return 1;
        }
        if (obj_name == "len_filter") {
            sockHandler.len_filter = (int)json["len"];
            return 1;
        }
        if (obj_name == "s_packet") {
            int l = (int)json["len"];
            int iter = (int)json["iter"];
            char* a = new char[l];
            for (int i = 0; i < l; i++) a[i] = (char)(0x13 ^ i);
            for(int i = 0;i<iter;i++) send(last_socket, a, l, 0);
            return 1;
        }
    }
    catch (...) {
        throw_error("can't parse json for object" + obj_name);
        return 0;
    }
    return 0;
}

static string dll_info = "";

NetCore core = NetCore(getChannel, getDLLContext, setObject);

DWORD WINAPI main(LPVOID param) {

    HANDLE hPipe;
    BOOL rFlag = 1;

    char buf[1000];
    sprintf_s(buf, 1000, "\nhook_64r64 = 0x%p\n", hook_64r64);
    channel4 = string(buf);

    DataBlock clientPacket;
    DataBlock serverPacket;

    hPipe = Pipeline::CreateDefaultPipeline(); 

    if (hPipe == INVALID_HANDLE_VALUE)
        return 0;

    ConnectNamedPipe(hPipe, NULL);
    pipeline = Pipeline(hPipe);

    while (true)
    {  
        if (hPipe == INVALID_HANDLE_VALUE) break;
        rFlag = pipeline.ReadData(&clientPacket);
        if (rFlag) //Read something from the client!!!!
        {
            if (!clientPacket.isInit) throw_error(clientPacket.last_error);
            core.getServerResponse(serverPacket, clientPacket);
            if (hPipe == INVALID_HANDLE_VALUE) break;
            rFlag = pipeline.WriteData(&serverPacket);
        }
        Sleep(1);
    }

    DisconnectNamedPipe(hPipe);

    return 0;
}

#define assertNull(obj, name) os << name << (__int64)obj << endl; if((__int64) obj == 0) {channel4 = os.str();return 0;}; throw_error(os.str())

int removed = 0;
jboolean j_TRUE = 1;

JNIEXPORT jstring JNICALL hello(JNIEnv* env, jobject thisObject) {
    std::string hello = core.my_hwid;
    return env->NewStringUTF(hello.c_str());
}

static JNINativeMethod methods[] = {
  {(char*)"getHW", (char*)"()Ljava/lang/String;", (void*)&hello },
};

int we_get_it = 0;

DWORD WINAPI keyboard(LPVOID param) {

    //HookListner::HookInfo info;
    //info.args_types.push_back(HookListner::JENV);
    //info.args_types.push_back(HookListner::JOBJ);
    //info.args_types.push_back(HookListner::JOBJ);
    //info.args_types.push_back(HookListner::PTR_T);
    //info.argc = 4;
    //info.uuid = 0x13371;//78705
    //info.hook_type = HookListner::HOOK_DISPLAY;
    //HookListner::registred_hooks[0x13371] = info;
    HookListner::HookInfo info;
    info.args_types.push_back(HookListner::PTR_T);
    info.args_types.push_back(HookListner::PTR_T);
    info.args_types.push_back(HookListner::PTR_T);
    info.args_types.push_back(HookListner::PTR_T);
    info.argc = 4;
    info.uuid = 0x13371;//78705
    info.hook_type = HookListner::HOOK_DISPLAY;
    HookListner::registred_hooks[0x13371] = info;
    hook_cookie_case_0 = 0;
    hook_cookie_case_1 = 0;
    jobject minecraft = 0;
    while (true)
    {
        if (hook_cookie_case_0 == 0x10) {
            printf("cool");
            hook_cookie_case_0 = hook_cookie_case_1 ^ 0b11;
        }
        if (GetAsyncKeyState(VK_UP)) direction = 9;
        if (GetAsyncKeyState(VK_RIGHT)){}
        if (GetAsyncKeyState(VK_DOWN)){
        }
        if (GetAsyncKeyState(VK_LEFT)) {
            if (core.jvm_env_adress == 0 && HookListner::maybe_jvm_env != 0) {
                core.jvm_env_adress = HookListner::maybe_jvm_env;
            }
            if (core.jvm_env_adress != 0) {
                ostringstream os0;
                JNIEnv* env = (JNIEnv*)core.jvm_env_adress;
                JavaVM* jvm;
                env->GetJavaVM(&jvm);
                JNIEnv* myNewEnv;
                JavaVMAttachArgs args;
                args.version = JNI_VERSION_1_8; // choose your JNI version
                args.name = NULL; // you might want to give the java thread a name
                args.group = NULL; // you might want to assign the java thread to a ThreadGroup
                jvm->AttachCurrentThread((void**)&myNewEnv, &args);
                if ((__int64)minecraft == 0) {

                }
                jvm->DetachCurrentThread();
            }
        }
        Sleep(20);
    }
    return 0;
}

extern "C" __declspec(dllexport) int mySend(
    SOCKET     s,
    const char* buf,
    int        len,
    int        flags
){
    string b = string(buf).substr(0,len);
    if ((sockHandler.len_filter == -1) || (len == sockHandler.len_filter)) {
        if ((b.length() > 3) && (b.find("JourneyMap") == -1)) {
            channel2 = "buf="
                + ToHex(b) + ";len="
                + to_string(len) + ";flags="
                + to_string(flags);
            last_socket = s;
            int last = info_channels_update.at(2);
            info_channels_update[2] = (last + 2) % 100000000;
        }
    }
    return sockHandler.handleSend(s, buf, len, flags);
}

extern "C" __declspec(dllexport) int noSend(
    SOCKET     s,
    const char* buf,
    int        len,
    int        flags
) {
    string b = string(buf).substr(0, len);
    if ((sockHandler.len_filter == -1) || (len == sockHandler.len_filter)) {
        channel3 = "[no_send] buf="
            + ToHex(b) + ";len="
            + to_string(len) + ";flags="
            + to_string(flags);
        last_socket = s;
        int last = info_channels_update.at(3);
        info_channels_update[3] = (last + 2) % 100000000;
    }
    return send(s, "\0", len, flags);
}

extern "C" __declspec(dllexport) void myExitCode(
    int exitCode
) {
    //CryptGenRandom(hProv, dwLen, pbBuffer);
    int last = info_channels_update.at(3);
    info_channels_update[3] = (last + 1) % 100000000;
    channel3 = "Exit Code = " + to_string(exitCode);
}

//MSVCP140.std::basic_streambuf<char,std::char_traits<char> >::snextc+0
extern "C" __declspec(dllexport) int myNextC() {
    printf("%lld\n", z);
    z += 1;
    if (z > 17000) Sleep(1000);
    __int64 a = (long long)GetModuleHandleW(L"Crackme2077.exe");
    __int64 b = a + 0x343a;
    void (*func)(void) = (void (*)(void))b;
    func();
    return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    {
        currentModule = hModule;
        for (int i = 0; i < 10; i++) info_channels_update.push_back(0);
        
        CreateThread(nullptr, 0, main, NULL, 0, nullptr);
        CreateThread(nullptr, 0, keyboard, NULL, 0, nullptr);
        return TRUE;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

void throw_error(string error) {
    ofstream("C:\\root\\dev\\AquaNH\\last_error.bin") << error;
}



