#pragma once
#include <string>
using namespace std;

class WinMemUtils
{
public:
	enum WinPageProtections
	{
		_PAGE_NOACCESS = 0x01,
		_PAGE_READONLY = 0x02,
		_PAGE_READWRITE = 0x04,
		_PAGE_WRITECOPY = 0x08,
		_PAGE_EXECUTE = 0x10,
		_PAGE_EXECUTE_READ = 0x20,
		_PAGE_EXECUTE_READWRITE = 0x40,
		_PAGE_EXECUTE_WRITECOPY = 0x80,
	};
	static int changeProtection(char* ptr, int sizeInBytes, int protection);
	static __int64 getSymbolByName(string library,string name);
	static inline wstring strToWide(const string& as);
};

