#include "pch.h"
#include "HookListner.h"
#include <sstream>


string HookListner::display(HookInfo info,vector<__int64> args)
{
	string out;
	if (info.argc = 0 || info.args_types.size() == 0) {
		out = "Void!";
		return out;
	}
	ostringstream os;
	os << "Hook[" << std::time(0) <<"] : {\n";
	if (info.args_types.size() > args.size()) {
		out = "Okay, args limit (8)!";
		return out;
	}
	for (int i = 0; i < info.args_types.size(); i++) {
		ArgType type = info.args_types[i];
		char j = 'a' + (i % 20); //fix this
		os << " " << j << " = " << displayType(type, args[i]);
		os << ";\n";
	}
	os << "}";
	out = os.str();
	return out;
}

string HookListner::displayF(HookInfo info, vector<double> args)
{
	string out;
	if (info.argc = 0 || info.args_types.size() == 0) {
		out = "Void!";
		return out;
	}
	ostringstream os;
	os << "Hook : {\n";
	if (info.args_types.size() > args.size()) {
		out = "Okay, args limit (8)!";
		return out;
	}
	for (int i = 0; i < info.args_types.size(); i++) {
		char j = 'a' + (i % 20); //fix this
		char buf[128];
		sprintf_s(buf, 128, "%llf ~ [%llx]", args[i], (__int64)args[i]);
		os << " " << j << " = " << buf;
		os << ";\n";
	}
	os << "}";
	out = os.str();
	return out;
}

union d_64 {
	double x;
	__int64 v;
};

string HookListner::displayType(ArgType t, __int64 v)
{
	if (t == CHAR_T || t == INT_T || t == INT64_T) {
		return to_string(v);
	}
	if (t == PTR_T) {
		char buf[64];
		sprintf_s(buf, 64, "%p", (void*)v);
		return string(buf);
	}
	if (t == DOUBLE_T) {
		char buf[128];
		d_64 t;
		t.v = v;
		sprintf_s(buf, 128, "%llf ~ [%llx]", t.x, t.v);
		return string(buf);
	}
	if (t == FLOAT_T) {
		char buf[128];
		sprintf_s(buf, 128, "%f", (float)v);
		return string(buf);
	}
	if (t == JENV) {
		char buf[64];
		sprintf_s(buf, 64, "[jvm] %p", (void*)v);
		maybe_jvm_env = v;
		return string(buf);
	}
	if (t == JOBJ) {
		char buf[128];
		__int64* p = (__int64*)v;
		__int64 hex = -1;
		if (p != 0) ReadProcessMemory(GetCurrentProcess(), p, &hex, 8, NULL);
		sprintf_s(buf, 128, "[jobject] %p : {%llx}", (void*)v, hex);
		return string(buf);
	}
	return "Unkown type"; 
}

string HookListner::lastError = "no error";
__int64 HookListner::maybe_jvm_env = 0;
map<int, HookListner::HookInfo> HookListner::registred_hooks = map<int, HookListner::HookInfo>();

__int64 HookListner::handleModel(vector<int> data, vector<__int64> args)
{
	if (data.size() == 0) return -1;
	if (registred_hooks.count(data[0]) > 0) {
		HookInfo info = registred_hooks[data[0]];
		if (info.hook_type == HOOK_DISPLAY) {
			string g = display(info, args);
			registred_hooks[data[0]].output = g;
		}
		else {  
			lastError = "Unknown type!";
		}
	}
	else {
		string a = "Hook not registred! With uuid: ";
		string b = to_string(data[0]);
		lastError = a + b;
	}
	return proxyModel(&registred_hooks[data[0]], args);
}

__int64 HookListner::handleModelF(vector<int> data, vector<double> args)
{
	if (data.size() == 0) return -1;
	if (registred_hooks.count(data[0]) > 0) {
		HookInfo info = registred_hooks[data[0]];
		if (info.hook_type == HOOK_DISPLAY) {
			string g = displayF(info, args);
			registred_hooks[data[0]].output = g;
		}
		else {
			lastError = "Unknown type!";
		}
	}
	else {
		string a = "Hook not registred! With uuid: ";
		string b = to_string(data[0]);
		lastError = a + b;
	}
	return proxyModelF(&registred_hooks[data[0]], args);
}

__int64 HookListner::proxyModel(HookInfo* info, vector<__int64> args)
{
	if (info->original == 0) return 0;
	__int64 (*func)(...) = (__int64 (*)(...))info->original;
	if (args[1] > 0)
	{
		lastError = "Hook!";
		return 100000000L;
	}
	if (info->argc > args.size()) return 0;
	if (info->argc == 0) return func();
	if (info->argc == 1) return func(args[0]);
	if (info->argc == 2) return func(args[0], args[1]);
	if (info->argc == 3) return func(args[0], args[1], args[2]);
	if (info->argc == 4) return func(args[0], args[1], args[2], args[3]);
	if (info->argc == 5) return func(args[0], args[1], args[2], args[3], args[4]);
	if (info->argc == 6) return func(args[0], args[1], args[2], args[3], args[4], args[5]);
	if (info->argc == 7) return func(args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
	return 0;
}

__int64 HookListner::proxyModelF(HookInfo* info, vector<double> args)
{
	if (info->original == 0) return 0;
	int (*func)(...) = (int (*)(...))info->original;
	if (info->argc > args.size()) return 0;
	if (info->argc == 0) return func();
	if (info->argc == 1) return func(args[0]);
	if (info->argc == 2) return func(args[0], args[1]);
	if (info->argc == 3) return func(args[0], args[1], args[2]);
	if (info->argc == 4) return func(args[0], args[1], args[2], args[3]);
	if (info->argc == 5) return func(args[0], args[1], args[2], args[3], args[4]);
	if (info->argc == 6) return func(args[0], args[1], args[2], args[3], args[4], args[5]);
	if (info->argc == 7) return func(args[0], args[1], args[2], args[3], args[4], args[5], args[6]);
	return 0;
}
