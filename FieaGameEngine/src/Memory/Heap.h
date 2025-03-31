#pragma once
#include "FieaGameEngine/Types.h"
#include "SystemAllocator.h"
namespace Fiea::Engine::Memory {
    using string = std::basic_string<char, std::char_traits<char>, SystemAllocator<char>>;

    class Heap
    {
    public:
        static Heap* CreateHeap(const char* name, size_t size); // use malloc and placement new
        static void DestroyHeap(Heap* heap); // call destructor explicitly and use free
        const string& GetName() const;       // simple accessor
        [[nodiscard]]
        void* Alloc(size_t size);            // allocate memory
        void Free(void* ptr);                // free memory
        size_t Used() const;                 // how many bytes are currently allocated
        size_t Available() const;            // how many bytes are currently free
        size_t Overhead() const;             // how many bytes are currently devoted to Headers or other overhead
        // note: the sum of these should match the requested size for the heap (including any padding for alignment)
        bool Contains(const void* ptr) const;// does the requested pointer fall into the range of memory the heap owns? (may return true even if the ptr does not point to an active block!)
    
    private:
        Heap(const char* name, size_t size); // private, will only be invoked from CreateHeap, may add params if needed
        ~Heap(); // not virtual, what modifier should we consider for the class declaration?
        size_t _heapsize;
        string _name;
        struct Header {
            size_t m_size;
            Header* _next;
            bool isFree;
        }; 
        static const size_t ALIGNMENT = 2 * sizeof(Header);                  // you may pad your allocations to increments of ALIGNMENT
        //   you may also force (with code) a heap size that prevents "leftover" blocks that are too small to allocate
        //   if desired, you could instead take alignment as a heap parameter and make alignment a property of the heap
        static size_t AlignedSize(size_t size);
        void* _TryAlloc(Header* current, size_t size);  // attempt an allocation of the requested size... what to return, on failure?
        Header* _TryCoalesce(Header* current);
        //Header* _Next(Header* current) const;           // helper to get from one block to the next... could this function as a loop increment?
        Header _Start;                                  // by making this the last member field, may be able to improve code readability
    };
}


