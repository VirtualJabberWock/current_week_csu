#pragma once
#include "pch.h"
using namespace std;

class DataBlock
{

public:
	typedef unsigned char BYTE;
	/*
	BLOCK:
	00 - first byte is type of data
	01 - second byte is some flag
	[02-05] - (int) data size;
	[05...2054] - data block;

	ERRORS:
	8 byte -- error code & description: 4 + 4
	*/

	const static int BLOCK_SIZE = 2054;
	const static int DATA_SIZE = 2048;

	const static int TYPE_BYTE_ARRAY = 0x01;
	const static int TYPE_STRING = 0x02;
	const static int TYPE_ERROR = 0x03;
	const static int TYPE_STATUS = 0x04;
	const static int TYPE_DEFAULT = 0x05;

	const static BYTE STATUS_OK = 0x40;
	const static BYTE STATUS_FAILED = 0x41;
	const static BYTE STATUS_UNEXPECTED = 0x42;
	const static BYTE STATUS_BAD_SYNTAX = 0x43;

	bool isInit = false;
	int data_type = -1;
	int data_size = -1;
	int disposed = 0;
	vector<BYTE> data;
	string last_error = "unknown_error";

	DataBlock();
	DataBlock(unsigned char* raw);
	DataBlock(string str);
	DataBlock(BYTE* raw, int type, int size, int flag);
	static DataBlock CreateStatus(BYTE status);
	static DataBlock CreateDefault();
	
	BYTE* GetData();

	void reInit(unsigned char* raw);
	vector<BYTE> GetDataV();
	string GetString();

	BYTE* GetBuffer();

	void dispose();

private:

	const static int FLAG_NONE = 0;

	BYTE* raw_buffer = new BYTE[BLOCK_SIZE];
	BYTE* raw_data = new BYTE[BLOCK_SIZE];

	int flag = -1;
	int readDataSize();
	void readData();
	void writeDataSize(int size);
	void init(unsigned char* raw, int type, int size, int flag);
};

