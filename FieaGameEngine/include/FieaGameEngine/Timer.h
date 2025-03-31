#pragma once
#include "FieaGameEngine/Types.h"
#include <chrono>

namespace Fiea::Engine
{
    using Clock = std::chrono::high_resolution_clock;

    /// @brief  A simple wrapper class for doing ad-hoc profiling
    ///
    /// Usage (single measure):
    ///   Timer t;
    ///   /* operation to measure */
    ///   double time = t.Elapsed();
    class Timer
    {
    public:
        Timer() : _Clock(), _Start(_Clock.now()) {}

        /// @brief Retrieves the time since start or last reset
        /// @return The elapsed time, in milliseconds
        double Elapsed() const
        {
            return std::chrono::duration<double, std::milli>(_Clock.now() - _Start).count();
        }

        /// @brief Resets the start time
        void Reset()
        {
            _Start = _Clock.now();
        }

    private:
        Clock               _Clock;
        Clock::time_point   _Start;
    };
}
