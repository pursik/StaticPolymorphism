#pragma once

#include <cstdint> 
#include <atomic>
#include <vector>

struct Buffer
{
	std::vector<char> dataVec_; ///< Vector 
	const size_t capacity_; ///< Capacity of the buffer
	std::atomic<size_t> head_;        // Head index (atomic for lock-free operations)
	std::atomic<size_t> tail_;        // Tail index (atomic for lock-free operations)
	std::atomic<bool> writing_ = true; ///< Atomic flag indicating if writing is enabled
	std::atomic<bool> reading_ = false; ///< Atomic flag indicating if reading is enabled

	Buffer(size_t capacity)
		: capacity_(capacity), head_(0), tail_(0) 
	{
		// Initialize the buffer with the specified capacity
		dataVec_.resize(capacity);
		head_.store(0, std::memory_order_release);
		tail_.store(0, std::memory_order_release);
		writing_.store(true, std::memory_order_release);
		reading_.store(false, std::memory_order_release);
	}
};

