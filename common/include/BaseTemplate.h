#pragma once
#include <span>
template <typename Derived>
class BaseTemplate 
{
public:
    // Non-virtual member methods shared by all derived types
    void ProcessData(std::span<const char> data) {
        static_cast<Derived*>(this)->ProcessData(data);
    }

    bool NotifyComplete() {
        return static_cast<Derived*>(this)->NotifyComplete();
    }

    std::span<const char> WaitNextData() {
        return static_cast<Derived*>(this)->WaitNextData();
    }
};
