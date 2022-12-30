#pragma once
#include <vector>
#include <map>
#include <string>

using namespace std;

class HookListner
{
public:
	
	enum ArgType {
		CHAR_T,
		INT_T,
		INT64_T,
		DOUBLE_T,
		FLOAT_T,
		PTR_T,
		JENV,
		JOBJ
	};

	typedef int HookType;
	static const HookType HOOK_DISPLAY = 0x1;
	static const HookType HOOK_SCRIPTED = 0x2;

	class HookInfo {
	public:
		HookInfo() {
			output = "<empty>";
			args_types;
			int argc = 0;
			return_type = INT_T;
			args_types = vector<ArgType>();
			original = 0;
			HookType hook_type;
		}
		int uuid;
		
		__int64 address;
		__int64 original;
		vector<ArgType> args_types;
		int argc;
		ArgType return_type;
		HookType hook_type;
		string output;
	
	};
		
	static string lastError;
	static __int64 maybe_jvm_env;
	static string display(HookInfo info ,vector<__int64> args);
	static string displayF(HookInfo info ,vector<double> args);
	static string displayType(ArgType t, __int64 v);
	static map<int, HookInfo> registred_hooks;

	static __int64 handleModel(vector<int> data, vector<__int64> args);
	static __int64 handleModelF(vector<int> data, vector<double> args);
	static __int64 proxyModel(HookInfo* info, vector<__int64> args);
	static __int64 proxyModelF(HookInfo* info, vector<double> args);
};

