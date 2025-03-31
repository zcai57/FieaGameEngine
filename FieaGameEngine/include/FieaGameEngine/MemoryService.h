#pragma once
#include "ServiceMgr.h"
#include "../src/Memory/Tracker.h"

#ifdef FIEA_MEMORY_DEBUG
#define FDELETE(_al, _ptr)    \
    std::destroy_at(_ptr);				\
	::operator delete(_ptr, _al, nullptr, (size_t)0);

#define FNEW(_al, _type, __FILE__, __LINE__)      \
    new (_al, __FILE__, __LINE__) _type;

#define FALLOC(_al, _size, __FILE__, __LINE__)    \
    _al.Alloc(_size, __FILE__, __LINE__);

#define FFREE(_al, _ptr)                          \
    _al.Free(_ptr);
#else
// delete _ptr using _al with the correct operator delete 
#define FDELETE(_al, _ptr)			    \
	std::destroy_at(_ptr);				\
	::operator delete(_ptr, _al);	
// new an object of _type, using allocator _al and our new form of operator new
#define FNEW(_al, _type)      new (_al) _type;
// allocate the requested amount of memory using allocator _al
#define FALLOC(_al, _size)    _al.Alloc(_size);
// free the requested pointer using allocator _al
#define FFREE(_al, _ptr)      _al.Free(_ptr);
#endif // FIEA_MEMORY_DEBUG


namespace Fiea::Engine::Memory
{
    class Heap;
    //class MemoryService;
    struct Allocator
    {
    public:
        [[nodiscard]]
#ifdef FIEA_MEMORY_DEBUG
        void* Alloc(size_t size, const char* file, size_t line) const;
        void Free(void* ptr) const;
#else
        void* Alloc(size_t size) const;
        void Free(void* ptr) const;
#endif // FIEA_MEMORY_DEBUG
        friend struct IMemoryService;
        friend class MemoryService;
        Allocator(const Allocator& other) = default;
        Allocator& operator=(const Allocator& rhs) = default;
        ~Allocator() = default;
    private:
        Allocator() = delete; // Allocator() cannot be called
        // Allocator constructor
        Allocator(Heap& heap, const char* label);
#ifdef FIEA_MEMORY_DEBUG
        Allocator(Heap& heap, const char* label, Tracker& tracker);
        Tracker& _Tracker;
#endif
        // Can't use this if not static because you can't use member
        // variables at compile time
        static inline const size_t MAX_LABEL_SIZE = 16;

        //const char* _Label; - Can't rely on a pointer since it can be lost with 
        //                      scope
        Heap& _Heap;
        char _Label[MAX_LABEL_SIZE];
    };

    struct IMemoryService
    {
        FIEA_SERVICE_INTERFACE;
        // remove static CreateHeap method from Heap class and implement here
        virtual Heap* CreateHeap(const char* name, size_t size) = 0;
        // remove static DestroyHeap method from Heap class and implement here
        virtual void DestroyHeap(Heap* heap) = 0;
#ifdef FIEA_MEMORY_DEBUG
        virtual size_t CountAllocations(const Heap* heap = nullptr) const = 0;
        virtual void DumpAllocations(std::ostream& output, const Heap* heap = nullptr) const = 0;
        //virtual Tracker& getTracker() = 0;
#else
        virtual void PushDefaultAllocator(const Allocator&) = 0;

#endif // FIEA_MEMORY_DEBUG

    };

    struct IAllocatorProvider
    {
        FIEA_SERVICE_INTERFACE;
        // return an allocator that can allocate from a Heap
        virtual Allocator GetAllocator(Heap* heap, const char* label = nullptr) = 0;
        // return an allocator that can allocate from a Heap
        virtual Allocator GetAllocator(const char* heapName, const char* label = nullptr) = 0;
#ifdef FIEA_MEMORY_DEBUG
        virtual void PushDefaultAllocator(const Allocator& al, const char* file, size_t line) = 0;
#else
        virtual void PushDefaultAllocator(const Allocator& al) = 0;
#endif // FIEA_MEMORY_DEBUG
        virtual void PopDefaultAllocator(const Allocator& al) = 0;

    };
}

#ifdef FIEA_MEMORY_DEBUG
void* operator new(size_t size, const Fiea::Engine::Memory::Allocator& alloc, const char* file, size_t line);
void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator& alloc, const char* file, size_t line);
#else
void* operator new(size_t size, const Fiea::Engine::Memory::Allocator&);
void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator&);
#endif // FIEA_MEMORY_DEBUG