#include "StaticHooks.h"
#include "HookListner.h"
#include <vector>

__declspec(noinline) __int64 __hook_64r64(int id,__int64 a, __int64 b, __int64 c, __int64 d, __int64 x, __int64 y, __int64 z)
{
	vector<__int64> args = vector<__int64>();
	vector<int> data = vector<int>();
	args.push_back(a);
	args.push_back(b);
	args.push_back(c);
	args.push_back(d);
	args.push_back(x);
	args.push_back(y);
	args.push_back(z);
	data.push_back(id);
	return HookListner::handleModel(data, args);
}

__declspec(noinline) int __hook_64r32(int id, __int64 a, __int64 b, __int64 c, __int64 d, __int64 x, __int64 y, __int64 z)
{
	vector<__int64> args = vector<__int64>();
	vector<int> data = vector<int>();
	args.push_back(a);
	args.push_back(b);
	args.push_back(c);
	args.push_back(d);
	args.push_back(x);
	args.push_back(y);
	args.push_back(z);
	data.push_back(id);
	return (int)HookListner::handleModel(data, args);
}

__declspec(noinline) void __hook_64rVoid(int id, __int64 a, __int64 b, __int64 c, __int64 d, __int64 x, __int64 y, __int64 z)
{
	vector<__int64> args = vector<__int64>();
	vector<int> data = vector<int>();
	args.push_back(a);
	args.push_back(b);
	args.push_back(c);
	args.push_back(d);
	args.push_back(x);
	args.push_back(y);
	args.push_back(z);
	data.push_back(id);
	HookListner::handleModel(data, args);
}

__declspec(noinline) int __hook_LLFr32(int id, double a, double b, double c, double d, double x, double y, double z)
{
	vector<double> args = vector<double>();
	vector<int> data = vector<int>();
	args.push_back(a);
	args.push_back(b);
	args.push_back(c);
	args.push_back(d);
	args.push_back(x);
	args.push_back(y);
	args.push_back(z);
	data.push_back(id);
	HookListner::handleModelF(data, args);
}

