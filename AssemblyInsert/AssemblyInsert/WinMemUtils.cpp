#include "WinMemUtils.h"
#include <Windows.h>

int WinMemUtils::changeProtection(char* ptr, int sizeInBytes, int protection)
{
    int old_v = 0;
    PDWORD old = (PDWORD) &old_v;
    BOOL status = VirtualProtect(ptr, sizeInBytes, protection, old);
    return status;
}

__int64 WinMemUtils::getSymbolByName(string library, string name)
{
	wstring lib_ = strToWide(library);
	HMODULE m = GetModuleHandleW(lib_.c_str());
	if (m == 0) return 0;
	GetProcAddress(m, name.c_str());
}

inline wstring WinMemUtils::strToWide(const string& as)
{
	// deal with trivial case of empty string
	if (as.empty())    return std::wstring();

	// determine required length of new string
	size_t reqLength = ::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), 0, 0);

	// construct new string of required length
	std::wstring ret(reqLength, L'\0');

	// convert old string to new string
	::MultiByteToWideChar(CP_UTF8, 0, as.c_str(), (int)as.length(), &ret[0], (int)ret.length());

	// return new string ( compiler should optimize this away )
	return ret;
}
