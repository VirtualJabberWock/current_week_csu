#include "ExecuteAsm.h"
#include "WinMemUtils.h"

#define ERR_CODE -1 //temp

#define _ASM_C_OPEN(r,s) WinMemUtils::changeProtection(r, s, WinMemUtils::_PAGE_EXECUTE_READ);
#define _ASM_C_CLOSE(r,s) WinMemUtils::changeProtection(r, s, WinMemUtils::_PAGE_READWRITE);
#define _ASM_CONTEXT_(r,s, code) int z = _ASM_C_OPEN(r,s); if(z==0) return ERR_CODE; code; z = _ASM_C_CLOSE(r,s); if(z == 0) return ERR_CODE;


int ExecuteAsm::executeInt(vector<BYTE> asm_)
{
	if (asm_.size() >= 0x2babe) {
		MessageBoxW(0, L"> 2babe", L"Memory error", 0x40L);
		return -1;
	}
	char* region = (char*)malloc(sizeof(char) * asm_.size());
	if (region == 0) return -1;
	for (int i = 0; i < asm_.size(); i++) {
		region[i] = asm_.at(i);
	}
	int (*func)(void) = (int (*)(void))region;
	int b = 0;
	_ASM_CONTEXT_(region, asm_.size(),
		b = func();
	);
	if(region != 0) free(region);
	return b;
}

int ExecuteAsm::executeIntInt(vector<BYTE> asm_, vector<int> args)
{
	if (asm_.size() >= 0x2babe) {
		MessageBoxW(0, L"> 2babe", L"Memory error", 0x40L);
		return -1;
	}
	if (args.size() > 5) {
		MessageBoxW(0, L"Max args is 5", L"Your have crazy func?", 0x40L);
		return -1;
	}
	char* region = (char*)malloc(sizeof(char) * asm_.size());
	if (region == 0) return -1;
	for (int i = 0; i < asm_.size(); i++) {
		region[i] = asm_.at(i);
	}
	int (*func)(...) = (int (*)(...))region;
	int s = args.size();
	int b = 0;
	_ASM_CONTEXT_(region, asm_.size(),
		if (s == 1) b = func(args[0]);
		if (s == 2) b = func(args[0], args[1]);
		if (s == 3) b = func(args[0], args[1], args[2]);
		if (s == 4) b = func(args[0], args[1], args[2], args[3]);
		if (s == 5) b = func(args[0], args[1], args[2], args[3], args[4]);
	);
	if (region != 0) free(region);
	return b;
}

int ExecuteAsm::executeIntInt(char* region, int reg_size, vector<int> args)
{
	if (args.size() > 5) {
		MessageBoxW(0, L"Max args is 5", L"Your have crazy func?", 0x40L);
		return -1;
	}
	if (region == 0) return -1;
	int (*func)(...) = (int (*)(...))region;
	int b = 0;
	int s = args.size();
	_ASM_CONTEXT_(region, reg_size,
		if (s == 1) b = func(args[0]);
		if (s == 2) b = func(args[0], args[1]);
		if (s == 3) b = func(args[0], args[1], args[2]);
		if (s == 4) b = func(args[0], args[1], args[2], args[3]);
		if (s == 5) b = func(args[0], args[1], args[2], args[3], args[4]);
	);
	return b;
}
