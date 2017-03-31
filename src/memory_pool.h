#pragma once

namespace Memory {
	void* allocate_mem_pool(size_t size);
	void deallocate_mem_pool();
	void* get_sub_pool(size_t byte_size);
};