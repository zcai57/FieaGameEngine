#include "Heap.h"
#include "../include/FieaGameEngine/Types.h"

namespace Fiea::Engine::Memory 
{
    /// <summary>
    /// Create heap. Call internal heap constructor
    /// </summary>
    /// <param name="name"></param>
    /// <param name="size"></param>
    /// <returns></returns>
    Heap* Heap::CreateHeap(const char* name, size_t size)
    {
        void* ptr = reinterpret_cast<Heap*>(malloc(sizeof(Heap) + Heap::AlignedSize(size)));
        FIEA_ERROR(ptr != nullptr);
        Heap* heap = new(ptr) Heap(name, Heap::AlignedSize(size));
        return heap;
    }
    /// <summary>
    /// Destroy heap. Call internal heap destructor
    /// </summary>
    /// <param name="heap"></param>
    void Heap::DestroyHeap(Heap* heap)
    {
        heap->~Heap();
        free(heap);
    }
    /// <summary>
    /// Return Heap name
    /// </summary>
    /// <returns>string&</returns>
    const string& Heap::GetName() const
    {
        return _name;
    }
    
    // allocate memory
    // sucess return header*
    // failure nullptr
    void* Heap::Alloc(size_t size)
    {
        // For the size to alignment
        size = Heap::AlignedSize(size);
        // if remain space is not large enough, return
        if (size > Available()) {
            return nullptr;
        }
        // Check start
        if (_Start.isFree) {
            void* temp = _TryAlloc(&_Start, size);
            if (temp != nullptr) return (Header*)temp + 1;
        }
        // Loop and check each free header
        Header* cur = _Start._next;
        while (cur != nullptr) {
            // if header is free try alloc
            if (cur->isFree) {
                void* temp = _TryAlloc(cur, size);
                if (temp != nullptr) {
                    return (Header*)temp + 1;
                }
            }
            cur = cur->_next;
        }
        // Alloc fail
        return nullptr;
    }
    // free memory
    // input - ptr that the allocated memory starts
    void Heap::Free(void* ptr)
    {
        // Backtrack to start of header and cast to header
        Header* hd = reinterpret_cast<Header*>((char*)ptr - sizeof(Header));
        hd->isFree = true;
    }
    // how many bytes are currently allocated
    size_t Heap::Used() const
    {
        size_t result = (size_t)0;
        if (!_Start.isFree) {
            result += _Start.m_size;
        }
        Header* cur = _Start._next;
        while (cur != nullptr) {
            if (!(cur->isFree)) {
                result += cur->m_size;
            }
            cur = cur->_next;
        }
        return result;
       
    }
    // how many bytes are currently free
    size_t Heap::Available() const
    {
        size_t result = (size_t)0;
        if (_Start.isFree) {
            result += _Start.m_size;
        }
        Header* cur = _Start._next;
        while (cur != nullptr) {
            if (cur->isFree) {
                result += cur->m_size;
            }
            cur = cur->_next;
        }
        return result;
    }
    // how many bytes are currently devoted to Headers or other overhead
    // note: the sum of these should match the requested size for the heap (including any padding for alignment)
    size_t Heap::Overhead() const
    {
        size_t result = (size_t)0;
        Header* cur = _Start._next;
        while (cur != nullptr) {
            result += sizeof(Header);
            cur = cur->_next;
        }
        return result;
    }
    // does the requested pointer fall into the range of memory the heap owns ? 
    // (may return true even if the ptr does not point to an active block!)
    bool Heap::Contains(const void* ptr) const
    {
        // Compute heap end address
        void* end = (char*)this + _heapsize;
        if (ptr <= end && ptr >= this) return true;
        return false;
    }
    /// <summary>
    /// Heap constructor
    /// </summary>
    /// <param name="name"></param>
    /// <param name="size"></param>
    Heap::Heap(const char* name, size_t size)
    {
        // assert size is already 8 byte aligned
        FIEA_ASSERT(size % 8 == (size_t)0);
        _name = name;
        // Init _Start using placement new and aggregate initialization
        new(&_Start) Header{ size, nullptr, true };
        _heapsize = size;

    }
    Heap::~Heap()
    {
        // Destruct start cuz using new
        _Start.~Header();
    }
    /// <summary>
    /// Helper function to align 8 byte
    /// </summary>
    /// <param name="size"></param>
    /// <returns></returns>
    size_t Heap::AlignedSize(size_t size)
    {
        return (size + 7) & ~static_cast<size_t>(7);
    }

    // attempt an allocation of the requested size...
    // return nullptr when failure
    void* Heap::_TryAlloc(Header* current, size_t size)
    {
        // Coalesce blocks before checking sizes
        _TryCoalesce(current);
        if (current->m_size >= size) {
            size_t newSize = current->m_size - size;
            current->isFree = false;

            // Split block or not
            if (newSize <= ALIGNMENT) {
                // allocate directly without splitting
                current->isFree = false;
                /*_allocated += size;*/
                return current;
            }
            else {
                // allocate by splitting a new block with newSize as m_Size
                current->isFree = false;
                size_t temp = current->m_size;
                current->m_size = size;
                // offset next by size + 1 header size
                Header* newHeader = reinterpret_cast<Header*>(reinterpret_cast<char*>(current) + size + sizeof(Header));
                current->_next = newHeader;
                newHeader->_next = nullptr;
                newHeader->m_size = temp - size - sizeof(Header);
                newHeader->isFree = true;
                return current;
            }
        }
        
        return nullptr;
    }
    // Internal helper to coalesce free blocks, reduce fragmentation
    Heap::Header* Heap::_TryCoalesce(Header * current) {
        // Check if next is not null and is free, merge with self
        if (current->_next != nullptr && current->_next->isFree) {
            // merge next with self
            Header* c_next = current->_next;
            Header* c_nextnext = c_next->_next;
            // connect next's next with current
            current->_next = c_nextnext;
            // set merged size
            current->m_size += c_next->m_size + sizeof(Header);
        }
        return current;
    }
    // helper to get from one block to the next... could this function as a loop increment?
   /* Heap::Header* Heap::_Next(Header* current) const
    {
        return current->_next;
    }*/
}

