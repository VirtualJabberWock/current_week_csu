#include "pch.h"
#include "Pipeline.h"

Pipeline::Pipeline(HANDLE handle)
{
	io_handle = handle;
}

Pipeline::Pipeline()
{
}

bool Pipeline::WriteData(DataBlock* data)
{
	return WriteFile(io_handle, data->GetBuffer(), data->BLOCK_SIZE, NULL, NULL);
}

bool Pipeline::ReadData(DataBlock* data)
{
	BOOL yes = ReadFile(io_handle, static_cast<LPVOID>(__ntns__), DataBlock::BLOCK_SIZE, NULL, NULL);
	if (!yes) return false;
	data->reInit(__ntns__);
	return true;
}


