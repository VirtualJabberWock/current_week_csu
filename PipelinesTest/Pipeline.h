#pragma once
#include "DataBlock.h"
class Pipeline
{
	/*
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;
	ret = CreatePipe(&pipe_read, &pipe_write, &sa, 0);
	*/

public:

	static HANDLE CreateDefaultPipeline() {
		return CreateNamedPipeW(L"\\\\.\\pipe\\AquaNHPipe",
			PIPE_ACCESS_DUPLEX,
			PIPE_TYPE_MESSAGE |
			PIPE_READMODE_MESSAGE |
			PIPE_NOWAIT,                    //changed from nowait
			PIPE_UNLIMITED_INSTANCES,    // max. instances 
			DataBlock::BLOCK_SIZE,                    // output buffer size 
			DataBlock::BLOCK_SIZE,                    // input buffer size 
			NMPWAIT_WAIT_FOREVER,                // client time-out 
			NULL);
	}

	Pipeline(HANDLE handle);
	Pipeline();
	bool WriteData(DataBlock*);
	bool ReadData(DataBlock* data);
	unsigned char* __ntns__ = new unsigned char[DataBlock::BLOCK_SIZE];
private:
	HANDLE io_handle = 0;

};

