#include "FieaGameEngine/ClockService.h"
#include <chrono>

namespace Fiea::Engine::Clock
{
    FIEA_SERVICE_BIND2(ClockService, ITimeProvider, ITimeUpdater);
    /**
     * in seconds, since "epoch"
     */
    uint32_t ClockService::Time()
    {
        const auto p1 = std::chrono::system_clock::now();
        return (uint32_t)std::chrono::duration_cast<std::chrono::seconds>(p1.time_since_epoch()).count();
    }
    /**
     * if newTime is zero, return time since epoch
     * Otherwise return previous time
     */
    uint32_t ClockService::Update(uint32_t newTime)
    {
        uint32_t prevTime = _RecordedTime;
        if (newTime == 0) {
            Time();
        }
        _RecordedTime = newTime;
        return prevTime;
    }

}
