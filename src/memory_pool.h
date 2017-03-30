#pragma once
#include <windows.h>
#include <cassert>

namespace Memory {
	namespace {
		HANDLE _heap = nullptr;
		char* _brk = nullptr;
		size_t _size = 0;
	}

	void allocate_mem_pool(size_t size)
	{
		assert(_heap == nullptr);

		_size = size;
		_heap = HeapCreate(HEAP_NO_SERIALIZE, size, size); // No multi-thread.
		assert(_heap != nullptr);

		_brk = (char*)HeapAlloc(_heap, HEAP_NO_SERIALIZE, size);
		assert(_heap != nullptr);
	}

	void deallocate_mem_pool()
	{
		assert(_heap != nullptr);
		BOOL success = HeapDestroy(_heap);
		assert(success == TRUE);
	}

	void* get_sub_pool(size_t byte_size)
	{
		assert(_brk + size <= _size);
		void* ret = (void*)_brk;
		_brk += byte_size;
		return ret;
	}

};