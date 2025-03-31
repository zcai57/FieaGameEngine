#include "Tracker.h"
#include "SystemAllocator.h"
//#include "FieaGameEngine/Types.h"
namespace Fiea::Engine::Memory
{

#ifdef FIEA_MEMORY_DEBUG
    // Override unordered_map with System Allocator
    using trackerMap = std::unordered_map<void*, Tracker::DebugMemoryInfo,
        std::hash<void*>,
        std::equal_to<void*>,
        SystemAllocator<std::pair<void* const, Tracker::DebugMemoryInfo>>>;
    trackerMap trackerData;
    // record the information for this allocation
    void Tracker::track(void* ptr, const char* heapName, const char* label, const char* file, size_t line)
    {
        DebugMemoryInfo alloc1 = {heapName, label, file, line};
        trackerData.insert({ ptr, alloc1 });
    }
    // discard the information for this allocation
    void Tracker::Untrack(void* ptr)
    {
        trackerData.erase(ptr);
    }
    // return the number of recorded allocations in the requested heap, or all heaps if none is provided
    size_t Tracker::Count(const char* heapName) const
    {
        size_t count = (size_t)0;
        for (const auto& i : trackerData)
        {
            if (i.second._heapName == heapName) {
                ++count;
            }
        }
        return count;
    }
    // output the debug information in CSV for all of the allocations in the requested heap,
    // or all heaps if none is provided
    void Tracker::Report(std::ostream& output, const char* heapName) const
    {
        if (heapName != nullptr) {
            for (const auto& i : trackerData)
            {
                if (i.second._heapName == heapName) {
                    // output it to file
                    output << "Address: " << i.first
                        << " | Heapname: " << i.second._heapName
                        << " | Label: " << i.second._label
                        << " | Line: " << i.second._line << "\n";
                }
            }
        }
        else {
            for (const auto& i : trackerData)
            {
                // output all
                output << "Address: " << i.first
                    << " | Heapname: " << i.second._heapName
                    << " | Label: " << i.second._label
                    << " | Line: " << i.second._line << "\n";
            }
        }
        
    }

#endif // FIEA_MEMORY_DEBUG
}
