#pragma once
#include <vector>
#include "Windows.h"
using namespace std;
class ExecuteAsm
{
	public:
		static int executeInt(vector<BYTE> asm_);
		static int executeIntInt(vector<BYTE> asm_, vector<int> args);
		static int executeIntInt(char* region, int reg_size, vector<int> args);
};

