#include "memory_pool.h"
#include <windows.h>
#include <cassert>

namespace Memory {
	//namespace {
		HANDLE _heap = nullptr;
		LPVOID _alloc = nullptr;
		char* _brk = nullptr;
		size_t _size = 0;
	//}

	void* allocate_mem_pool(size_t size)
	{
		assert(_heap == nullptr);

		_size = size;
		//_heap = HeapCreate(HEAP_NO_SERIALIZE, size, size);
		_heap = GetProcessHeap();
		assert(_heap != nullptr);

		_alloc = HeapAlloc(_heap, HEAP_NO_SERIALIZE/* | HEAP_ZERO_MEMORY*/, size);
		_brk = (char*)_alloc;
		assert(_brk != nullptr);
		return _brk;
	}

	void deallocate_mem_pool()
	{
		assert(_heap != nullptr);
		BOOL success = HeapFree(_heap, HEAP_NO_SERIALIZE, _alloc);
		assert(success == TRUE);
	}

	void* get_sub_pool(size_t byte_size)
	{
		assert(_brk + byte_size <= _brk + _size);
		void* ret = (void*)_brk;
		_brk += byte_size;
		return ret;
	}

};