#pragma once
#include "FieaGameEngine/ServiceMgr.h"

namespace Fiea::Engine::Clock
{
    struct ITimeProvider
    {
        FIEA_SERVICE_INTERFACE;
        // in seconds, since "epoch"
        virtual uint32_t Time() = 0;
    };

    struct ITimeUpdater : ITimeProvider
    {
        FIEA_SERVICE_INTERFACE;
        // if newTime is zero, just use the system clock, return previous time
        virtual uint32_t Update(uint32_t newTime = 0) = 0;
    };
    class ClockService : ITimeUpdater
    {
        virtual uint32_t Time() override;
        virtual uint32_t Update(uint32_t newTime = 0) override;
    private:
        uint32_t _RecordedTime = 0;
        static bool _Registration();
        static inline bool _registrate = _Registration();
    };
}


