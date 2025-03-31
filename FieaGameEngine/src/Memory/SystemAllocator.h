#pragma once
#include <cstdlib>

namespace Fiea::Engine::Memory
{
    /// @brief A super-simple allocator to bypass our memory system
    /// @tparam T 
    template <class T>
    class SystemAllocator
    {
    public:
        using value_type = T;
        using pointer = T*;

        SystemAllocator() noexcept {}
        template <class U> SystemAllocator(SystemAllocator<U> const&) noexcept {}

        pointer allocate(std::size_t n)
        {
            return (pointer) malloc(n * sizeof(value_type));
        }

        void deallocate(pointer p, std::size_t) noexcept
        {
            free(p);
        }
    };

    template <class T, class U>
    bool operator==(SystemAllocator<T> const&, SystemAllocator<U> const&) noexcept
    {
        return true;
    }
    template <class T, class U>
    bool operator!=(SystemAllocator<T> const& x, SystemAllocator<U> const& y) noexcept
    {
        return !(x == y);
    }
}