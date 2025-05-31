#pragma once

#include "BaseTemplate.h"
#include "Buffer.h"
#include "Const.h"
#include <iostream>

class BufferHandler : public BaseTemplate<BufferHandler>
{
public:
	BufferHandler(Buffer& buffer) :buffer_(buffer)
	{}

	void ProcessData(std::span<const char> data)
	{
		size_t tail = buffer_.tail_.load(std::memory_order_acquire);
		size_t head = buffer_.head_.load(std::memory_order_acquire);

		const auto dataSize = data.size();
		size_t nextTail = (tail + dataSize) % buffer_.capacity_;

		if (nextTail == buffer_.head_.load()) // Buffer full
		{
			buffer_.writing_.store(false, std::memory_order_release);
			return;
		}
		for (auto i = 0; i < dataSize; ++i)
		{
			buffer_.dataVec_[(tail + i) % buffer_.capacity_] = data[i];
		}
		std::cout << std::endl;
		buffer_.tail_.store(nextTail, std::memory_order_release);
		buffer_.reading_.store(true, std::memory_order_release);
	}

	bool NotifyComplete()
	{
		return !(buffer_.reading_.load(std::memory_order_acquire)
			|| buffer_.writing_.load(std::memory_order_acquire));
	}

	std::span<const char> WaitNextData()
	{
		if (!buffer_.reading_.load(std::memory_order_acquire)) // No data to read
		{
			return std::span<const char>{};
		}
		size_t head = buffer_.head_.load();
		size_t tail = buffer_.tail_.load();
		size_t dataSize = head <= tail ?
			tail - head
			: buffer_.capacity_ - head + tail;

		size_t newHead = (head + dataSize) % buffer_.capacity_;
		buffer_.head_.store(newHead);
		buffer_.writing_.store(true, std::memory_order_release);
		if (newHead == buffer_.tail_.load()) // Buffer empty
		{
			buffer_.reading_.store(false, std::memory_order_release);
		}
		return std::span<const char>{buffer_.dataVec_}.subspan(head, dataSize);
	}
private:
	Buffer& buffer_;
};