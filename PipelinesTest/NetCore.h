#pragma once

#include "DataBlock.h"
#include <functional>
#include "FormatUtils.h"

class NetCore
{
public:

	__int64 jvm_env_adress = 0;
	__int64 jvm_test_reflect = 0;

	string my_hwid = "2";

	NetCore(
		function<string(int c)> getInfoChannels_,
		function<HMODULE()> getContext_,
		function<int(string obj_name, map<string, long long>)> setObject_
	);
	void getServerResponse(DataBlock& out_ptr, DataBlock& client);
	


private:

	function<string(int c)> getInfoChannels;
	function<HMODULE()> getContext;
	function<int(string obj_name, map<string, long long>)> setObject;
	long lifetime = 0;
	bool doCommand(string cmd, vector<string> args, DataBlock& out);
	BOOL readNumberArgument(string arg, long long* out, DataBlock& data_out);
	bool handleError(int status, DataBlock& out);
};

