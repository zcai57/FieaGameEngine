#pragma once
#include "../include/FieaGameEngine/Types.h"
namespace Fiea::Engine::Memory
{
#ifdef FIEA_MEMORY_DEBUG
    class Tracker {
    public:
        // record the information for this allocation
        void track(void* ptr, const char* heapName, const char* label, const char* file, size_t line);
        // discard the information for this allocation
        void Untrack(void* ptr);
        // return the number of recorded allocations in the requested heap, or all heaps if none is provided
        size_t Count(const char* heapName = nullptr) const;
        // output the debug information in CSV for all of the allocations in the requested heap,
        // or all heaps if none is provided
        void Report(std::ostream& output, const char* heapName = nullptr) const;
        /*Tracker() = default;*/
        // Rule of three
        // Assignment
        /*Tracker& operator=(Tracker& rhs) = default;*/
        // Copy
        //Tracker(const Tracker& other) = default;
        // Destruct
       /* ~Tracker() = default;*/
        // a structure to hold the debug information for a single allocation
        struct DebugMemoryInfo {
            const char* _heapName;
            const char* _label;
            const char* _file;
            size_t _line;
        };
    //public:
    };
#endif // FIEA_MEMORY_DEBUG
}

