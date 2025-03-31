#pragma once
#include "FieaGameEngine/ServiceMgr.h"
#include "FieaGameEngine/Types.h"

namespace Fiea::Engine::Test::Support {
    struct INameGiver {
        FIEA_SERVICE_INTERFACE;
        virtual string GiveName(const char* key) const = 0;
    };
    /**
     * Test class
     */
    class StringManager : INameGiver
    {
    public:
        string GiveName(const char* key) const override;
    private:
        std::unordered_map<string, string> _stringMap;
        /**
        * Static registrate service
        */
        static bool _Registration();
        static inline bool _register = _Registration();
    };
}
