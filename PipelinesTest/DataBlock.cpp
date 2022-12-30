#include "pch.h"
#include "DataBlock.h"


DataBlock::DataBlock() {}

DataBlock::DataBlock(unsigned char* raw)
{
	raw_data = raw;
	data_type = raw[0];
	flag = raw[1];
	if (flag > 2) {
		last_error = "Invalid flag";
		return;
	}
	data_size = readDataSize();
	if (data_size > 2048) {
		last_error = "Invalid size of data: "+to_string(data_size);
		return;
	}
	readData();
	isInit = true;
}

DataBlock::DataBlock(BYTE* raw, int type, int size, int flag)
{
	init(raw, type, size, flag);
}

DataBlock::DataBlock(string str)
{
	const char* a = str.c_str();
	if (str.size() > 2048) str = "ERROR: STRING TOO BIG";
	BYTE* buffer_ = new BYTE[str.size()];
	for (int i = 0; i < str.size(); i++) {
		BYTE b = str.at(i);
		buffer_[i] = b;
	}
	init(buffer_, TYPE_STRING, str.size(), FLAG_NONE);
}

DataBlock DataBlock::CreateStatus(BYTE status)
{
	BYTE* raw = new BYTE[1];
	raw[0] = status;
	return DataBlock(raw, TYPE_STATUS, 1, 0);
}

DataBlock DataBlock::CreateDefault()
{
	BYTE* raw = new BYTE[1];
	raw[0] = 1;
	return DataBlock(raw, TYPE_DEFAULT, 1, 0);
}

BYTE* DataBlock::GetData()
{
	if (!isInit) return new BYTE[DATA_SIZE];
	return data.data();
}

void DataBlock::reInit(unsigned char* raw)
{
	raw_data = raw;
	data_type = raw[0];
	flag = raw[1];
	if (flag > 2) {
		last_error = "Invalid flag";
		return;
	}
	data_size = readDataSize();
	if (data_size > 2048) {
		last_error = "Invalid size of data: " + to_string(data_size);
		return;
	}
	readData();
	isInit = true;
}

vector<BYTE> DataBlock::GetDataV()
{
	return data;
}

string DataBlock::GetString()
{
	BYTE* buffer = new BYTE[data_size + 1];
	for (int i = 0; i < data_size; i++) buffer[i] = data.at(i);
	buffer[data_size] = 0;
	string sName(reinterpret_cast<char*>(buffer));
	return sName;
}

BYTE* DataBlock::GetBuffer() {
	return raw_buffer;
}

void DataBlock::dispose()
{
	if (disposed == 1) return;
	data.clear();
	data.shrink_to_fit();
	if (raw_buffer != 0) {
		delete raw_buffer;
	}
	free(raw_data);
	disposed = 1;
}

//private                                                 

void DataBlock::init(unsigned char* raw, int type, int size, int flag)
{
	raw_buffer[0] = type;
	raw_buffer[1] = flag;
	writeDataSize(size);
	for (int i = 0; i < size; i++) {
		raw_buffer[6 + i] = raw[i];
	}
}

int DataBlock::readDataSize()
{
	int a = 0;
	a += raw_data[2] * 0x1000000;
	a += raw_data[3] * 0x10000;
	a += raw_data[4] * 0x100;
	a += raw_data[5];
	return a;
}

void DataBlock::readData()
{
	data.clear();
	data.shrink_to_fit();
	int end_point = 6 + data_size;
	for (int i = 6; i < end_point; i++) {
		data.push_back(raw_data[i]);
	}
}

void DataBlock::writeDataSize(int size)
{
	raw_buffer[2] = (size / 0x1000000);
	raw_buffer[3] = (size / 0x10000) % 0x1000000;
	raw_buffer[4] = (size / 0x100) % 0x10000;
	raw_buffer[5] = (size) % 0x100;
}



