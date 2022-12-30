#pragma once

//extern "C" __declspec(dllexport)

__int64 __hook_64r64(int id, __int64 a, __int64 b, __int64 c, __int64 d, __int64 x, __int64 y, __int64 z);
int __hook_64r32(int id, __int64 a, __int64 b, __int64 c, __int64 d, __int64 x, __int64 y, __int64 z);
void __hook_64rVoid(int id, __int64 a, __int64 b, __int64 c, __int64 d, __int64 x, __int64 y, __int64 z);
int __hook_LLFr32(int id, double a, double b, double  c, double d, double x, double y, double z);