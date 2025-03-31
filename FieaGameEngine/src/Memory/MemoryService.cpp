#include "FieaGameEngine/Types.h"
#include "FieaGameEngine/MemoryService.h"
#include "FieaGameEngine/ServiceMgr.h"
#include "Tracker.h"
#include "SystemAllocator.h"
#include "Heap.h"
namespace Fiea::Engine::Memory
{
    // IDefaultAllocator interface
    struct IDefaultAllocator {
        struct AllocatorInfo {
            const Allocator& _allocator;
#ifdef FIEA_MEMORY_DEBUG
            const char* _file;
            size_t _line;
#endif // FIEA_MEMORY_DEBUG
        };
        //Perform an allocation using the allocator at the top of the stack of default allocators, 
        // or malloc if no allocators have been pushed
        virtual void* DefaultAlloc(size_t size) = 0;
        // Determine the heap that the allocation was made from, and free it
        // Attempt to use free() if the allocation is not from one of our existing Heaps
        virtual void DefaultFree(void* ptr) = 0;
    };

    // Override memory system types for MemorySubSystem
    using string = std::basic_string<char, std::char_traits<char>, SystemAllocator<char>>;
    using unordered_map = std::unordered_map<string, Heap*,
        std::hash<string>,
        std::equal_to<string>,
        SystemAllocator<std::pair<const string, Heap*>>>;
    using AllocatorStack = std::stack <IDefaultAllocator::AllocatorInfo, 
        std::deque<IDefaultAllocator::AllocatorInfo,
        SystemAllocator<IDefaultAllocator::AllocatorInfo>>>;

    // Memory Service
    class MemoryService : public IMemoryService, public IAllocatorProvider, public IDefaultAllocator
    {
    public:
        // Chicken and egg fix for gloabl new and delete
        static IDefaultAllocator* _defaultAlloc;
        unordered_map heapMp;
        AllocatorStack allocStack;
#ifdef FIEA_MEMORY_DEBUG
        Tracker tracker;
#endif // FIEA_MEMORY_DEBUG
        MemoryService() {
            _defaultAlloc = this;
        }

        ~MemoryService() {
            _defaultAlloc = nullptr;
        }
        // IMemoryService
        Heap* CreateHeap(const char* name, size_t size) override
        {
            Heap* hp = Heap::CreateHeap(name, size);
            heapMp.insert({ string(name), hp });
            return hp;
        }
        // IMemoryService
        void DestroyHeap(Heap* heap) override
        {
            Heap::DestroyHeap(heap);
        }
        // IDefault Allocator - For global new and delete
        // Perform an allocation using the allocator at the top of the stack of default allocators, 
        // or malloc if no allocators have been pushed
        void* DefaultAlloc(size_t size) override
        {
            if (allocStack.size() == size_t(0)) {
                // malloc if no allocator
                return malloc(size);
            }
            // Get allocator from top of stack
            AllocatorInfo& a1 = allocStack.top();
            // use stack to pass in file and line
#ifdef FIEA_MEMORY_DEBUG
            return a1._allocator.Alloc(size, a1._file, a1._line);
#else
            return a1._allocator.Alloc(size);
#endif
    }
        // IDefault Allocator - For global new and delete
        // Determine the heap that the allocation was made from, and free it
        // Attempt to use free() if the allocation is not from one of our existing Heaps
        void DefaultFree(void* ptr) override
        {
#ifdef FIEA_MEMORY_DEBUG
            tracker.Untrack(ptr); // Free tracker
#endif
            bool freed = false;
            // Use heap contains to determin heap
            for (auto i : heapMp) {
                if (i.second->Contains(ptr)) {
                    i.second->Free(ptr);
                    freed = true;
                    break;
                }
            }
            if (ptr == nullptr) return;
            // attemp free anyway, since fallback for DefaultAlloc is malloc
            // there is no need to do destroy at
            if(!freed) free(ptr);
        }
        // IAllocatorProvider
        // return an allocator that can allocate from a Heap
        Allocator GetAllocator(Heap* heap, const char* label = nullptr) override
        {
#ifdef FIEA_MEMORY_DEBUG
            return Allocator(*heap, label, tracker);
#else
            return Allocator(*heap, label);
#endif
        }
        // IAllocatorProvider
        // return an allocator that can allocate from a Heap
        Allocator GetAllocator(const char* heapName, const char* label = nullptr)
        {
            Heap* hp = nullptr;
            // Check valid heapname
            if (heapMp.find(heapName) == heapMp.end()) {
                FIEA_ACCESS_VIOLATION;
            }
            else {
                // get heap based on name
                hp = heapMp[string(heapName)];
            }
#ifdef FIEA_MEMORY_DEBUG
            return Allocator(*hp, label, tracker);
#else 
            return Allocator(*hp, label);
#endif // FIEA_MEMORY_DEBUG
        }
#ifdef FIEA_MEMORY_DEBUG
        // IMemoryService
        // pass through to Tracker::Count
        size_t CountAllocations(const Heap* heap = nullptr) const override
        {
            if (heap != nullptr) 
            {
                return tracker.Count(heap->GetName().c_str());
            }
            return (size_t)0;
        }
        // IMemoryService
        // Output diagnostic data to the provided ostream for the given heap, or all heaps if a heap is not provided
        // Heap Statistics - Name, Used bytes, Free bytes, Overhead bytes
        // Allocation information in CSV via Tracker::Report
        void DumpAllocations(std::ostream& output, const Heap* heap = nullptr) const override
        {
            if (heap == nullptr) {
                tracker.Report(output, nullptr);
            }
            else {
                tracker.Report(output, heap->GetName().c_str());
            }
        }

       /* Tracker& getTracker() override{
            return tracker;
        }*/
        // IAllocatorProvider
        // Push Allocator into stack<IDefaultAllocator::AllocatorInfo>
        void PushDefaultAllocator(const Allocator& al, const char* file, size_t line) override 
        {
            AllocatorInfo allocInfo = { al, file, line };
            allocStack.push(allocInfo);
        }
#else 
        void PushDefaultAllocator(const Allocator& al) override
        {
            AllocatorInfo allocInfo = { al };
            allocStack.push(allocInfo);
        }
        
#endif  // FIEA_MEMORY_DEBUG
        // IAllocatorProvider
        // Pop the allocator from stack<IDefaultAllocator::AllocatorInfo>
        virtual void PopDefaultAllocator(const Allocator& al) override
        {
            allocStack.pop();
        }
    private:
        static bool _Registration();
        static inline bool _register = _Registration();
};
// end of scope for MemoryService
    // Initialization for static member field
    IDefaultAllocator* MemoryService::_defaultAlloc = nullptr;
    // Registration of services for MemoryService
    FIEA_SERVICE_BIND2(MemoryService, IMemoryService, IAllocatorProvider);
#ifdef FIEA_MEMORY_DEBUG
    // Allocator class - Alloc with tracking
    void* Allocator::Alloc(size_t size, const char* file, size_t line) const
    {
        void* alloc1 = _Heap.Alloc(size);
        _Tracker.track(alloc1, _Heap.GetName().c_str(), _Label, file, line);
        return alloc1;
    }
    // Allocator class - Free with untracking
    void Allocator::Free(void* ptr) const
    {
        _Tracker.Untrack(ptr);
        _Heap.Free(ptr);
    }
    // Allocator class constructor with tracker
    Allocator::Allocator(Heap& heap, const char* label, Tracker& tracker) : _Heap(heap), _Tracker(tracker)
    {
        strncpy(_Label, label , MAX_LABEL_SIZE);
        _Label[MAX_LABEL_SIZE - 1] = '\0';
    }
//#endif // FIEA_MEMORY_DEBUG
#else
    void* Allocator::Alloc(size_t size) const
    {
        return _Heap.Alloc(size);
    }
    void Allocator::Free(void* ptr) const
    {
        _Heap.Free(ptr);
    }

    Allocator::Allocator(Heap& heap, const char* label) : _Heap(heap) {
        strncpy(_Label, label, MAX_LABEL_SIZE);
        _Label[MAX_LABEL_SIZE - 1] = '\0';
    }
#endif // FIEA_MEMORY_DEBUG
    // Free function - IMemoryService instance
    /*IMemoryService* GetMemoryService()
    {
        return MemoryService::Instance();
    }*/
    // Free function - IAllocatorProvider instance
   /* IAllocatorProvider* GetAllocatorProvider()
    {
        return MemoryService::Instance();
    }*/
    // Create Memory service instance
    /*void CreateMemoryService()
    {
        return MemoryService::Create();
    }*/
    // Destroy Memory service instance
    /*void DestroyMemoryService()
    {
        return MemoryService::Destroy();
    }*/
    // Create IDefaultAllocator instance
    /*static IDefaultAllocator* GetDefaultAllocator() 
    {
        return MemoryService::Instance();
    }*/

}


#ifdef FIEA_MEMORY_DEBUG
// New with allocator and tracker
void* operator new(size_t size, const Fiea::Engine::Memory::Allocator& alloc, const char* file, size_t line) {
    return alloc.Alloc(size, file, line);
}
// Delete with allocator and tracker
void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator& alloc, const char* file, size_t line) {
    return alloc.Free(ptr);
}
#else
// New with allocator
void* operator new(size_t size, const Fiea::Engine::Memory::Allocator& alloc) {
    return alloc.Alloc(size);
}
// Delete with allocator
void operator delete(void* ptr, const Fiea::Engine::Memory::Allocator& alloc) {
    return alloc.Free(ptr);
}
#endif
// Global new override
void* operator new(size_t size)
{
    // Edge case for 0 size allocation
    FIEA_ASSERT(size != 0);
    if (size == 0) {
        ++size;
    }
    // Get IDefaultAllocator instance
    Fiea::Engine::Memory::IDefaultAllocator* defAlloc = Fiea::Engine::Memory::MemoryService::_defaultAlloc;
    if (defAlloc == nullptr) {
        // if instance is null, fall back to malloc
        void* ptr = malloc(size);
        if (!ptr) throw std::bad_alloc();
        return ptr;
    }
    // Call defaultalloc
    return defAlloc->DefaultAlloc(size);
}
// Global delete override
void operator delete(void* ptr)
{
    Fiea::Engine::Memory::IDefaultAllocator* defAlloc = Fiea::Engine::Memory::MemoryService::_defaultAlloc;
    if (defAlloc != nullptr) {
        // defaultFree
        defAlloc->DefaultFree(ptr);
    }
    else {
        // Free malloc
        free(ptr);
    }
}