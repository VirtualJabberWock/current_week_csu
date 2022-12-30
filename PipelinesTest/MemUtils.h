#pragma once
#include <string>

#ifdef _WIN64
#define address_t __int64
#else
#define address_t __int32
#endif

class MemUtils
{
public:

	static int unlock(address_t adr) {
		DWORD OldProtection;
		if (VirtualProtect((void *) adr, 128, PAGE_EXECUTE_READWRITE, &OldProtection))
		{
			return 1;
		}
		else {
			return 0;
		}
	}

	static int replaceInt64(address_t adr, address_t value) {
		address_t out_value = value;
		if (adr == 0) {
			MessageBoxW(0,L"null",L"null", MB_ICONINFORMATION);
			return 0;
		}
		try {
			memcpy((void*)adr, (const void*)(&out_value), sizeof(address_t));
		}
		catch (...) {
			return 0;
		}
		return 1;
	}

	static int hookFunction(std::wstring module_name, address_t shift, std::string func, HMODULE context) {
		address_t address = (address_t)GetModuleHandleW(module_name.c_str());
		if (address == 0) return 0;
		address = address + shift;
		address_t value = (address_t)GetProcAddress(context, func.c_str());
		if (value == 0) return 0;
		int a = MemUtils::unlock(address);
		if (a == 1) a = MemUtils::replaceInt64(address, value);
		return a;
	}
	
};

	