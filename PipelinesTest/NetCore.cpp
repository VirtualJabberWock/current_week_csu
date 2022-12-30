#include "pch.h"
#include "NetCore.h"
#include "MemUtils.h"
#include <ctime>
#include "HookListner.h"
#include <jni.h>
#define RETURN out = //RETURN to SERVER

static __int64 local_procedure = 0;

NetCore::NetCore(
	function<string(int c)> getInfoChannels_,
	function<HMODULE()> getContext_,
	function<int(string obj_name, map<string, long long>)> setObject_
	)
{
	getInfoChannels = getInfoChannels_;
	getContext = getContext_;
	setObject = setObject_;
}

wstring widestr(const std::string& s)
{
	std::wstring wsTmp(s.begin(), s.end());
	return wsTmp;
}

void NetCore::getServerResponse(DataBlock& out, DataBlock& client)
{
	lifetime = (lifetime + 1) % 1000000000L;
	out.dispose();
	if (client.data_type == DataBlock::TYPE_DEFAULT) {
		RETURN DataBlock::CreateDefault();
		return;
	}
	if (client.data_type == DataBlock::TYPE_STRING) {
		string client_request = client.GetString();
		vector<string> args = vector<string>();
		vector<string> parsed_0 = split(client_request, "<-");
		if (parsed_0.size() == 2) {
			args = split(parsed_0.at(1), "|");
		}
		if (doCommand(parsed_0.at(0), args, out)) {
			args.clear();
			args.shrink_to_fit();
			parsed_0.clear();
			parsed_0.shrink_to_fit();
			if(out.disposed == 1) RETURN DataBlock("Segmentaion fault || VALUE NOT RETURNED");
			return;
		}
		RETURN DataBlock("Echo: --" + client.GetString() + "-- META: " + to_string(parsed_0.size()));
		return;
	}
	RETURN DataBlock("Default Message: life_time = " + to_string(lifetime));
}

DWORD WINAPI procedure(LPVOID param) {
	if (local_procedure == 0) return 0;
	void (*foo)(void) = (void (*)()) local_procedure;
	MessageBoxW(0, L"Test", L"text", 0x40L);
	foo();
	//goto ********
	return 0;
}


bool NetCore::doCommand(string cmd, vector<string> args, DataBlock& out)
{
	if (cmd == "info_send") {
		string info = getInfoChannels(2);
		RETURN DataBlock("SocketSend: " + info);
		return true;
	}
	if (cmd == "info_hook") {
		string info = getInfoChannels(3);
		RETURN DataBlock("SocketSend: " + info);
		return true;
	}
	if ((cmd == "info_dynamic_hook") && (args.size() == 1)) {
		if (HookListner::registred_hooks.size() == 0) {
			RETURN DataBlock("DynamicHook: null!");
			return true;
		}
		int uuid = stoi(args[0]);
		if (HookListner::registred_hooks.count(uuid) > 0) {
			string info = HookListner::registred_hooks[uuid].output;
			info = info + "\nLAST_ERROR = " + HookListner::lastError;
			RETURN DataBlock("DynamicHook :: \n" + info);
		}
		else {
			RETURN DataBlock("DynamicHook: invalid uuid, don't exists!");
		}
		return true;
	}
	if ((cmd == "set_dynamic_hook_proxy") && (args.size() == 2)) {
		if (HookListner::registred_hooks.size() == 0) {
			RETURN DataBlock("DynamicHook: null!");
			return true;
		}
		int uuid = stoi(args[0]);
		if (HookListner::registred_hooks.count(uuid) > 0) {
			HookListner::registred_hooks[uuid].original = stoll(args[1], nullptr, 16);
			RETURN DataBlock("DynamicHook :: UPDATED");
		}
		else {
			RETURN DataBlock("DynamicHook: invalid uuid, don't exists!");
		}
		return true;
	}
	if ((cmd == "set_jvm_env") && (args.size() >= 1)) {
		__int64 a = stoll(args[0], nullptr, 16);
		if (args.size() == 2) {
			jvm_test_reflect = stoll(args[1], nullptr, 16);
		}
		jvm_env_adress = a;
		RETURN DataBlock("DynamicHook :: UPDATED [jvm]");
		return true;
	}
	if (cmd == "rental_jvm") {
		HMODULE module = GetModuleHandleW(L"jvm.dll");
		if (module == 0) {
			RETURN DataBlock("Module NULL");
			return true;
		}
		HookListner::registred_hooks[0x13371].original = (__int64)module + 0x17d260;
		int status =
			MemUtils::hookFunction(L"java.dll", 0x1A6D8, "hook_64r64", getContext());
		handleError(status, out);
		return true;
	}
	if (cmd == "mine_jvm") {
		HMODULE module = GetModuleHandleW(L"jvm.dll");
		if (module == 0) {
			RETURN DataBlock("Module NULL");
			return true;
		}
		HookListner::registred_hooks[0x13371].original = (__int64)module + 0x179f20;
		int status =
			MemUtils::hookFunction(L"java.dll", 0x1A690, "hook_64r64", getContext());
		handleError(status, out);
		return true;
	}
	if (cmd == "rental_jvm_off") {
		__int64 origin = HookListner::registred_hooks[0x13371].original;
		if (origin == 0) return false;
		HMODULE module = GetModuleHandleW(L"java.dll");
		if (module == 0) {
			RETURN DataBlock("Module NULL");
			return true;
		}
		__int64 address = (__int64)module + 0x1A6D8;
		int a = MemUtils::unlock(address);
		if (a == 1) a = MemUtils::replaceInt64(address, origin);
		handleError(a, out);
		return true;
	}
	if (cmd == "get_tick") {
		
		RETURN DataBlock("Info: "+to_string(std::time(0))+"\n"+getInfoChannels(4));
		return true;
	}
	if ((cmd == "set_hwid") && (args.size() == 1)) {
		my_hwid = args[0];
	}
	if ((cmd == "setObject") && (args.size() == 2)) {
		map<string, long long> json = parseJson(args[1]);
		if (json.empty()) return false;
		int status = setObject(args[0], json);
		return handleError(status, out);
	}
	if ((cmd == "hook_send") && (args.size() == 1)) {
		int status = 
			MemUtils::hookFunction(L"net.dll", stoll(args[0]), "mySend", getContext());
		handleError(status, out);
		return true;
	}
	//hook_func(module_name, offset, my_func)
	if ((cmd == "hook_func") && (args.size() == 3)) {
		wstring module_name = StringToLPWSTR(args[0].c_str());
		address_t offset = stoll(args[1]);
		string my_func = args[2];
		int status = 
			MemUtils::hookFunction(module_name, offset, my_func, getContext());
		handleError(status, out);
		return true;
	}
	if (cmd == "hook_send_R") {
		int status =
			MemUtils::hookFunction(L"net.dll", 0x10380, "mySend", getContext());
		handleError(status, out);
		return true;
	}
	if (cmd == "hook_send_W") {
		int status =
			MemUtils::hookFunction(L"net.dll", 0x11210, "mySend", getContext());
		handleError(status, out);
		return true;
	}
	if (cmd == "hook_send_WC") { //WarMine.exe + 0x260680; 
		int status =
			MemUtils::hookFunction(L"WarMine.exe", 0x260680, "noSend", getContext());
		status *=
			MemUtils::hookFunction(L"nio.dll", 0x9380, "noSend", getContext());
		/*status *=
			MemUtils::hookFunction(L"WarMine.exe", 0x260680, "mySend", getContext());*/
		handleError(status, out);
		return true;
	}
	if (cmd == "init_jvm") {
		__int64 jvm_dll = (__int64)GetModuleHandleW(L"jvm.dll");
		if (jvm_dll != 0)
		{
			auto created_java_vms = reinterpret_cast<jint(__stdcall*)(JavaVM**, jsize, jsize*)>(GetProcAddress((HMODULE)jvm_dll, "JNI_GetCreatedJavaVMs"));
			JavaVM* vm;
			auto ret = created_java_vms(&vm, 1, nullptr);
			if (vm != 0) {
				JNIEnv* t_env;
				vm->AttachCurrentThread(reinterpret_cast<void**>(&t_env), nullptr);
				vm->GetEnv(reinterpret_cast<void**>(&t_env), JNI_VERSION_1_8);
				jvm_env_adress = (__int64)t_env;
				vm->DetachCurrentThread();
				RETURN DataBlock("JVM found? : " + to_string((__int64) t_env));
				return true;
			}
		}
		RETURN DataBlock("Can't init JVM :(");
		return true;
	}
	//nio.dll+9380
	//jvm.JVM_RegisterSignal+358818
	return false;
}

BOOL NetCore::readNumberArgument(string arg, long long* out, DataBlock& data_out)
{
	try {
		*out = stoll(arg);
	}
	catch(...) {
		data_out = DataBlock::CreateStatus(DataBlock::STATUS_BAD_SYNTAX);
		return 0;
	}
	return 1;
}

bool NetCore::handleError(int status, DataBlock& out)
{
	if(status == 1)
		RETURN DataBlock::CreateStatus(DataBlock::STATUS_OK);
	else
		RETURN DataBlock::CreateStatus(DataBlock::STATUS_FAILED);
	return true;
}


